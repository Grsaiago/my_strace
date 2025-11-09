#include <elf.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

typedef enum {
	ENTER,
	EXIT,
} SyscallState;

int start_traced_process(char *cmd) {
	extern char           **environ;
	char                   *argv[] = {"/bin/ls", NULL};
	int                     child_pid;
	int                     err;
	int                     child_status;
	int                     pid_notification;
	struct user_regs_struct registers;
	struct iovec            iov;
	SyscallState            syscall_state = ENTER;

	// https://man7.org/linux/man-pages/man2/ptrace.2.html#:~:text=sizeof(sigset_t)).-,PTRACE_SETOPTIONS,-(since%20Linux%202.4.6
	child_pid = fork();
	if (child_pid == 0) {
		err = ptrace(PTRACE_TRACEME);
		if (err != 0) {
			printf("failed to PTRACE_TRACEME in child process\n");
			return (EXIT_FAILURE);
		}
		kill(getpid(), SIGSTOP); // ensures parent sees a stop before execve
		execve(cmd, argv, environ);
	}
	pid_notification = waitpid(child_pid, &child_status, WUNTRACED);
	if (WSTOPSIG(child_status) != SIGSTOP) {
		printf(
		    "the first signal was not a sigtrap, something went wrong and the "
		    "process didn't stop right after the execve as per man\n"
		);
		return (EXIT_FAILURE);
	}
	printf(
	    "the process stopped before the execve, setting child process "
	    "configs...\n"
	);
	// config the child before it starts to execute user code
	ptrace(
	    PTRACE_SETOPTIONS, child_pid, NULL,
	    PTRACE_O_EXITKILL           // propagate a received SIGKILL to the
	                                // child
	        | PTRACE_O_TRACEEXIT    // stop the tracee at exit
	        | PTRACE_O_TRACESYSGOOD // set syscall bit on waitpid
	);
	err = ptrace(
	    PTRACE_SYSCALL, child_pid, 0, 0
	); // continue the child until another syscall
	while (42) {
		pid_notification = waitpid(child_pid, &child_status, 0);
		if (pid_notification == -1 ||
		    (WIFEXITED(child_status) || WIFSIGNALED(child_status))) {
			printf("processo filho terminou\n");
			break;
		}

		if (WIFSTOPPED(child_status) &&
		    WSTOPSIG(child_status) == (SIGTRAP | 0x80)) {
			iov = (struct iovec){.iov_base = &registers,
			                     .iov_len = sizeof(struct user_regs_struct)};
			ptrace(PTRACE_GETREGSET, child_pid, NT_PRSTATUS, &iov);

			switch (syscall_state) {
			case (ENTER):
				printf("rolou a syscall %lld\n", registers.orig_rax);
				break;
			case (EXIT):
				printf("rolou a syscall %lld\n", registers.rax);
				break;
			}
			syscall_state ^= 1; // toggle state so it reads in/out alternate
		}
		err = ptrace(
		    PTRACE_SYSCALL, child_pid, 0, 0
		); // continue the child until another syscall
	}
	return (EXIT_SUCCESS);
}

int main(void) {
	start_traced_process("/bin/ls");
	return (EXIT_SUCCESS);
}

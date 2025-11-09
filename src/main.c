#include "my_strace.h"

int main(void) { try_find_cmd("ls"); }
// int main(int argc, char **argv) {
// 	extern char           **environ;
// 	pid_t                   child_pid;
// 	int                     err;
// 	int                     child_status;
// 	int                     pid_notification;
// 	char                   *found_cmd;
// 	struct user_regs_struct registers;
// 	struct iovec            iov;
// 	SyscallState            syscall_state = ENTER;

// 	if (argc < 2) {
// 		dprintf(STDERR_FILENO, "Usage: %s <program> [args...]\n", argv[0]);
// 		return (EXIT_FAILURE);
// 	}

// 	found_cmd = try_find_cmd(argv[1]);
// 	if (!found_cmd) {
// 		dprintf(STDERR_FILENO, "%s: command not found\n", argv[1]);
// 		return (EXIT_FAILURE);
// 	}

// 	child_pid = spawn_tracee();
// 	//
// https://man7.org/linux/man-pages/man2/ptrace.2.html#:~:text=sizeof(sigset_t)).-,PTRACE_SETOPTIONS,-(since%20Linux%202.4.6
// 	while (42) {
// 		pid_notification = waitpid(child_pid, &child_status, 0);
// 		if (pid_notification == -1 ||
// 		    (WIFEXITED(child_status) || WIFSIGNALED(child_status))) {
// 			printf("processo filho terminou\n");
// 			break;
// 		}

// 		if (WIFSTOPPED(child_status) &&
// 		    WSTOPSIG(child_status) == (SIGTRAP | 0x80)) {
// 			iov = (struct iovec){.iov_base = &registers,
// 			                     .iov_len = sizeof(struct user_regs_struct)};
// 			ptrace(PTRACE_GETREGSET, child_pid, NT_PRSTATUS, &iov);

// 			switch (syscall_state) {
// 			case (ENTER):
// 				printf("rolou a syscall %lld\n", registers.orig_rax);
// 				break;
// 			case (EXIT):
// 				printf("rolou a syscall %lld\n", registers.rax);
// 				break;
// 			}
// 			syscall_state ^= 1; // toggle state so it reads in/out alternate
// 		}
// 		err = ptrace(
// 		    PTRACE_SYSCALL, child_pid, 0, 0
// 		); // continue the child until another syscall
// 	}
// 	return (EXIT_SUCCESS);
// }

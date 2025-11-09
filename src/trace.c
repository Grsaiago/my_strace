#include "my_strace.h"

/**
 * @brief spawn a child process to be traced
 *
 * Creates a new child process that will execute the program specified by @path.
 * The child process is set up to be traced by the parent process.
 *
 * @path: path to the executable to run
 * @ret: On success, returns the child process PID. On error, returns -1.
 */
int spawn_tracee(char **const path_with_args) {
	pid_t         child_pid;
	int           pid_notification;
	int           child_status;
	int           err;
	extern char **environ;

	child_pid = fork();
	if (child_pid == -1) {
		dprintf(STDERR_FILENO, "failed to fork\n");
		return (EXIT_FAILURE);
	}
	if (child_pid == 0) {
		err = ptrace(PTRACE_TRACEME);
		if (err != 0) {
			printf("failed to PTRACE_TRACEME in child process\n");
			return (EXIT_FAILURE);
		}
		kill(getpid(), SIGSTOP); // ensures parent sees a stop before execve
		execve(path_with_args[0], path_with_args, environ);
	}
	pid_notification = waitpid(child_pid, &child_status, WUNTRACED);
	if (pid_notification == -1) {
		kill(child_pid, SIGKILL);
		dprintf(STDERR_FILENO, "failed to wait for child process\n");
		return (EXIT_FAILURE);
	}
	if (WSTOPSIG(child_status) != SIGSTOP) {
		return (EXIT_FAILURE);
	}
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
	return (child_pid);
}

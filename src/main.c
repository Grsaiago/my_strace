#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <unistd.h>

int start_traced_process(char *cmd) {
	extern char **environ;
	char         *argv[] = {"/bin/ls", NULL};
	int           pid;
	int           err;

	pid = fork();
	if (pid == 0) {
		err = ptrace(PTRACE_TRACEME);
		if (err != 0) {
			printf("failed to PTRACE_TRACME in child process\n");
			return (EXIT_FAILURE);
		}
		execve(cmd, argv, environ);
	}
	return (EXIT_SUCCESS);
}

int main(void) {
	start_traced_process("/bin/ls");
	return (EXIT_SUCCESS);
}

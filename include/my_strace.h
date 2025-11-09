#ifndef _MY_STRACE_H
#define _MY_STRACE_H

#include <elf.h>
#include <string.h>
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

typedef enum {
    NONE = 0,
	FIRST = 1 << 0,
	SECOND = 1 << 1,
} JoinFreePolicy;

/* aux functions */
char *strjoin(char *s1, char *s2, JoinFreePolicy free_policy);
char *try_find_cmd(char *cmd);
int spawn_tracee(char **const path_with_args);
#endif // _MY_STRACE_H

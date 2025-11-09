# my_strace
My C implementation of the strace command

## TODO:

- [ ] Create the map for syscall number/name and args
- [ ] Handle non-syscall signals (and forwarding).
      If the tracee is stopped for a real signal (e.g. SIGSEGV, SIGCHLD from a child, or user-delivered signals), WSTOPSIG will be that signal (not SIGTRAP|0x80).
      I should then forward it to the tracee by passing that signal as the data argument to ptrace(PTRACE_SYSCALL, ..., sig) so it gets delivered.
      Not doing so may leave some signals unhandled and confuse the tracee’s execution.
- [ ] Check for ptrace return values / errno.

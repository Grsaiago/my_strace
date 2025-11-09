#include "my_strace.h"

char *strjoin(char *s1, char *s2, JoinFreePolicy free_policy) {
	size_t len1;
	size_t len2;
	char  *result;

	if (!s1 || !s2)
		return NULL;

	len1 = strlen(s1);
	len2 = strlen(s2);

	result = (char *)malloc(len1 + len2 + 1);
	if (!result)
		return NULL;

	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2);
	result[len1 + len2] = '\0';

	if (free_policy & FIRST) {
		free((void *)s1);
	}
	if (free_policy & SECOND) {
		free((void *)s2);
	}

	return result;
}

char *try_find_cmd(char *cmd) {
	char *curr_path;
	char *path_str;
	char *full_path_candidate;

	if (access(cmd, X_OK) == 0) {
		return strdup(cmd);
	}
	path_str = getenv("PATH");
	if (!path_str) {
		dprintf(STDERR_FILENO, "failed to find PATH env variable\n");
		return (NULL);
	}

	// we have to duplicate the raw env string because strtok modifies the
	// string
	path_str = strdup(path_str);
	curr_path = strtok(path_str, ":");

	for (; curr_path != NULL; curr_path = strtok(NULL, ":")) {
		full_path_candidate = strjoin(curr_path, "/", NONE);
		full_path_candidate = strjoin(full_path_candidate, cmd, FIRST);
		if (access(full_path_candidate, X_OK) == 0) {
			free(path_str);
			return (full_path_candidate);
		}
		free(full_path_candidate);
	}
	free(path_str);
	return (NULL);
}

#include "my_strace.h"
#include <criterion/criterion.h>

// tem que resolver o leak aqui em
Test(find_cmd, find_ls) {
	struct test_candidate {
		char *cmd;
		char *expected_path;
	} candidates[] = {
	    {"ls", "/usr/bin/ls"},
	    {"echo", "/usr/bin/echo"},
	    {"grep", "/usr/bin/grep"},
	    {"cat", "/usr/bin/cat"},
	    {"nonexistentcommand", NULL},
	    {"nonexistentcommand", NULL},
	    {NULL, NULL}
	};
	struct test_candidate *candidate = candidates;

	for (; candidate->cmd != NULL; candidate++) {
		char *found_cmd = try_find_cmd(candidate->cmd);

		if (candidate->expected_path == NULL) {
			cr_assert_null(
			    found_cmd, "expected [%s] to be [%s]", found_cmd,
			    candidate->expected_path
			);
		} else {
			cr_assert_str_eq(
			    candidate->expected_path, found_cmd, "expected [%s] to be [%s]",
			    found_cmd, candidate->expected_path
			);
		}
		free(found_cmd);
	}
}

Test(find_cmd, find_the_test_suite) {
	char *found_cmd;
	char *expected = "./test_suite";

	found_cmd = try_find_cmd("./test_suite");
	cr_assert_str_eq(
	    expected, found_cmd, "expected [%s] to be [%s]", found_cmd, expected
	);
}

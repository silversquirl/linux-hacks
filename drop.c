// A command to run a program and drop privileges to a specified user
// Only works when run as euid root
// Has a similar effect to sudo -u USER CMD
#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE
#include <assert.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc <= 2) {
		fprintf(stderr, "Usage: drop USERNAME COMMAND [ARGS...]\n");
		return 1;
	}

	// Get the UID and GID to use
	struct passwd *pw = getpwnam(argv[1]);
	if (!pw) {
		perror("getpwnam");
		return 1;
	}

	// Get supplementary groups
	int ngroups = 0;
	int ret = getgrouplist(argv[1], pw->pw_gid, NULL, &ngroups);
	assert(ret < 0); // If we have 0 groups, something weird has happened
	gid_t groups[ngroups];
	ret = getgrouplist(argv[1], pw->pw_gid, groups, &ngroups);
	if (ret < 0) {
		perror("getgrouplist");
		return 1;
	}

	// Set supplementary groups
	if (setgroups(ngroups, groups) < 0) {
		perror("setgroups");
		return 1;
	}

	// Drop privs
	setgid(pw->pw_gid);
	setuid(pw->pw_uid);

	// Exec the program
	execvp(argv[2], argv+2);
}

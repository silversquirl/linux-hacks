#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <termios.h>

int main(int argc, char *argv[]) {
	if (argc <= 2) {
		fprintf(stderr, "Usage: cterm TTY COMMAND [ARGS]\n");
		return 1;
	}

	const char *tty = argv[1];
	int fd = open(tty, O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	switch (fork()) {
	case 0:
		break;
	case -1:
		perror("fork");
	default:
		wait(NULL);
		return 0;
	}

	if (setsid() < 0) {
		perror("setsid");
		return 1;
	}

	if (ioctl(fd, TIOCSCTTY, 1) < 0) {
		perror("ioctl(TIOCSCTTY)");
		return 1;
	}

	// Drop privs
	seteuid(getuid());
	setegid(getgid());

	execvp(argv[2], argv+2);
}

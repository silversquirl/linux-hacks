#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#include <linux/kd.h>

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "Usage: textmode TTY\n");
		return 1;
	}

	int fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	if (ioctl(fd, KDSETMODE, KD_TEXT) < 0) {
		perror("ioctl(KDSETMODE)");
		return 1;
	}
	
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

void	err_msg(const char *s) {
	write(2, s, strlen(s));
	exit(1);
}

int	servfd_set(int port) {
	int	fd;
	struct sockaddr addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		return -1;
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0 || listen(fd, 100) < 0) {
		close(fd);
		return -1;
	}
	return fd;
}

int	main(int argc, const char **argv) {
	if (argc < 2) {
		err_msg("Wrong number of arguments\n");
	}
	int port = atoi(argv[1]);
	servfd = servfd_set(port);
	if (servfd < 0) {
		err_msg("Fatal error\n");
	}
	return 0;
}

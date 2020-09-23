#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "../include/rerror.h"

const char *help = "--help";
int main(int argc, char **argv)
{
	if (argc < 2)
		rerror("Few arguments!\nType ./sent --help\n", 1);
	if (!strcmp(help, argv[1]))
		rerror("For single file:\n\t./sent [ip address] [port] [file]\nFor many files:\n\t./sent [ip address] [port] [file 1] [file 2] etc\n", 0);
	if (argc < 4)
		rerror("Few arguments!\nType ./sent --help\n", 1);
	int tmp_fd;
	for (int step = 3; step < argc; ++step) {
		tmp_fd = open(argv[step], O_RDONLY);
		if (-1 == tmp_fd) {
			write(2, argv[step], strlen(argv[step]));
			rerror(" - no such file\n", 1);
		} else
			close(tmp_fd);
	}

	int fd, addrlen;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == fd)
		rerror("socket-error\n", 1);

	addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in *addr = (struct sockaddr_in*)malloc(addrlen);
	if (!addr)
		rerror("alloc-error\n", 1);

	addr->sin_family = AF_INET;
	addr->sin_port = htons(atoi(argv[2]));
	if (!inet_aton(argv[1], &(addr->sin_addr)))
		rerror("ip-address-error\n", 1);
	
	if (connect(fd, (struct sockaddr*)addr, addrlen))
		rerror("connect-error\n", 1);
	
//	write();
	char buff[100];
	read(fd, buff, 100);
	printf("%s\n", buff);

	close(fd);
	free(addr);

	exit(0);
}

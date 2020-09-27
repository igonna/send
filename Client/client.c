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
#include <stdint.h>
#include "../include/rerror.h"
#include "../include/fsize.h"
#include "../include/gfnc.h"
#include "../include/exi_file.h"

const char *help = "--help";
int main(int argc, char **argv)
{
	if (argc < 2)
		rerror("Few arguments!\nType ./sent --help\n", 1);
	if (!strcmp(help, argv[1]))
		rerror("For single file:\n\t./sent [ip address] [port] [file]\nFor many files:\n\t./sent [ip address] [port] [file 1] [file 2] etc\n", 0);
	if (argc < 4)
		rerror("Few arguments!\nType ./sent --help\n", 1);
	for (int step = 3; step < argc; ++step) {
		if (!exi_file(argv[step])) {
			write(2, argv[step], strlen(argv[step]));
			rerror(" - no such file\n", 1);
		}
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
	
	//send file size to server	
	int file_size = fsize(argv[3]);
	int32_t nf_size = htonl(file_size);
	write(fd, &(nf_size), sizeof(nf_size));

	//read file data to buffer
	int ffd = open(argv[3], O_RDONLY);
	char *buff = (char*)malloc(file_size);
	read(ffd, buff, file_size);
	
	//sent file data
	write(fd, buff, file_size);
	free(buff);

	//getting clean file name, without '/' character	
	char *nf_name = (char*)malloc(1);
	gfnc(argv[3], nf_name);
	
	//sent file name size
	int32_t fname_size = strlen(nf_name) + 1;
	int hfname_size = fname_size;
	fname_size = htonl(fname_size);
	write(fd, &(fname_size), sizeof(fname_size));
	
	//sent file name
	write(fd, nf_name, hfname_size);
	free(nf_name);

	close(ffd);
	close(fd);
	free(addr);

	exit(0);
}

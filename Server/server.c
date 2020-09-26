#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/rerror.h"

const char *help = "--help";
int main(int argc, char **argv)
{
	if (argc < 2)
                rerror("Few arguments!\nType ./sent --help\n", 1);
        if (!strcmp(help, argv[1]) || argc < 3)
                rerror("./server [ip address] [port]\n", 0);
	int fd, addrlen;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		rerror("socket-error\n", 1);
	
	addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in *addr = (struct sockaddr_in*)malloc(addrlen);
	if (!addr)
		rerror("addr-error", 1);
	addr->sin_family = AF_INET;
	addr->sin_port = htons(atoi(argv[2]));
//	addr->sin_addr.s_addr = htonl(INADDR_ANY);
	if (!inet_aton(argv[1], &(addr->sin_addr)))
                rerror("ip-address-error\n", 1);

	if (bind(fd, (struct sockaddr*)addr, addrlen))
		rerror("bind-error\n", 1);
	
	if (listen(fd, 0))
		rerror("listen-error\n", 1);
	int cfd;
	cfd = accept(fd, (struct sockaddr*)addr, (socklen_t*)&addrlen);
	if (cfd < 0)
		rerror("accept-error\n", 1);

	//get file size from client
	int32_t file_size[1];
	read(cfd, file_size, sizeof(*file_size));
	*file_size = ntohl(*file_size);
	printf("file size: %d\n", *file_size);
	
	//read file data
	char *file_data = (char*)malloc(*file_size);
	read(cfd, file_data, *file_size);
	printf("file data: %s\n", file_data);

	//read file name size
	int32_t fname_size[1];
	read(cfd, fname_size, sizeof(*file_size));
	*fname_size = ntohl(*fname_size);
	printf("file name size: %d\n", *fname_size);	
	
	//read file name
	char *file_name = (char*)malloc(*fname_size);
	read(cfd, file_name, *fname_size);
	printf("file name: %s\n", file_name);

	free(file_name);
	free(file_data);
	close(fd);
	close(cfd);
	free(addr);
	exit(0);
}

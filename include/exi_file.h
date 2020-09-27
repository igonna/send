#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int exi_file(char *path)
{
	int fd = open(path, O_RDONLY);
	if (fd == -1)
		return 0;
	else
		return 1;
}

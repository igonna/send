#include <stdio.h>

int fsize(char *path)
{
	int file_size;
	FILE *fp = fopen(path, "r");
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fclose(fp);
	return file_size;
}	

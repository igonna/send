#ifndef STDLIB_H
	#include <stdlib.h>
#endif
#ifndef UNISTD_H
	#include <unistd.h>
#endif

void rerror(char* str, int status);

void rerror(char* str, int status)
{
        write(2, str, strlen(str));
        exit(status);
}

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define MAX 128

void main()
{

int fd;
char buffer[MAX]
fd=open("/dev/mydev",O_RDWR);

read(fd,buffer,128);

close(fd);
}



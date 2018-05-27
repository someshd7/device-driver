#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main()
{
    int fd;
    char Ubuff[50], Kbuff[50];

    fd = open("/dev/sample_cdev1", O_RDWR);
    if(fd < 0) {
        perror("Unable to open the device file\n");
        return -1;
    }

    printf("\nPlease enter a string");
    gets(Ubuff);

    /* Write the data into the device */
    write(fd , Ubuff , strlen(Ubuff) + 1);

    close(fd);  
    return 0;
}

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

    /* Read the data back from the device */
    memset(Kbuff , 0 ,sizeof(Kbuff));
    read(fd , Kbuff , sizeof(Kbuff));
    printf("Data from kernel : %s\n", Kbuff);

    close(fd);
    return 0;
}

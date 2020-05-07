#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#define DEVICE_NAME "Dev_frame0"
 
int main()
{
        int fd,num;
        fd = open(DEVICE_NAME, O_RDWR, S_IRUSR|S_IWUSR);
        if(fd==-1)
        {
                printf("%s device open failure.\n",DEVICE_NAME);
                return -1;
        }
        printf("Reading data.\n");
        read(fd,&num,sizeof(int));
        printf("Writing data.\n");
        write(fd, &num, sizeof(int));
        printf("Control data input.\n");
        ioctl(fd,5);
        printf("Device closed.\n");
        close(fd);
        return 0;
}      

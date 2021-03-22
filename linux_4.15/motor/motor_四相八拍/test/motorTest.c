#include <stdio.h>
#include <stdlib.h>
#include <linux/types.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>


/* ioctl cmd */
#define MOTOR_STOP		0x1
#define MOTOR_RESET		0x2
#define MOTOR_MOVE		0x3
#define MOTOR_GET_STATUS	0x4
#define MOTOR_SPEED		0x5
#define MOTOR_GOBACK	0x6
#define MOTOR_CRUISE	0x7

/* motor speed */
#define MOTOR_MAX_SPEED	900		/**< unit: beats per second */
#define MOTOR_MIN_SPEED	100

/////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	int motor_fd;
    int timer_cnt = 0, timer_call = 0, timer_fulse_delay = 0;	
    int start = 0;
    if (argc < 4) {
        printf("please input data\n");
        printf("   1:timer_cnt\n"
               "   2:timer_call\n"
               "   3:timer_fulse_delay\n"
               "   3:timer_fulse_delay\n");
        return -1;
    }
    timer_cnt  = atoi(argv[1]);
    timer_call  = atoi(argv[2]);
    timer_fulse_delay  = atoi(argv[3]);
    start  = atoi(argv[4]);
    printf("timer_cnt:%d, timer_call:%d, timer_fulse_delay:%d\n", timer_cnt, timer_call, timer_fulse_delay);
	motor_fd = open("/dev/motor_dev", O_RDWR | O_SYNC);
	if (motor_fd == -1) {
		perror("open /dev/motor");
		return -3;
	}
	ioctl(motor_fd, 3, timer_fulse_delay);
	ioctl(motor_fd, 2, timer_call);
	ioctl(motor_fd, 1, timer_cnt);
	ioctl(motor_fd, 4, start);//rotate_direction
    while (1) {
        sleep(1);
    };
	close(motor_fd);
	motor_fd = -1;
	return 0;
}

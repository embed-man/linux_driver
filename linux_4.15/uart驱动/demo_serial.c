#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <log/log.h>
#include <stdlib.h>

#define UART_DEVICE     "/dev/ttySLB1"

int main(void)
{
	int count, i, fd;
	struct termios oldtio, newtio;

	//-----------打开uart设备文件------------------
	fd = open(UART_DEVICE, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		printf("Open %s failed\n", UART_DEVICE);
		return -1;
	} else {
		printf("Open %s successfully\n", UART_DEVICE);
	}

	//-----------设置操作参数-----------------------
	tcgetattr(fd, &oldtio);//获取当前操作模式参数
	memset(&newtio, 0, sizeof(newtio));

	//波特率=230400 数据位=8 使能数据接收
	newtio.c_cflag = B230400 | CS8 | CLOCAL | CREAD | CSTOPB;
	newtio.c_iflag = IGNPAR;

	tcflush(fd, TCIFLUSH);//清空输入缓冲区和输出缓冲区
	tcsetattr(fd, TCSANOW, &newtio);//设置新的操作参数

	for (i = 0; i < 9; i++)
		printf("%#X ", cmd_buf1[i]);

	count = write(fd, buf, 9);
	if (count != 9) {
		printf("send failed\n");
		return -1;
	}

	usleep(500000);

	memset(read_buf, 0, sizeof(read_buf));

	close(fd);
	tcsetattr(fd, TCSANOW, &oldtio); //恢复原先的设置

	return 0;
}

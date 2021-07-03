/*************************************************************************
    > File Name: uart.c
    > Author: xzy
    > Mail: 
    > Created Time: 2021年06月16日 星期三 14时45分41秒
 ************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <termios.h>
#include "uart.h"
#include "ipc_inc.h"

#define SERIAL_DEV_NAME "/dev/ttyS0"

#define max(a,b) ((a) > (b) ? (a) : (b))
#define D printf


/*************************************************************************
* function name: uart_write
* return: int
* parameters: 
*   #1: char *data
*   #2: int data_len
*   该接口只发送命令格式:
*
*   CMD[1BYTE] + DATA[NBYTE]
****************************************************************************/
int uart_write(char *data, int data_len)
{
    int len = 0, i;
    int send_len = 0;
    int frame_len = 0;
    int left_len = 0;
    unsigned char send_buf[1024] = {0x00};
    if (data == NULL) {
        return -1;
    }

    left_len = frame_len;

    for (i = 0; i < frame_len; i++) {
        D(" 0x%02X ", send_buf[i]);
    }
    D("\n");	
    while (left_len > 0) {
        len = write(fd, (unsigned char*)(send_buf + send_len), left_len);
        
        if (len == 0) {
        	perror("uart_write error");
            return -1;
        }
        send_len += len;
        left_len = frame_len - send_len;
    }
    //tcflush(serial_ctx.radar_fd, TCIOFLUSH);
    return 0;
}

/*************************************************************************
* function name: parse_radar_protocol
* return: int
* parameters: 
*   #1: void *serial_buf
*   #2: int serial_buf_len
*   serial_buf的数据格式为:
*   起始码 数据长度 功能码 地址码 1 地址码 2 数据 校验码
*
*   0X55 Lenth_L Lenth_H Command Address_1 Address_2 Data Crc16_L Crc16_H
*
*   1Byte 1Byte 1Byte 1Byte 1Byte 1Byte nByte 1Byte 1Byte
*
*   0x55   0x0B  0x00  0x04  0x03  0x06  0x00  0x00  0xC8  0x42  0x0A  0x64 
****************************************************************************/
int parse_radar_protocol(void *serial_buf, int serial_buf_len)
{
    if (serial_buf == NULL) {
        return -1;
    }
    int i = 0;
    int frame_len = 0;
    unsigned char *buf = (unsigned char*)serial_buf;
#if 1
    for (;i < serial_buf_len; i++) {
        D(" 0x%02X ", buf[i]);
    }
    D("\n");
#endif 
    //fflush(stdout);

    return 0;
}

int process_data()
{
    int ret;
    int f_fd = -1,read_len = 0,curren_len = 0;
    char read_buf[1025] = {0};
    int i = 0;
    unsigned short serial_crc = 0;
    unsigned short calculate_crc = 0;

    ret = read(serial_ctx.radar_fd, read_buf, 1024);
    if (ret <= 0) {
        perror("read error");
        //tcflush(fd,TCIOFLUSH);
        return -1;
    }
	D(" \n");
    //write(fd, read_buf, strlen(read_buf));
    return ret;
}

int baud_rate[] = {B115200,B57600,B38400,B19200,B9600,B4800,B2400,B1200,B300};
int baud_rate_num[] = {115200,57600,38400,19200,9600,4800,2400,1200,300};

#define CSTOPB 0000100
/* c_cflag bit meaning */
#define PARENB 0000400
#define PARODD 0001000
/* c_iflag bits */
#define INPCK 0000020

#define CSIZE 0000060
#define CS5 0000000
#define CS6 0000020
#define CS7 0000040
#define CS8 0000060

int uart_config(int fd, int baudrate,int databits,int stopbits,char parity)
{
    int i = 0;
    struct termios opt;
    /*
     * 清空串口接收缓冲区
     * Clear the serial port receive buffer
     * */
    tcflush(fd, TCIOFLUSH);
    if (tcgetattr(fd, &opt)) {
        perror("tcsetattr fail\n");
        return -1;
    }
    /**
     * 设置串口波特率
     * Set the serial port baud rate
     * */
    for(i = 0; i < sizeof(baud_rate)/sizeof(int) ;i++) {
        if(baud_rate_num[i] == baudrate) {
            cfsetospeed(&opt, baud_rate[i]);
            cfsetispeed(&opt, baud_rate[i]);
        }
    }
    /*设置控制模式*/
    opt.c_cflag |= CLOCAL;//保证程序不占用串口
    opt.c_cflag |= CREAD;//保证程序可以从串口中读取数据

    opt.c_cflag &= ~CRTSCTS;

     /* 设置数据位数,先清除CSIZE数据位的内容
     * Set the number of data bits, first clear the content of the CSIZE data bit
     * */    
    opt.c_cflag &= ~CSIZE;
    switch (databits) {
        case 5:
            opt.c_cflag |= CS5;
            break;
        case 6:
            opt.c_cflag |= CS6;
            break;
        case 7:
            opt.c_cflag |= CS7;
            break;
        case 8:
            opt.c_cflag |= CS8;
            break;
        default:
            break;
    }
    /* 
     * 设置停止位
     * Set stop bit
     **/
    switch (stopbits) {
        case 1:
            opt.c_cflag &= ~CSTOPB;//1位 1 bit
            break;
        case 2:
            opt.c_cflag |= CSTOPB;//2位 2 bit
            break;
        default:
            break;
    }
    /*
     * 设置校验位
     * Set check digit
     * */
    switch (parity) {
        case 'n': 
        case 'N':
            opt.c_cflag &= ~PARENB; // 不使用奇偶校验 
            opt.c_iflag &= ~INPCK;  // 禁止输入奇偶检测
            break;
        case 'o':
        case 'O':
            opt.c_cflag |= PARENB;  // 启用奇偶效验 
            opt.c_iflag |= INPCK;   // 启用输入奇偶检测 
            opt.c_cflag |= PARODD ; // 设置为奇效验 
            break;
        case 'e':
        case 'E':
            opt.c_cflag |= PARENB;  // 启用奇偶效验
            opt.c_iflag |= INPCK;   // 启用输入奇偶检测
            opt.c_cflag &= ~PARODD; // 设置为偶效验
            break;
        default:
            break;
    }
 
    /*设置输出模式为原始输出*/
    opt.c_oflag &= ~OPOST;//OPOST：若设置则按定义的输出处理，否则所有c_oflag失效
 
    /*设置本地模式为原始模式*/
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    /*
     *ICANON：允许规范模式进行输入处理
     *ECHO：允许输入字符的本地回显
     *ECHOE：在接收EPASE时执行Backspace,Space,Backspace组合
     *ISIG：允许信号
     */
 
    /*设置等待时间和最小接受字符*/
    opt.c_cc[VTIME] = 0;//可以在select中设置
    opt.c_cc[VMIN] = 1;//最少读取一个字符
 
    /*如果发生数据溢出，只接受数据，但是不进行读操作*/
    tcflush(fd,TCIFLUSH);
 
    /*激活配置*/
    if(tcsetattr(fd,TCSANOW,&opt) < 0) {
        perror("tcsetattr failed");
        return -1;
    }

    return 0;
}

void *loop_recv_radar(void *arg)
{
    int32_t idx, ret, max_fd;
    struct timeval tt;
    fd_set rfds;
    printf("start thread\n");
    
    SET_THREAD_NAME("uart0_recv_radar");
    while (1){
		tt.tv_sec = 0;
   		tt.tv_usec = 10 * 1000;
        max_fd = -1;
        FD_ZERO(&rfds);

        if (serial_ctx.radar_fd < 0){
        	serial_ctx.radar_fd = open(SERIAL_DEV_NAME, O_RDWR | O_NONBLOCK);
            if (serial_ctx.radar_fd < 0) {
                perror("open serial dev  fail");
                return NULL;
            }
#if defined(CLOUD_PLATFORM_MOTOR)
            if (uart_config(serial_ctx.radar_fd, 115200, 8, 1, 'N')) {
#else
            if (uart_config(serial_ctx.radar_fd, 9600, 8, 1, 'N')) {
#endif
                fprintf(stderr,"uart set failed!\n");
                exit(-1);
            }
        } else {
        	FD_SET(serial_ctx.radar_fd, &rfds);
            max_fd = max(max_fd, serial_ctx.radar_fd);
        }
        ret = select(max_fd + 1, &rfds, NULL, NULL, &tt);
        if (ret < 0 && errno == EINTR) // -1 clear all fds
            continue;
        if (ret <= 0) //0 timeout
            continue;
            
        if (serial_ctx.radar_fd >= 0 && FD_ISSET(serial_ctx.radar_fd, &rfds)) {
        #if defined(CLOUD_PLATFORM_MOTOR)
        	process_data_cloud_machine();
        #else
            process_data();
        #endif
        }
    }
   printf("end thread\n");
}
int uart_init()
{
    static pthread_t id;
    if (pthread_create(&id, NULL, loop_recv_radar, NULL) < 0) {
        perror("pthread create uart0 fail");
        return -1;
    }
    return 0;
}
#if 0
int main(int argv,char **argc)
{
    pthread_t id;
    pthread_create(&id, NULL, loop_recv_radar, NULL);
    while(1){
        usleep(100 * 1000);
    }
    return 0;
}
#endif

#include <unistd.h>
#include <fcntl.h>

#include <termios.h>
#include <sys/ioctl.h>

#include "platform.h"

// 获取波特率
static speed_t getBbaud(int baud){
    switch(baud){
        case 50:
            return B50;
        case 75:
            return B75;
        case 110:
            return B110;
        case 134:
            return B134;
        case 150:
            return B150;
        case 200:
            return B200;
        case 300:
            return B300;
        case 600:
            return B600;
        case 1200:
            return B1200;
        case 1800:
            return B1800;
        case 2400:
            return B2400;
        case 4800:
            return B4800;
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        case 230400:
            return B230400;
        case 460800:
            return B460800;
        case 500000:
            return B500000;
        case 576000:
            return B576000;
        case 921600:
            return B921600;
        case 1000000:
            return B1000000;
        case 1152000:
            return B1152000;
        case 1500000:
            return B1500000;
        case 2000000:
            return B2000000;
        case 2500000:
            return B2500000;
        case 3000000:
            return B3000000;
        case 3500000:
            return B3500000;
        case 4000000:
            return B4000000;
        default:
            return B115200;
    }
}

// isp初始化
int platform_ispinit(char *port, int baud){
    int fd = open(port, O_RDWR|O_NOCTTY);
    struct termios opt;
    speed_t Bbaud = getBbaud(baud);
    if(fd < 0)
        return fd;
    if(tcgetattr(fd, &opt) < 0)
        return -1;
    /* Set baud and timeout */
    if(cfsetispeed(&opt, Bbaud) < 0)
        return -1;
    if(cfsetospeed(&opt, Bbaud) < 0)
        return -1;
    opt.c_cc[VTIME] = 5;            // 超时 = 5×100ms
    opt.c_cc[VMIN] = 0;
    opt.c_cflag |= CLOCAL;          // 启动接收
    opt.c_cflag |= CREAD;           // 忽略调制调解器状态行
    /* Databits setting section */
    opt.c_cflag &= ~CSIZE;
    opt.c_cflag |= CS8;             // 数据位 8
    /* Parity setting section */
    opt.c_cflag |= PARENB;          // 奇偶校验使能
    opt.c_cflag &= ~PARODD;         // 偶校验
    opt.c_cflag |= INPCK;           // 使奇偶校验起作用
    opt.c_cflag |= ISTRIP;          // 若设置则有效输入数字被剥离7个字节，否则保留全部8位
    /* Stopbits setting section */
    opt.c_cflag &= ~CSTOPB;         // 停止位 1
    /* Flowcontrol setting section */
    opt.c_cflag &= ~CRTSCTS;
    opt.c_iflag &= ~(IXON | IXOFF | IXANY);
    /* Using raw data mode */
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    opt.c_iflag &= ~(INLCR | IGNCR | ICRNL);
    opt.c_oflag &= ~(ONLCR | OCRNL);
    if(tcsetattr(fd, TCSANOW, &opt) < 0)
        return -1;
    if(tcflush(fd, TCIOFLUSH) < 0)
        return -1;
    return fd;
}

int platform_ispclose(int fd){
    return close(fd);
}

ssize_t platform_ispread(int fd, void *buf, size_t count){
    return read(fd, buf, count);
}

ssize_t platform_ispwrite(int fd,void *buf,size_t count){
    return write(fd, buf, count);
}

// 硬件复位
void platform_hardreset(int fd){
    #define RTS_1 status|=TIOCM_RTS
    #define DTR_1 status|=TIOCM_DTR
    #define RTS_0 status&=~TIOCM_RTS
    #define DTR_0 status&=~TIOCM_DTR
    int status;
    usleep(300000);
    ioctl(fd, TIOCMSET, &status);
    RTS_1;
    DTR_0;
    ioctl(fd, TIOCMSET, &status);
    usleep(500000);
    RTS_0;
    DTR_0;
    ioctl(fd, TIOCMSET, &status);
    #undef RTS_1
    #undef DTR_1
    #undef RTS_0
    #undef DTR_0
}

// 硬件复位至boot模式
void platform_hardreset2boot(int fd){
    #define RTS_1 status|=TIOCM_RTS
    #define DTR_1 status|=TIOCM_DTR
    #define RTS_0 status&=~TIOCM_RTS
    #define DTR_0 status&=~TIOCM_DTR
    int status;
    ioctl(fd, TIOCMSET, &status);
    RTS_0;
    DTR_0;
    ioctl(fd, TIOCMSET, &status);
    usleep(50000);
    RTS_0;
    DTR_1;
    ioctl(fd, TIOCMSET, &status);
    usleep(20000);
    RTS_1;
    DTR_0;
    ioctl(fd, TIOCMSET, &status);
    usleep(10000);
    RTS_0;
    DTR_1;
    ioctl(fd, TIOCMSET, &status);
    usleep(5000);
    RTS_0;
    DTR_0;
    ioctl(fd, TIOCMSET, &status);
    #undef RTS_1
    #undef DTR_1
    #undef RTS_0
    #undef DTR_0
}
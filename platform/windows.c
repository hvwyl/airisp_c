#include <unistd.h>
#include <fcntl.h>

#include <errno.h>

#include "PComm.h"

#include "platform.h"

// 获取波特率
static int getBbaud(int baud){
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
        case 7200:
            return B7200;
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
        case 921600:
            return B921600;
        default:
            return B115200;
    }
}

// isp初始化
int platform_ispinit(char *port, int baud){
    int portnum;
    if(!(port[0] == 'C' && port[1] == 'O' && port[2] =='M' && strlen(port)!=3)){
        errno = EINVAL;
        return -1;
    }
    portnum = atoi(port+3);
    if(sio_open(portnum) != 0){
        errno = EIO;
        return -1;
    }
    if(sio_ioctl(portnum, getBbaud(baud), P_EVEN|STOP_1|BIT_8) != 0){
        errno = EIO;
        return -1;
    }
    sio_SetReadTimeouts(portnum, 20, 500);
    return portnum;
}

int platform_ispclose(int fd){
    if(sio_close(fd)!=0)
        return -1;
    else
        return 0;
}

ssize_t platform_ispread(int fd, void *buf, size_t count){
    int r = sio_read(fd, buf, count);
    if(r == 0){
        errno = EIO;
        return -1;
    }else
        return r;
}

ssize_t platform_ispwrite(int fd,void *buf,size_t count){
    int r = sio_write(fd, buf, count);
    if(r == 0){
        errno = EIO;
        return -1;
    }else
        return r;
}

// 硬件复位
void platform_hardreset(int fd){
    sio_RTS(fd, 1);
    sio_DTR(fd, 0);
    usleep(500000);
    sio_RTS(fd, 0);
    sio_DTR(fd, 0);
}

// 硬件复位至boot模式
void platform_hardreset2boot(int fd){
    sio_RTS(fd, 0);
    sio_DTR(fd, 0);
    usleep(50000);
    sio_RTS(fd, 0);
    sio_DTR(fd, 1);
    usleep(20000);
    sio_RTS(fd, 1);
    sio_DTR(fd, 0);
    usleep(10000);
    sio_RTS(fd, 0);
    sio_DTR(fd, 1);
    usleep(5000);
    sio_RTS(fd, 0);
    sio_DTR(fd, 0);
}

#ifndef __PLATFORM_H
#define __PLATFORM_H

#include <unistd.h>

int platform_ispinit(char *port, int baud);
int platform_ispclose(int fd);
ssize_t platform_ispread(int fd, void *buf, size_t count);
ssize_t platform_ispwrite(int fd,void *buf,size_t count);
void platform_hardreset(int fd);
void platform_hardreset2boot(int fd);


#endif
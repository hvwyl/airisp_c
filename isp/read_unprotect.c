#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform.h"
#include "isp.h"

void isp_read_unprotect(struct cmdOption *opt){
    int fd;

    if(opt->port == NULL){
        puts("串口号为空");
        exit(-1);
    }

    fd = platform_ispinit(opt->port, opt->baud);
    if(fd < 0){
        perror("打开串口失败");
        exit(-1);
    }
    printf("串口打开成功: %s\n", opt->port);

    if(isp_utils_reset_into_bootloader(fd, opt->connect_attempts) < 0){
        perror("芯片连接失败");
        exit(-1);
    };
    printf("芯片连接成功\n");

    printf("Start turn off read protection...\nthis will erase all content on your flash !!!\n");
    if(isp_utils_readunprotect(fd) < 0){
        perror("error in turn off read protection");
        exit(-1);
    }
    printf("Success in turn off read protection\n\n");
    
    platform_hardreset(fd);
    platform_ispclose(fd);
}
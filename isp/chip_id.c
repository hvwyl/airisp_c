#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform.h"
#include "isp.h"
#include "ispdef.h"

void isp_chip_id(struct cmdOption *opt){
    int fd;

    uint8_t odata[2] = {GetID, (uint8_t)~GetID};
    uint8_t idata[128] = {0};
    int rlen, i, idlen;

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

    if(platform_ispwrite(fd, odata, 2) < 0){
        perror("写命令失败");
        exit(-1);
    }
    rlen = platform_ispread(fd, idata, 128);
    if(rlen < 0 || rlen < 2 || idata[0] == NACK){
        perror("读取失败");
        exit(-1);
    }

    printf("ID读取成功: ");
    idlen = 1+(int)idata[1];
    for(i=0; i<idlen; i++)
        printf("0x%.2X ", idata[i+2]);
    printf("\n\n");

    platform_hardreset(fd);
    platform_ispclose(fd);
}
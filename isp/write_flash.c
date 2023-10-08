#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "platform.h"
#include "isp.h"

void isp_write_flash(struct cmdOption *opt){
    int fd = -1;
    uint32_t addr = 0;

    size_t file_size = 0;
    uint8_t *file = NULL;

    char *filename = NULL;

    // 传入参数验证
    if((opt->argc - opt->optind) != 1){
        errno = EINVAL;
        perror("");
        exit(-1);
    }
    filename = opt->argv[opt->optind];
    if(strcmp(filename+strlen(filename)-4, ".bin") == 0){
        if(opt->address_pointer == NULL){
            addr = 0x08000000;
        }else{
            if(strlen(opt->address_pointer)<2 || opt->address_pointer[0]!='0' || opt->address_pointer[1]!='x'){
                errno = EINVAL;
                perror("");
                exit(-1);
            }
            addr = isp_utils_hextoi(opt->address_pointer);
        }
        file = isp_utils_binreader(filename, &file_size);
        if(file == NULL){
            perror("");
            exit(-1);
        }
    }else{
        puts("错误的文件类型");
        exit(-1);
    }
    if(opt->port == NULL){
        puts("串口号为空");
        exit(-1);
    }

    // 开始烧录
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
    if(opt->erase_all){
        printf("开始擦除Flash...\n");
        if(isp_utils_earseflash(fd) < 0){
            if(isp_utils_readunprotect(fd) < 0){
                perror("擦除Flash失败");
                exit(-1);
            }
            if(isp_utils_earseflash(fd) < 0){
                perror("擦除Flash失败");
                exit(-1);
            }
        };
        printf("擦除Flash成功\n");
    }
    printf("开始烧录...\n");
    if(isp_utils_program(fd, addr, file ,file_size) < 0){
        perror("芯片烧录失败");
        exit(-1);
    }
    printf("芯片烧录成功\n");
    
    platform_hardreset(fd);
    platform_ispclose(fd);
}
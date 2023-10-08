#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>

#include "ispdef.h"
#include "platform.h"

static int connect_bootloader(int fd){
    uint8_t odata[1] = {0x7F};
    uint8_t idata[128] = {0};
    int i;
    platform_ispwrite(fd, odata, 1);
    platform_ispread(fd, idata, 8);
    for(i=0; i<128; i++){
        if(idata[i] == ACK)return 0;
    };
    return -1;
}

// 16进制输入转uint32_t
uint32_t isp_utils_hextoi(char *str){
    char *endptr;
    uint32_t num;
    num = strtol(str, &endptr, 16);
    if(*endptr != '\0') {
        errno = EINVAL;
        perror("");
        exit(-1);
    }
    return num;
}

// 重启至Bootloader
int isp_utils_reset_into_bootloader(int fd, int connect_attempts){
    int i=0;
    for(i=0; i<connect_attempts || connect_attempts<=0; i++){
        platform_hardreset2boot(fd);
        usleep(10000);
        if(connect_bootloader(fd) == 0)
            return 0;
        usleep(100000);
    }
    errno = ETIMEDOUT;
    return -1;
}

// isp写入数据(带有ack)
int isp_utils_writebytes(int fd, uint8_t *odata, size_t size){
    uint8_t idata[128] = {0};
    int i;
    if(platform_ispwrite(fd, odata, size) < 0)
        return -1;
    if(platform_ispread(fd, idata, 128) < 0)
        return -1;
    for(i=0; i<128; i++){
        if(idata[i] == ACK)return 0;
    };
    errno = EIO;
    return -1;
}

// 去除readprotect
int isp_utils_readunprotect(int fd){
    uint8_t odata[2] = {ReadUnprotect, (uint8_t)~ReadUnprotect};
    return isp_utils_writebytes(fd, odata, 2);
}

// 设置readprotect
int isp_utils_readprotect(int fd){
    uint8_t odata[2] = {ReadProtect, (uint8_t)~ReadProtect};
    return isp_utils_writebytes(fd, odata, 2);
}

// 擦除flash
int isp_utils_earseflash(int fd){
    uint8_t odata1[2] = {ExtendedErase, (uint8_t)~ExtendedErase};
    uint8_t odata2[3] = {0xFF, 0xFF, 0x00};
    int r;
    r = isp_utils_writebytes(fd, odata1, 2);
    if(r < 0){
        errno = EIO;
        return -1;
    }
    r = isp_utils_writebytes(fd, odata2, 3);
    if(r < 0){
        errno = EIO;
        return -1;
    }
    return 0;
}

// 读取bin文件
uint8_t * isp_utils_binreader(char *file, size_t *sizep){
    FILE *fp;
    size_t size;
    uint8_t *buffer = NULL;
    fp = fopen(file, "rb");
    if(fp == NULL)
        return NULL;
    if(fseek(fp, 0 , SEEK_END) != 0){
        fclose(fp);
        return NULL;
    }
    size = ftell(fp);
    if(size < 0){
        fclose(fp);
        return NULL;
    }
    if(fseek(fp, 0 , SEEK_SET) != 0){
        fclose(fp);
        return NULL;
    }
    *sizep = size;
    buffer = (uint8_t *)malloc(size*sizeof(uint8_t));
    if(fread(buffer, 1, size, fp) == 0){
        fclose(fp);
        free(buffer);
        return NULL;
    }
    fclose(fp);
    return buffer;
}

// 烧录flash
int isp_utils_program(int fd, uint32_t addr, uint8_t *file, size_t filesize){
    size_t i, j;
    
    uint32_t addr_now;

    uint8_t *program_bytes;
    size_t program_size;

    uint8_t command[2] = {WriteMemory, (uint8_t)~WriteMemory};
    uint8_t addr_bytes[5] = {0};
    uint8_t data_bytes[130] = {0};

    // 128字节对齐
    if(filesize%128 != 0)
        program_size = filesize + 128 -filesize%128;
    else
        program_size = filesize;

    program_bytes = (uint8_t *)malloc(program_size*sizeof(uint8_t));
    memset(program_bytes, 0xFF, program_size);
    memcpy(program_bytes, file, filesize);
    free(file);

    // 开始烧录
    for(i=0; i<program_size; i+=128){
        addr_now = addr + i;
        addr_bytes[3] = (uint8_t)(addr_now&0xFF);
        addr_bytes[2] = (uint8_t)((addr_now>>8)&0xFF);
        addr_bytes[1] = (uint8_t)((addr_now>>16)&0xFF);
        addr_bytes[0] = (uint8_t)((addr_now>>24)&0xFF);
        addr_bytes[4] = (uint8_t)(addr_bytes[0]^addr_bytes[1]^addr_bytes[2]^addr_bytes[3]);
        if(isp_utils_writebytes(fd, command, 2) < 0)
            return -1;
        if(isp_utils_writebytes(fd, addr_bytes, 5) < 0)
            return -1;
        data_bytes[0] = 0x7F;
        data_bytes[129] = 0x7F;
        for(j=0; j<128; j++){
            data_bytes[j+1] = program_bytes[i+j];
            data_bytes[129] ^= program_bytes[i+j];
        }
        if(isp_utils_writebytes(fd, data_bytes, 130) < 0)
            return -1;
        printf("写地址: 0x%.8X 成功！\n", addr_now);
    }
    free(program_bytes);
    return 0;
}
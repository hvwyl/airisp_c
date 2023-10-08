#ifndef __ISP_H
#define __ISP_H

#include "main.h"

// utils
uint32_t isp_utils_hextoi(char *str);
int isp_utils_reset_into_bootloader(int fd, int connect_attempts);
int isp_utils_writebytes(int fd, uint8_t *odata, size_t size);
int isp_utils_readunprotect(int fd);
int isp_utils_readprotect(int fd);
int isp_utils_earseflash(int fd);
uint8_t * isp_utils_binreader(char *file, size_t *sizep);
int isp_utils_program(int fd, uint32_t addr, uint8_t *file, size_t filesize);

// isp routes
void isp_chip_id(struct cmdOption *opt);
void isp_write_flash(struct cmdOption *opt);
void isp_read_unprotect(struct cmdOption *opt);
void isp_read_protect(struct cmdOption *opt);


#endif
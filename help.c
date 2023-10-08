#include <stdio.h>

#include "help.h"

static char *help_root_str = "Description:\n"
"  AirISP 是一个flash烧录工具\n"
"\n"
"Usage:\n"
"  AirISP [command] [options]\n"
"\n"
"Options:\n"
"  -c, --chip <chip>                      目标芯片型号，auto/air001\n"
"  -p, --port <port>                      串口名称\n"
"  -b, --baud <baud>                      串口波特率\n"
"  --connect-attempts <connect-attempts>  最大重试次数，小于等于0表示无限次 (默认为10次)\n"
"  -h, --help                             Show help and usage information\n"
"\n"
"Commands:\n"
"  chip_id                                获取芯片ID\n"
"  write_flash <filename>                 向flash刷入固件\n"
"  read_unprotect                         关闭读保护\n"
"  read_protect                           开启读保护\n"
"\n"
"Run 'AirISP COMMAND --help' or 'AirISP COMMAND -h' for more information on a command.\n";

static char *help_chip_id_str = "Description:\n"
"  获取芯片ID\n"
"\n"
"Usage:\n"
"  AirISP chip_id [options]\n"
"\n"
"Options:\n"
"  -h, --help  Show help and usage information\n";
 
static char *help_write_flash_str = "Description:\n"
"  向flash刷入固件\n"
"\n"
"Usage:\n"
"  AirISP write_flash <filename> [options]\n"
"\n"
"Arguments:\n"
"  <filename>         待烧录的文件名\n"
"\n"
"Options:\n"
"  -a, --address      烧录地址 (默认为0x08000000)\n"
"  -e, --erase-all    全片擦除 (默认只擦除待写入的页)\n"
"  -h, --help         Show help and usage information\n";

static char *help_read_unprotect_str = "Description:\n"
"  关闭读保护\n"
"\n"
"Usage:\n"
"  AirISP read_unprotect [options]\n"
"\n"
"Options:\n"
"  -h, --help  Show help and usage information\n";

static char *help_read_protect_str = "Description:\n"
"  开启读保护\n"
"\n"
"Usage:\n"
"  AirISP read_protect [options]\n"
"\n"
"Options:\n"
"  -h, --help  Show help and usage information\n";

void printHelp(enum helpcode helpcode){
    switch(helpcode){
        #define HELP_HANDLER(x) case HELPCODE_##x:puts(help_##x##_str);break;
        HELP_HANDLER(root)
        HELP_HANDLER(chip_id)
        HELP_HANDLER(write_flash)
        HELP_HANDLER(read_unprotect)
        HELP_HANDLER(read_protect)
        #undef HELP_HANDLER
        default:
            break;
    }
}
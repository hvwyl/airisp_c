#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "cmdparser.h"
#include "help.h"
#include "isp.h"

struct cmdOption opt = {
    .chip = chip_auto,
    .port = NULL,
    .baud = 115200,
    .connect_attempts = 10,
    // for write_flash
    .address_pointer = NULL,
    .erase_all = false
};
char *command = NULL;

int main(int argc, char *argv[]){
    int optind;
    if(parserHelp(argc, argv, &command) == -1){
        if(command == NULL)
            printHelp(HELPCODE_root);
        #define HELP_ROUTER(x) else if(strcmp(command, #x) == 0)printHelp(HELPCODE_##x);
        HELP_ROUTER(chip_id)
        HELP_ROUTER(write_flash)
        HELP_ROUTER(read_unprotect)
        HELP_ROUTER(read_protect)
        #undef HELP_ROUTER
        else
            printHelp(HELPCODE_root);
        return 0;
    }
    optind = parserVar(argc, argv, &opt, &command);
    opt.argc = argc;
    opt.optind = optind;
    opt.argv = argv;
    if(command == NULL)
        printHelp(HELPCODE_root);
    #define CMD_ROUTER(x) else if(strcmp(command, #x) == 0)isp_##x(&opt);
    CMD_ROUTER(chip_id)
    CMD_ROUTER(write_flash)
    CMD_ROUTER(read_unprotect)
    CMD_ROUTER(read_protect)
    #undef CMD_ROUTER
    else
        printHelp(HELPCODE_root);
    return 0;
}
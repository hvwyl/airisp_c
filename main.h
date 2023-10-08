#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>
#include <stdbool.h>

enum chip {
    chip_auto = 0,
    chip_air001
};

struct cmdOption {
    // required parameters
    enum chip chip;
    char *port;
    int baud;
    int connect_attempts;
    // optional parameters
    char *address_pointer;
    bool erase_all;

    // for arguments
    int argc;
    int optind;
    char **argv;
};

#endif
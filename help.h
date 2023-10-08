#ifndef __HELP_H
#define __HELP_H

enum helpcode {
    HELPCODE_root = 0,
    HELPCODE_chip_id,
    HELPCODE_write_flash,
    HELPCODE_read_unprotect,
    HELPCODE_read_protect
};

void printHelp(enum helpcode);

#endif
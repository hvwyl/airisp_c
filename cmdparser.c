#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <getopt.h>
#include <errno.h>

#include "main.h"
#include "cmdparser.h"
#include "help.h"

extern int opterr, optind;

static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    // required parameters
    {"chip", required_argument, 0, 'c'},
    {"port", required_argument, 0, 'p'},
    {"baud", required_argument, 0, 'b'},
    {"connect-attempts", required_argument, 0, 0},
    // optional parameters
    {"address", required_argument, 0, 'a'},
    {"erase-all", no_argument, 0, 'e'},
    {0, 0, 0, 0}
};

int parserHelp(int argc, char *argv[], char **command){
    int c;
    int option_index = 0;
    for(c=0; c<argc; c++){
        if(strcmp(argv[c],"-h")==0 || strcmp(argv[c],"--help")==0){
            c = -1;
            break;
        }
    }
    if(c < 0){
        opterr = 0;
        while(getopt_long(argc, argv, "hc:p:b:a:e", long_options, &option_index) != -1);
        if(optind < argc)
            *command = argv[optind];
    }
    return c;
}

int parserVar(int argc, char *argv[], struct cmdOption *optp, char **command){
    int c;
    int option_index = 0;
    opterr = 1;
    while(1){
        c = getopt_long(argc, argv, "hc:p:b:a:e", long_options, &option_index);
        if(c == -1)
            break;
        switch (c) {
            case 0:
                if(strcmp(long_options[option_index].name, "connect-attempts") == 0){
                    if(optarg)
                        optp->connect_attempts = atoi(optarg);
                };
                break;
            // required parameters
            case 'c':
                if(optarg){
                    if(strcmp(optarg,"auto") == 0)
                        optp->chip = chip_auto;
                    else if(strcmp(optarg,"air001") == 0)
                        optp->chip = chip_air001;
                    else{
                        errno = EINVAL;
                        perror("错误的芯片型号");
                        exit(-1);
                    }
                };
                break;
            case 'p':
                if(optarg)
                    optp->port = optarg;
                break;
            case 'b':
                if(optarg)
                    optp->baud = atoi(optarg);
                if(optp->baud == 0)
                    optp->baud = 115200;
                break;
            // optional parameters
            case 'a':
                if(optarg)
                    optp->address_pointer = optarg;
                break;
            case 'e':
                optp->erase_all = true;
                break;
            case '?':
                exit(-1);
                break;
            default:
                printf("?? getopt returned character code 0%o ??\n", c);
        }
    }
    if(optind < argc)
        *command = argv[optind];
    return optind + 1;
}
#ifndef __CMDPARSER_H
#define __CMDPARSER_H

int parserHelp(int argc, char *argv[], char **command);
int parserVar(int argc, char *argv[], struct cmdOption *optp, char **command);

#endif
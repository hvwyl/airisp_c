#ifndef __ISPDEF_H
#define __ISPDEF_H

// #define Get             0x00
// #define GetVersion      0x01
#define GetID           0x02
// #define GetDeviceID     0x03
// #define ReadMemory      0x11
// #define Go              0x21
#define WriteMemory     0x31
#define ExtendedErase   0x44
// #define WriteProtect    0x63
// #define WriteUnrotect   0x73
#define ReadProtect     0x82
#define ReadUnprotect   0x92

#define ACK             0x79
#define NACK            0x1F

#endif
 
#ifndef __VIRTUAL_DEVICE_H_
#define __VIRTUAL_DEVICE_H_

typedef unsigned char *(*FUNCTION1)(int);
typedef void(*FUNCTION0)(void);

typedef struct __DATA_DEVICE
{
    char *name;
    int uniqueID;
    FUNCTION1 read_sector;
    void (*write_sector)(unsigned char *Buffer, unsigned int Length, unsigned int sector);
    FUNCTION0 reset;
    FUNCTION1 init;
    FUNCTION0 deinit;
    FUNCTION1 set_drive;
} DATADEVICE;

bool vd_populate();
DATADEVICE vd_getCurrentDataDevice();
void vd_setCurrentDataDevice(DATADEVICE d);
DATADEVICE vd_getDataDeviceById(int id);

#endif
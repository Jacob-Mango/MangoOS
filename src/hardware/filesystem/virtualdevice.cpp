#include <hardware/filesystem/virtualdevice.h>
#include <hardware/filesystem/harddrive.h>

DATADEVICE hdd;

static DATADEVICE byid[4];
static DATADEVICE _c;

bool vd_populate()
{

    hdd.init = (FUNCTION1)hdd_init;
    hdd.deinit = (FUNCTION0)0;
    hdd.name = (char *)"HARDDISKDRIVER";
    hdd.read_sector = (FUNCTION1)hdd_read_sector;
    hdd.reset = (FUNCTION0)0;
    hdd.set_drive = (FUNCTION1)hdd_set_working_drive;
    hdd.uniqueID = 1;
    hdd.init(0);
    byid[hdd.uniqueID] = hdd;

    vd_setCurrentDataDevice(hdd);

    return true;
}

DATADEVICE vd_getDataDeviceById(int id)
{
    return byid[id];
}

DATADEVICE vd_getCurrentDataDevice()
{
    return _c;
}
void vd_setCurrentDataDevice(DATADEVICE d)
{
    _c = d;
}
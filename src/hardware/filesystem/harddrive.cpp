#include <hardware/filesystem/harddrive.h>
#include <memory.h>

#include <hardware/communication/port.h>

using namespace mangoos::hardware::communication;

static int _drive = 0;
static Port32Bit* _port1;
static Port32Bit* _port2;
static Port32Bit* _port3;
static Port32Bit* _port4;
static Port32Bit* _port5;
static Port32Bit* _port6;
static Port32Bit* _port7;
static Port32Bit* _port8;

void hdd_irq()
{
}

void hdd_init(int irq)
{
    hdd_set_working_drive(0x1F0);
}

void hdd_set_working_drive(int drive)
{
    _drive = drive;

    _port1 = new Port32Bit(_drive + 1);
    _port2 = new Port32Bit(_drive + 2);
    _port3 = new Port32Bit(_drive + 3);
    _port4 = new Port32Bit(_drive + 4);
    _port5 = new Port32Bit(_drive + 5);
    _port6 = new Port32Bit(_drive + 6);
    _port7 = new Port32Bit(_drive + 7);
    _port8 = new Port32Bit(0x1F0);
}

char *hdd_read_sector(int sectorLBA)
{
    char *buffer = (char *)malloc(512);
    _port1->Write(0);
    _port2->Write(1);
    _port3->Write((unsigned char)sectorLBA);
    _port4->Write((unsigned char)(sectorLBA >> 8));
    _port5->Write((unsigned char)(sectorLBA >> 16));
    _port6->Write(0x40);
    _port7->Write(0x20);

    for (int i = 0; i < 256; i++)
    {
        short tmpword = _port8->Read();
        buffer[i * 2] = (unsigned char)tmpword;
        buffer[i * 2 + 1] = (unsigned char)(tmpword >> 8);
    }

    return buffer;
}
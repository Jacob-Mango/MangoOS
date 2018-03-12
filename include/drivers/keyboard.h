#ifndef __MANGOOS__DRIVERS__KEYBOARD_H
#define __MANGOOS__DRIVERS__KEYBOARD_H

#include <common/types.h>

#include <drivers/driver.h>

#include <hardware/communication/interrupts.h>
#include <hardware/communication/port.h>

namespace mangoos
{
namespace drivers
{

class KeyboardEventHandler
{
  public:
	KeyboardEventHandler();

	virtual void OnKeyDown(char);
	virtual void OnKeyUp(char);
};

class KeyboardDriver : public mangoos::hardware::communication::InterruptHandler, public Driver
{
	mangoos::hardware::communication::Port8Bit dataport;
	mangoos::hardware::communication::Port8Bit commandport;

	KeyboardEventHandler *handler;

  public:
	KeyboardDriver(mangoos::hardware::communication::InterruptManager *manager, KeyboardEventHandler *handler);
	~KeyboardDriver();
	virtual uint32_t HandleInterrupt(uint32_t esp);
	virtual void Activate();
};
}
}

#endif
#ifndef __MANGOOS__DRIVERS__MOUSE_H
#define __MANGOOS__DRIVERS__MOUSE_H

#include <common/types.h>

#include <drivers/driver.h>

#include <hardware/communication/port.h>
#include <hardware/communication/interrupts.h>

namespace mangoos
{
namespace drivers
{

class MouseEventHandler
{
  public:
	MouseEventHandler();

	virtual void OnActivate();
	virtual void OnMouseDown(uint8_t button);
	virtual void OnMouseUp(uint8_t button);
	virtual void OnMouseMove(int x, int y);
};

class MouseDriver : public mangoos::hardware::communication::InterruptHandler, public Driver
{
	mangoos::hardware::communication::Port8Bit dataport;
	mangoos::hardware::communication::Port8Bit commandport;
	uint8_t buffer[3];
	uint8_t offset;
	uint8_t buttons;

	MouseEventHandler *handler;

  public:
	MouseDriver(mangoos::hardware::communication::InterruptManager *manager, MouseEventHandler *handler);
	~MouseDriver();
	virtual uint32_t HandleInterrupt(uint32_t esp);
	virtual void Activate();
};
}
}

#endif
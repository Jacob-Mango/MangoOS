#ifndef __MANGOOS__DRIVERS__VGA_H
#define __MANGOOS__DRIVERS__VGA_H

#include <common/types.h>

#include <drivers/driver.h>

#include <hardware/communication/port.h>

namespace mangoos
{
namespace drivers
{

class VideoGraphicsArray
{
  protected:
	hardware::communication::Port8Bit miscPort;
	hardware::communication::Port8Bit crtcIndexPort;
	hardware::communication::Port8Bit crtcDataPort;
	hardware::communication::Port8Bit sequencerIndexPort;
	hardware::communication::Port8Bit sequencerDataPort;
	hardware::communication::Port8Bit graphicsControllerIndexPort;
	hardware::communication::Port8Bit graphicsControllerDataPort;
	hardware::communication::Port8Bit attributeControllerIndexPort;
	hardware::communication::Port8Bit attributeControllerReadPort;
	hardware::communication::Port8Bit attributeControllerWritePort;
	hardware::communication::Port8Bit attributeControllerResetPort;

	void WriteRegisters(uint8_t *registers);
	uint8_t *GetFrameBufferSegment();

	virtual uint8_t GetColorIndex(uint8_t r, uint8_t g, uint8_t b);

  public:
	VideoGraphicsArray();
	~VideoGraphicsArray();

	virtual bool SupportsMode(uint32_t width, uint32_t height, uint32_t colordepth);
	virtual bool SetMode(uint32_t width, uint32_t height, uint32_t colordepth);
	virtual void PutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
	virtual void PutPixel(uint32_t x, uint32_t y, uint8_t colorIndex);

	virtual void FillRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b);
};
}
}

#endif
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

	void WriteRegisters(common::uint8_t *registers);
	common::uint8_t *GetFrameBufferSegment();

	virtual common::uint8_t GetColorIndex(common::uint8_t r, common::uint8_t g, common::uint8_t b);

  public:
	VideoGraphicsArray();
	~VideoGraphicsArray();

	virtual bool SupportsMode(common::uint32_t width, common::uint32_t height, common::uint32_t colordepth);
	virtual bool SetMode(common::uint32_t width, common::uint32_t height, common::uint32_t colordepth);
	virtual void PutPixel(common::uint32_t x, common::uint32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b);
	virtual void PutPixel(common::uint32_t x, common::uint32_t y, common::uint8_t colorIndex);

	virtual void FillRectangle(common::uint32_t x, common::uint32_t y, common::uint32_t w, common::uint32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b);
};
}
}

#endif
#ifndef __MANGOOS__GDT_H
#define __MANGOOS__GDT_H

#include <common/types.h>

namespace mangoos
{

class GlobalDescriptorTable
{
  public:
	class SegmentDescriptor
	{
	  private:
		mangoos::common::uint16_t limit_lo;
		mangoos::common::uint16_t base_lo;
		mangoos::common::uint8_t base_hi;
		mangoos::common::uint8_t type;
		mangoos::common::uint8_t limit_hi;
		mangoos::common::uint8_t base_vhi;

	  public:
		SegmentDescriptor(mangoos::common::uint32_t base, mangoos::common::uint32_t limit, mangoos::common::uint8_t type);
		mangoos::common::uint32_t Base();
		mangoos::common::uint32_t Limit();
	} __attribute__((packed));

  private:
	SegmentDescriptor nullSegmentSelector;
	SegmentDescriptor unusedSegmentSelector;
	SegmentDescriptor codeSegmentSelector;
	SegmentDescriptor dataSegmentSelector;

  public:
	GlobalDescriptorTable();
	~GlobalDescriptorTable();

	mangoos::common::uint16_t CodeSegmentSelector();
	mangoos::common::uint16_t DataSegmentSelector();
};
}

#endif
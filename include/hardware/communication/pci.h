#ifndef __MANGOOS__HARDWARECOMMUNICATION__PCI_H
#define __MANGOOS__HARDWARECOMMUNICATION__PCI_H

#include <common/types.h>

#include <drivers/driver.h>

#include <hardware/communication/port.h>
#include <hardware/communication/interrupts.h>

namespace mangoos
{
namespace hardware
{
namespace communication
{

enum BaseAddressRegisterType
{
	MemoryMapping = 0,
	InputOutput = 1
};

class BaseAddressRegister
{
  public:
	bool prefetchable;
	mangoos::common::uint8_t *address;
	mangoos::common::uint32_t size;
	BaseAddressRegisterType type;
};

class PeripheralComponentInterconnectDeviceDescriptor
{
  public:
	mangoos::common::uint32_t portBase;
	mangoos::common::uint32_t interrupt;

	mangoos::common::uint16_t bus;
	mangoos::common::uint16_t device;
	mangoos::common::uint16_t function;

	mangoos::common::uint16_t vendor_id;
	mangoos::common::uint16_t device_id;

	mangoos::common::uint8_t class_id;
	mangoos::common::uint8_t subclass_id;
	mangoos::common::uint8_t interface_id;

	mangoos::common::uint8_t revision;

	PeripheralComponentInterconnectDeviceDescriptor();
	~PeripheralComponentInterconnectDeviceDescriptor();
};

class PeripheralComponentInterconnectController
{
	Port32Bit dataPort;
	Port32Bit commandPort;

  public:
	PeripheralComponentInterconnectController();
	~PeripheralComponentInterconnectController();

	mangoos::common::uint32_t Read(mangoos::common::uint16_t bus, mangoos::common::uint16_t device, mangoos::common::uint16_t function, mangoos::common::uint32_t registeroffset);
	void Write(mangoos::common::uint16_t bus, mangoos::common::uint16_t device, mangoos::common::uint16_t function, mangoos::common::uint32_t registeroffset, mangoos::common::uint32_t value);
	bool DeviceHasFunctions(mangoos::common::uint16_t bus, mangoos::common::uint16_t device);

	void SelectDrivers(mangoos::drivers::DriverManager *driverManager, mangoos::hardware::communication::InterruptManager *interrupts);
	mangoos::drivers::Driver *GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, mangoos::hardware::communication::InterruptManager *interrupts);
	PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(mangoos::common::uint16_t bus, mangoos::common::uint16_t device, mangoos::common::uint16_t function);
	BaseAddressRegister GetBaseAddressRegister(mangoos::common::uint16_t bus, mangoos::common::uint16_t device, mangoos::common::uint16_t function, mangoos::common::uint16_t bar);
};
}
}
}
#endif
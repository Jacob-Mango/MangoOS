#include <common/types.h>

#include <gdt.h>
#include <memory.h>
#include <string.h>

#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>

#include <hardware/communication/interrupts.h>
#include <hardware/communication/pci.h>

#include <tasks/taskmanager.h>
#include <tasks/task.h>

using namespace mangoos;
using namespace mangoos::common;
using namespace mangoos::drivers;
using namespace mangoos::hardware::communication;

void printf(char *str)
{
	static uint16_t *VideoMemory = (uint16_t *)0xb8000;

	static uint8_t x = 0, y = 0;

	for (int i = 0; str[i] != '\0'; ++i)
	{

		if (x >= 80)
		{
			x = 0;
			y++;
		}

		if (y >= 25)
		{
			for (y = 1; y < 25; y++)
				for (x = 0; x < 80; x++)
					VideoMemory[80 * (y - 1) + x] = VideoMemory[80 * y + x];

			x = 0;
			y = 24;
		}

		switch (str[i])
		{
		case '\n':
			for (x; x < 80; x++)
				VideoMemory[80 * y + x] = ' ';
			x = 0;

			y++;
			break;
		default:
			VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
			x++;
			break;
		}
	}
}

void printfHex(uint8_t key)
{
	char *foo = "00";
	char *hex = "0123456789ABCDEF";
	foo[0] = hex[(key >> 4) & 0xF];
	foo[1] = hex[key & 0xF];
	printf(foo);
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
	for (constructor *i = &start_ctors; i != &end_ctors; i++)
		(*i)();
}

class Shell : public KeyboardEventHandler
{
  private:
	char *currentLine;
	int lineLength;

  public:
	void OnKeyDown(char c)
	{
		char *temp;
		memcpy(temp, currentLine, lineLength + 1);
		temp[lineLength] = c;
		memcpy(currentLine, temp, lineLength++);

		char *foo = " ";
		foo[0] = c;
		printf(foo);

		if (c == '\n')
		{
			RunLine();
			ClearLine();
		}
	}

	void RunLine()
	{
		if (str_begins_with(currentLine, "test") > 0)
		{
			printf("YOOOO");
		} else {
			
		}
	}

	void ClearLine()
	{
		currentLine = "";
		lineLength = 0;
	}
};

extern "C" void kernelMain(const void *multiboot_structure, uint32_t /*multiboot_magic*/)
{
	printf("Loading...");

	GlobalDescriptorTable gdt;

	TaskManager taskManager;

	InterruptManager interrupts(0x20, &gdt, &taskManager);

	printf("Initializing Hardware, Stage 1\n");

	DriverManager drvManager;

	Shell *shell = new Shell();

	KeyboardDriver keyboard(&interrupts, shell);
	drvManager.AddDriver(&keyboard);

	PeripheralComponentInterconnectController PCIController;
	PCIController.SelectDrivers(&drvManager, &interrupts);

	printf("Initializing Hardware, Stage 2\n");
	drvManager.ActivateAll();

	printf("Initializing Hardware, Stage 3\n");
	interrupts.Activate();

	//int width = 320;
	//int height = 200;
	//VideoGraphicsArray vga;

	while (1)
	{
	}
}
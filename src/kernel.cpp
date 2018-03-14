#include <common/types.h>
#include <common/print.h>

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
using namespace mangoos::drivers;
using namespace mangoos::hardware::communication;

bool Shutdown = false;

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

	bool upper = false;

	bool started = false;

  public:
	Shell()
	{
	}

	void Start()
	{
		started = true;
		ResetLine();
	}

	void OnKeyDown(char c)
	{
		if (!started)
			return;

		if (c == 0x2A)
		{
			upper = true;
			return;
		}
		else if (c == 0x36)
		{
			upper = false;
			return;
		}

		if (c == 0x3A)
			upper = !upper;

		char *foo = " ";
		if (upper)
			foo[0] = toupper(c);
		else
			foo[0] = tolower(c);

		char *temp;
		memcpy(temp, currentLine, lineLength + 1);
		temp[lineLength] = c;
		memcpy(currentLine, temp, lineLength++);

		printf(foo);

		if (c == '\n')
		{
			RunLine();
			ResetLine();
		}
	}

	void RunLine()
	{
		if (str_begins_with(currentLine, "help"))
		{
			Console::Print("clear - Clears the screen.\n");
			Console::Print("shutdown - Shutsdown the PC.\n");
		}
		else if (str_begins_with(currentLine, "shutdown"))
		{
			Console::Print("Shutting down the PC.\n");
			Shutdown = true;
		}
		else if (str_begins_with(currentLine, "clear"))
		{
			for (uint16_t yi = 0; yi < 25; yi++)
				for (uint16_t xi = 0; xi < 80; xi++)
					Console::Set(xi, yi, ' ');
			Console::SetCursor(0, 0);
		}
		else
		{
			Console::Print("Unknown command, type \"help\" for more commands.");
		}
	}

	void ResetLine()
	{
		if (Shutdown)
			return;
		Console::Print("\n> ");
		currentLine = "";
		lineLength = 0;
	}

	void Update()
	{
		if (Shutdown)
			return;
	}
};

void ShutdownSequence()
{
	int mhz = 1000;
	int ghz = 5;
	int ns = 100;
	int ms = 1000;
	int s = 2;

	int time = mhz * ghz * ns * ms * s;

	for (int i = 0; i < time / 2; i++)
		;

	Console::Print("Goodbye.");

	for (int i = 0; i < time / 2; i++)
		;
}

extern "C" void kernelMain(const void *multiboot_structure, uint32_t /*multiboot_magic*/)
{
	Console::Initialize();

	printf("Loading...");

	GlobalDescriptorTable gdt;

	TaskManager taskManager;

	printf("Initializing the Shell\n");
	Shell shell;

	InterruptManager interrupts(0x20, &gdt, &taskManager);

	printf("Initializing Hardware, Stage 1\n");

	DriverManager drvManager;

	KeyboardDriver keyboard(&interrupts, &shell);
	drvManager.AddDriver(&keyboard);

	PeripheralComponentInterconnectController PCIController;
	PCIController.SelectDrivers(&drvManager, &interrupts);

	printf("Initializing Hardware, Stage 2\n");
	drvManager.ActivateAll();

	printf("Initializing Hardware, Stage 3\n");
	interrupts.Activate();

	printf("Starting the Shell\n");
	shell.Start();

	while (!Shutdown)
		;

	ShutdownSequence();
}
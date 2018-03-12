GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
          obj/gdt.o \
          obj/memory.o \
          obj/string.o \
          obj/drivers/driver.o \
          obj/hardware/communication/port.o \
          obj/hardware/communication/interruptstubs.o \
          obj/hardware/communication/interrupts.o \
          obj/hardware/communication/pci.o \
          obj/hardware/filesystem/filesystem.o \
          obj/hardware/filesystem/FAT12.o \
          obj/hardware/filesystem/harddrive.o \
          obj/hardware/filesystem/virtualdevice.o \
          obj/drivers/keyboard.o \
          obj/drivers/mouse.o \
          obj/drivers/vga.o \
          obj/tasks/task.o \
          obj/tasks/taskmanager.o \
          obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o kernel.bin $(objects)

iso:
	rm -rf out
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "The Mango OS" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=kernel.iso iso
	cp kernel.iso ../VM/Mango.ISO
	rm -rf iso

install:
	sudo cp $< /boot/kernel.bin

.PHONY: clean
clean:
	rm -f $(objects)
	rm -rf obj
	rm -f kernel.bin
	rm -f kernel.iso


#ojbs = uboot boot setup application_processor handler32 long protected_mp handler64 long_mp
mods = uboot boot setup protected
submods = lib

ojbs = $(patsubst %,%.o,$(mods))

.PHONY: Makefile all lib clean distclean image flash run
all: $(submods) $(ojbs)

lib:
	make -C lib

uboot.o: boot.asm
	nasm -I./ -I./include/ -dUBOOT boot.asm -o $@

%.o: %.asm
	nasm -I./ -I./include/ $*.asm -o $@

floppy.img:
	### 1.44MB
	dd if=/dev/zero of=$@ bs=512 count=2880

disk.img:
	### 32MB CHS=65/16/63
	dd if=/dev/zero of=$@ bs=512 count=65520

usbdisk.img:
	### 32MB CHS=65/16/63
	dd if=/dev/zero of=$@ bs=512 count=65520

image: all floppy.img disk.img
	### floppy
	dd conv=notrunc if=boot.o of=floppy.img seek=0 count=1
	dd conv=notrunc if=setup.o of=floppy.img seek=1 count=4
	dd conv=notrunc if=protected.o of=floppy.img seek=64 count=10
	dd conv=notrunc if=lib/lib16.o of=floppy.img seek=20 count=2
	dd conv=notrunc if=lib/lib32.o of=floppy.img seek=192 count=10
	### harddisk
	dd conv=notrunc if=uboot.o of=disk.img seek=0 count=1
	dd conv=notrunc if=setup.o of=disk.img seek=1 count=4
	dd conv=notrunc if=protected.o of=disk.img seek=64 count=10
	dd conv=notrunc if=lib/lib16.o of=disk.img seek=20 count=2
	dd conv=notrunc if=lib/lib32.o of=disk.img seek=192 count=10

flash: all usbdisk.img
	### usbdisk
	dd conv=notrunc if=uboot.o of=usbdisk.img seek=63 count=1
	dd conv=notrunc if=setup.o of=usbdisk.img seek=1 count=4
	dd conv=notrunc if=protected.o of=usbdisk.img seek=64 count=10
	dd conv=notrunc if=lib/lib16.o of=usbdisk.img seek=20 count=2
	dd conv=notrunc if=lib/lib32.o of=usbdisk.img seek=192 count=10
	#dd if=usbdisk.img of=/dev/sdx

run: image
	bochs -qf bochs.bxrc || true
debug: image
	bochs-debugger -qf bochs.bxrc || true

clean:
	for m in $(submods); do make -C $$m clean; done
	rm -f $(ojbs)

distclean: clean
	rm -f bochsout.txt floppy.img disk.img usbdisk.img


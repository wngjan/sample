submods = lib common

.PHONY: Makefile all lib common clean distclean images run
all: $(submods) images

common:
	make -C common

lib:
	make -C lib

run: images
	bochs -qf bochs.bxrc || true

debug: images
	bochs-debugger -qf bochs.bxrc || true

images: $(submods) floppy.img disk.img
	### floppy
	dd conv=notrunc if=common/boot.o of=floppy.img seek=0 count=1
	dd conv=notrunc if=common/setup.o of=floppy.img seek=1 count=4
	dd conv=notrunc if=common/protected.o of=floppy.img seek=64 count=10
	dd conv=notrunc if=lib/lib16.o of=floppy.img seek=20 count=2
	dd conv=notrunc if=lib/lib32.o of=floppy.img seek=192 count=10
	### harddisk
	dd conv=notrunc if=common/uboot.o of=disk.img seek=0 count=1
	dd conv=notrunc if=common/setup.o of=disk.img seek=1 count=4
	dd conv=notrunc if=common/protected.o of=disk.img seek=64 count=10
	dd conv=notrunc if=lib/lib16.o of=disk.img seek=20 count=2
	dd conv=notrunc if=lib/lib32.o of=disk.img seek=192 count=10

floppy.img:
	### 1.44MB
	dd if=/dev/zero of=$@ bs=512 count=2880

disk.img:
	### 32MB CHS=65/16/63
	dd if=/dev/zero of=$@ bs=512 count=65520

clean:
	for m in $(submods); do make -C $$m clean; done
	rm -f $(ojbs)

distclean: clean
	rm -f bochsout.txt floppy.img disk.img usbdisk.img


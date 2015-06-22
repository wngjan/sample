#ojbs = uboot boot setup application_processor handler32 long protected_mp handler64 long_mp
srcs = uboot boot setup protected
ojbs = $(patsubst %,%.o,$(srcs))

.PHONY: all clean
all: $(ojbs)

uboot.o: boot.asm
	nasm -I./ -I../include/ -dUBOOT boot.asm -o $@

%.o: %.asm
	nasm -I./ -I../include/ $*.asm -o $@

clean:
	rm -f $(ojbs)

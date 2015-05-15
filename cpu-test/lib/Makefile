
#mods = apic64 creg hpet lib64 page64 pic8259A system_data64 apic debug64 ioapic msr pci smm x87 conforming_lib32 debug lib16 mtrr perfmon64 sse64 cpuid fpu64 lib32 page32 perfmon sse
mods = lib16 lib32
ojbs = $(patsubst %,%.o,$(mods))

.PHONY: all clean
all: $(ojbs)

%.o: %.asm
	nasm -I./ -I../include/ $*.asm -o $@

clean:
	rm -f $(ojbs)

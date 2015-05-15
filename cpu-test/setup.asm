; setup.asm
; Copyright (c) 2009-2012 mik
; All rights reserved.

%include "support.inc"
%include "protected.inc"

	bits 16
	org SETUP_SEG - 2

SETUP_BEGIN: ; begin of setup
setup_length    dw (SETUP_END - SETUP_BEGIN)

setup_entry:
	cli

	db 0x66
	lgdt [__gdt_pointer]

	db 0x66
	lidt [__idt_pointer]

	mov WORD [tss32_desc], 0x68 + __io_bitmap_end - __io_bitmap
	mov WORD [tss32_desc + 2], __task_status_segment
	mov BYTE [tss32_desc + 5], 0x80 | TSS32

	mov WORD [tss_test_desc], 0x68 + __io_bitmap_end - __io_bitmap
	mov WORD [tss_test_desc + 2], __test_tss
	mov BYTE [tss_test_desc + 5], 0x80 | TSS32

	mov WORD [ldt_desc], __local_descriptor_table_end - __local_descriptor_table - 1        ; limit
	mov DWORD [ldt_desc + 4], __local_descriptor_table              ; base [31:24]
	mov DWORD [ldt_desc + 2], __local_descriptor_table              ; base [23:0]
	mov WORD [ldt_desc + 5], 80h | LDT_SEGMENT                      ; DPL=0, type=LDT

	mov eax, cr0
	bts eax, 0                              ; CR0.PE = 1
	mov cr0, eax

	jmp kernel_code32_sel:entry32

entry32:
	bits 32
	mov ax, kernel_data32_sel
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov esp, 0x7ff0

	;; load TSS segment
	mov ax, tss32_sel
	ltr ax

	call test_cpuid
    mov esi, [message_table + eax * 4]
    call puts32
    
;; 获得最大 basic 功能号
    mov eax, 0
    cpuid
    mov esi, eax
    mov di, value_address
    call itoah
    mov esi, basic_message
    call puts32
    mov esi, value_address
    call puts32
    call println
    
;; 获得最大 extended 功能号        
    mov eax, 0x80000000
    cpuid
    mov esi, eax
    mov di, value_address
    call itoah
    mov esi, extend_message
    call puts32
    mov esi, value_address
    call puts32
    call println
    
    jmp $

support_message         db 'support CPUID instruction', 13, 10, 0
no_support_message      db 'no support CPUID instruction', 13, 10, 0                
message_table           dd no_support_message, support_message, 0

basic_message           db 'maximun basic function: 0x', 0
extend_message          db 'maximun extended function: 0x', 0
value_address           dd 0, 0, 0

	jmp PROTECTED_SEG


__global_descriptor_table:

null_desc                       dq 0                    ; NULL descriptor
code16_desc                     dq 0x00009a000000ffff   ; base=0, limit=0xffff, DPL=0
data16_desc                     dq 0x000092000000ffff   ; base=0, limit=0xffff, DPL=0
kernel_code32_desc              dq 0x00cf9a000000ffff   ; non-conforming, DPL=0, P=1
kernel_data32_desc              dq 0x00cf92000000ffff   ; DPL=0, P=1, writeable, expand-up
user_code32_desc                dq 0x00cff8000000ffff   ; non-conforming, DPL=3, P=1
user_data32_desc                dq 0x00cff2000000ffff   ; DPL=3, P=1, writeable, expand-up
tss32_desc                      dq 0
call_gate_desc                  dq 0
conforming_code32_desc          dq 0x00cf9e000000ffff   ; conforming, DPL=0, P=1
tss_test_desc                   dq 0
task_gate_desc                  dq 0
ldt_desc                        dq 0
times 10                        dq 0
__global_descriptor_table_end:

__interrupt_descriptor_table:
	times 0x80 dq 0                                ; 保留 0x80 个 vector
__interrupt_descriptor_table_end:

__local_descriptor_table:
	times 10 dq 0
__local_descriptor_table_end:

__task_status_segment:
	dd 0
	dd PROCESSOR_KERNEL_ESP         ; esp0
	dd kernel_data32_sel            ; ss0
	dq 0                            ; ss1/esp1
	dq 0                            ; ss2/esp2
times 19 dd 0
	dw 0
	dw __io_bitmap - __task_status_segment

__task_status_segment_end:

__test_tss:
	dd 0
	dd 0x8f00                       ; esp0
	dd kernel_data32_sel            ; ss0
	dq 0                            ; ss1/esp1
	dq 0                            ; ss2/esp2
times 19 dd 0
	dw 0
	dw __io_bitmap - __test_tss
__test_tss_end:

__io_bitmap:
	times 10 db 0
__io_bitmap_end:

__gdt_pointer:
gdt_limit       dw      (__global_descriptor_table_end - __global_descriptor_table) - 1
gdt_base        dd      __global_descriptor_table

__idt_pointer:
idt_limit       dw      (__interrupt_descriptor_table_end - __interrupt_descriptor_table) - 1
idt_base        dd       __interrupt_descriptor_table

__ivt_pointer:
			dw 3FFH
			dd 0

FUNCTION_IMPORT_TABLE:

test_cpuid:             jmp LIB32_SEG + LIB32_TEST_CPUID * 5
puts32:                 jmp LIB32_SEG + LIB32_PUTS * 5
itoah:   				jmp LIB32_SEG + LIB32_ITOAH * 5
println:                jmp LIB32_SEG + LIB32_PRINTLN * 5
print_value:            jmp LIB32_SEG + LIB32_PRINT_VALUE * 5
dump_encodes:           jmp  LIB32_SEG + LIB32_DUMP_ENCODES * 5

SETUP_END: ; end of setup
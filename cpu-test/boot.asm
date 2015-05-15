; boot.asm
; Copyright (c) 2009-2012 mik
; All rights reserved.

%include "support.inc"
%include "ports.inc"

	bits 16
	; start from 0x7c00
	org BOOT_SEG

start:
	cli
	NMI_DISABLE

	; enable a20 line
	FAST_A20_ENABLE

	sti

	; set BOOT_SEG environment
	mov ax, cs
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov sp, BOOT_SEG                                ; �� stack ��Ϊ BOOT_SEG
	mov [driver_number], dl                          ; ���� boot driver
	mov WORD [buffer_selector], es
	call get_driver_parameters
	call clear_screen

	mov WORD [buffer_offset], SETUP_SEG - 2
	mov DWORD [start_sector], SETUP_SECTOR
	call load_module                                ; ���� setup ģ��

	mov WORD [buffer_offset], LIB16_SEG - 2
	mov DWORD [start_sector], LIB16_SECTOR
	call load_module                                ; ���� lib16 ģ��

	mov WORD [buffer_offset], 9000h - 2
	mov DWORD [start_sector], LONG_SECTOR
	call load_module                                ; ���� long ģ��

	mov si, 9000h
	push es
	mov ax, 1000h
	mov es, ax
	xor di, di
	mov cx, WORD [9000h - 2]
	rep movsb
	pop es

	mov DWORD [start_sector], PROTECTED_SECTOR
	mov WORD [buffer_offset], PROTECTED_SEG - 2
	call load_module                                ; ���� protected ģ��

	mov DWORD [start_sector], LIB32_SECTOR
	mov WORD [buffer_offset], LIB32_SEG - 2
	call load_module                                ; ���� lib32 ģ��

	jmp SETUP_SEG                                   ; ת�� SETUP_SEG

;------------------------------------------------------
; clear_screen()
; description:
;                clear the screen & set cursor position at (0,0)
;------------------------------------------------------
clear_screen:
	pusha
	mov ax, 0x0600
	xor cx, cx
	xor bh, 0x0f            ; white
	mov dh, 24
	mov dl, 79
	int 0x10
	mov ah, 02
	mov bh, 0
	mov dx, 0
	int 0x10
	popa
	ret

;--------------------------------------------
; read_sector(): ��ȡ����
; input:
;       ʹ�� disk_address_packet �ṹ
; output:
;       0-successfule
; unsuccessful:
;       ax - error code
;-------------------------------------------
read_sector:
	push es
	mov es, WORD [buffer_selector]                  ; es = buffer_selector

	;
	; ��ʼ�������� 0FFFFFFFFh
	;
	cmp DWORD [start_sector + 4], 0
	jnz check_lba

	;
	; ���ģ���ڵ��� 504M ��������ʹ�� CHS ģʽ
	;
	cmp DWORD [start_sector], 504 * 1024 * 2        ; 504M
	jb chs_mode

check_lba:
	;
	; ����Ƿ�֧�� 13h ��չ����
	;
	call check_int13h_extension
	test ax, ax
	jz chs_mode

lba_mode:
	;
	; ʹ�� LBA ��ʽ�� sector
	;
	call read_sector_with_extension
	test ax, ax
	jz read_sector_done

chs_mode:
	;
	; ʹ�� CHS ��ʽ�� sector
	;
	call read_sector_with_chs
read_sector_done:
	pop es
	ret

;--------------------------------------------------------
; check_int13h_extension(): �����Ƿ�֧�� int13h ��չ����
; input:
;       ʹ�� driver_paramter_table �ṹ
; ouput:
;       1 - support, 0 - not support
;--------------------------------------------------------
check_int13h_extension:
	push bx
	mov bx, 55AAh
	mov dl, [driver_number]                   ; driver number
	mov ah, 41h
	int 13h
	setnc al                                ; c = ʧ��
	jc do_check_int13h_extension_done
	cmp bx, 0AA55h
	setz al                                 ; nz = ��֧��
	jnz do_check_int13h_extension_done
	test cx, 1
	setnz al                                ; z = ��֧����չ���ܺţ�AH=42h-44h,47h,48h
do_check_int13h_extension_done:
	pop bx
	movzx ax, al
	ret

;--------------------------------------------------------------
; read_sector_with_extension(): ʹ����չ���ܶ�����
; input:
;       ʹ�� disk_address_packet �ṹ
;--------------------------------------------------------------
read_sector_with_extension:
	mov si, disk_address_packet             ; DS:SI = disk address packet
	mov dl, [driver_number]                   ; driver
	mov ah, 42h                             ; ��չ���ܺ�
	int 13h
	movzx ax, ah                            ; if unsuccessful, ah = error code
	ret

;-------------------------------------------------------------
; read_sector_with_chs(): ʹ�� CHS ģʽ������
; input:
;       ʹ�� disk_address_packet �� driver_paramter_table
; output:
;       0 - successful
; unsuccessful:
;       ax - error code
;-------------------------------------------------------------
read_sector_with_chs:
	;
	; �� LBA ת��Ϊ CHS��ʹ�� int 13h, ax = 02h ��
	;
	call do_lba_to_chs
	mov dl, [driver_number]                   ; driver number
	mov es, WORD [buffer_selector]          ; buffer segment
	mov bx, WORD [buffer_offset]            ; buffre offset
	mov al, BYTE [read_sectors]             ; number of sector for read
	mov ah, 02h
	int 13h
	movzx ax, ah                            ; if unsuccessful, ah = error code
read_sector_with_chs_done:
	ret

;-------------------------------------------------------------
; do_lba_to_chs(): LBA ��ת��Ϊ CHS
; input:
;       ʹ�� driver_parameter_table �� disk_address_packet �ṹ
; output:
;       ch - cylinder �� 8 λ
;       cl - [5:0] sector, [7:6] cylinder �� 2 λ
;       dh - header
;
; ������
;
; 1)
;       eax = LBA / (head_maximum * sector_maximum),  cylinder = eax
;       edx = LBA % (head_maximum * sector_maximum)
; 2)
;       eax = edx / sector_maximum, head = eax
;       edx = edx % sector_maximum
; 3)
;       sector = edx + 1
;-------------------------------------------------------------
do_lba_to_chs:
	movzx ecx, BYTE [sector_maximum]        ; sector_maximum
	movzx eax, BYTE [header_maximum]        ; head_maximum
	imul ecx, eax                           ; ecx = head_maximum * sector_maximum
	mov eax, DWORD [start_sector]           ; LBA[31:0]
	mov edx, DWORD [start_sector + 4]       ; LBA[63:32]
	div ecx                                 ; eax = LBA / (head_maximum * sector_maximum)
	mov ebx, eax                            ; ebx = cylinder
	mov eax, edx
	xor edx, edx
	movzx ecx, BYTE [sector_maximum]
	div ecx                                 ; LBA % (head_maximum * sector_maximum) / sector_maximum
	inc edx                                 ; edx = sector, eax = head
	mov cl, dl                              ; secotr[5:0]
	mov ch, bl                              ; cylinder[7:0]
	shr bx, 2
	and bx, 0C0h
	or cl, bl                               ; cylinder[9:8]
	mov dh, al                              ; head
	ret

;---------------------------------------------------------------------
; get_driver_parameters(): �õ� driver ����
; input:
;       ʹ�� driver_parameters_table �ṹ
; output:
;       0 - successful, 1 - failure
; failure:
;       ax - error code
;---------------------------------------------------------------------
get_driver_parameters:
	push dx
	push cx
	push bx
	mov ah, 08h                             ; 08h ���ܺţ��� driver parameters
	mov dl, [driver_number]                 ; driver number
	mov di, [parameter_table]               ; es:di = address of parameter table
	int 13h
	jc get_driver_parameters_done
	mov BYTE [driver_type], bl              ; driver type for floppy drivers
	inc dh
	mov BYTE [header_maximum], dh           ; ��� head ��
	mov BYTE [sector_maximum], cl           ; ��� sector ��
	and BYTE [sector_maximum], 3Fh          ; �� 6 λ
	shr cl, 6
	rol cx, 8
	and cx, 03FFh                           ; ��� cylinder ��
	inc cx
	mov [cylinder_maximum], cx              ; cylinder
get_driver_parameters_done:
	movzx ax, ah                            ; if unsuccessful, ax = error code
	pop bx
	pop cx
	pop dx
	ret

;-------------------------------------------------------------------
; load_module(int module_sector, char *buf):  ����ģ�鵽 buf ������
; example:
;       load_module(SETUP_SEG, SETUP_SECTOR);
;-------------------------------------------------------------------
load_module:
	mov WORD [read_sectors], 1
	call read_sector
	test ax, ax
	jnz do_load_module_done
	movzx esi, WORD [buffer_offset]
	mov es, WORD [buffer_selector]
	movzx ecx, WORD [es: esi]                                       ; ��ȡģ�� siz
	test cx, cx
	setz al
	jz do_load_module_done
	add ecx, 512 - 1
	shr ecx, 9							; ���� block��sectors��
	mov WORD [read_sectors], cx                                     ;
	call read_sector
do_load_module_done:
	ret

;
; ���� int 13h ʹ�õ� disk address packet������ 13h ��/д
;
disk_address_packet:
	size                    DW      10h             ; size of packet
	read_sectors            DW      0               ; number of sectors
	buffer_offset           DW      0               ; buffer far pointer(16:16)
	buffer_selector         DW      0               ; Ĭ�� buffer Ϊ 0
	start_sector            DQ      0               ; start sector

;
; ���ڱ��� int 13h/ax=08h ��õ� driver ����
;
driver_parameters_table:
	driver_type             DB      0               ; driver type
	driver_number           DB      0               ; driver number
	cylinder_maximum        DW      0               ; ���� cylinder ��
	header_maximum          DW      0               ; ���� header ��
	sector_maximum          DW      0               ; ���� sector ��
	parameter_table         DW      0               ; address of parameter table

times 510-($-$$) db 0
	dw 0xaa55

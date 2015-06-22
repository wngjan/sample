; boot.inc
; Copyright (c) 2009-2013 mik 
; All rights reserved.

%ifndef __BOOT_INC__
%define __BOOT_INC__


;-------------------------------------------------------------------
; �� DISK_ADDRESS_PACKET: ���� int 13h ���ڶ�д�� packet
; input��
;       $1: ��ʼ���� 
;       $2: far pointer �� selector
;       $3: far pointer �� offset
;       $4: ��Ҫ��ȡ��������
; example:
;       DISK_ADDRESS_PACKET 64, 2000, 0, 10
;       �� 64 ��������ȡ 10 ����������ַ 2000:0000h ��
;-------------------------------------------------------------------
%macro DISK_ADDRESS_PACKET 4
packet_size:            DW      10h                             ; packet �ṹ size
sectors:                DW      $4                              ; ��ȡ sectors ��
buffer_offset:          DW      $3                              ; offset 
buffer_selector:        DW      $2                              ; selector
start_sector:           DQ      $1                              ; ��ʼ sector
%endmacro


%macro DISK_PARAMETERS 0
driver_type:            DB      0
driver_number:          DB      0
maximum_cylinders:      DW      0
maximum_headers:        DW      0
maximum_sector:         DW      0
parameter_table:        DW      0
%endmacro


%endif
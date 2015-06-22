;*************************************************
; ex.inc                                         *
; Copyright (c) 2009-2013 ��־                   *
; All rights reserved.                           *
;*************************************************

%ifndef _EX_INC
%define _EX_INC

;;
;; ������ ex.asm ��ʹ�ã�64 λ�Լ� 32 λ��ͨ�õļĴ�����
;;

%if __BITS__ == 64
        
%define R0      rax
%define R1      rcx
%define R2      rdx
%define R3      rbx
%define R4      rsp
%define R5      rbp
%define R6      rsi
%define R7      rdi

%else

%define R0      eax
%define R1      ecx
%define R2      edx
%define R3      ebx
%define R4      esp
%define R5      ebp
%define R6      esi
%define R7      edi

%endif




%endif
;************************************************
;* vmcs.inc                                     *
;* Copyright (c) 2009-2013 ��־                 *
;* All rights reserved.                         *
;************************************************

%ifndef _VMCS_INC
%define _VMCS_INC


%define GUEST_FLAG_PE                           1
%define GUEST_FLAG_PG                           80000000h
%define GUEST_FLAG_IA32E                        4
%define GUEST_FLAG_V8086                        8
%define GUEST_FLAG_UNRESTRICTED                 10h
%define GUEST_FLAG_EPT                          20h
%define GUEST_FLAG_USER                         40h


;;
;; ####### VMB(VM Manage Block���ṹ #######
;;

struc VMB
        .PhysicalBase           RESQ    1               ; VMCS region ������ַ
        .Base                   RESQ    1               ; VMCS region �����ַ
        .VsbBase                RESQ    1               ; VSB ��ַ
        .VsbPhysicalBase        RESQ    1               ; VSB ������ַ
        .DomainBase             RESQ    1               ; domain �����ַ
        .DomainPhysicalBase     RESQ    1               ; domain ������ַ
        .DomainPhysicalTop      RESQ    1               ; domain ������ַ����

        ;;
        ;; Guest ״̬
        ;;
        .GuestStatus            RESD    1


        ALIGNB 8

        ;;
        ;; guest ������ڵ�
        ;;
        .GuestEntry             RESQ    1
        .GuestStack             RESQ    1               ; guest ʹ�õ�ջ

        ;;
        ;; VMM ���
        ;;
        .HostEntry              RESQ    1
        .HostStack              RESQ    1

        ;;
        ;; ÿ�� VM �� EP4TA
        ;;
        .Ep4taBase              RESQ    1               ; ָ�� EPT PML4T �����ַ
        .Ep4taPhysicalBase      RESQ    1               ; ָ�� EPT PML4T ������ַ

        
        ;;
        ;; ÿ�� VM ���Լ��� VMX-preemption timer value ֵ
        ;;
        .VmxTimerValue          RESD    1

        ;;
        ;; ÿ�� logical processor ��ÿ��VMCS�� VPID
        ;;
        .Vpid                   RESW    1
        



        ALIGNB 4
        .GuestFlags             RESD    1

        ;;
        ;; ĳЩ����£����� VMM do_xxx�����Ĳ�����
        ;;
        .DoProcessParam         RESD    1
        .VmmDumpVmcsMask        RESQ    1

        ;;
        ;; APIC-access address
        ;;
        .ApicAccessAddress      RESQ    1
        .ApicAccessPhyAddress   RESQ    1

        ;;
        ;; #### �����������Ҫ�ڳ�ʼ��ʱ����ҳ�� ####
        ;; 1) IoBitmap A page
        ;; 2) IoBitmap B page
        ;; 3) Virtual-access page
        ;; 4) MSR-Bitmap page
        ;; 5) VM-entry/VM-exit MSR store page
        ;; 6) VM-exit MSR load page
        ;; 7) IoVteBuffer page
        ;; 8) MsrVteBuffer page
        ;; 9) GpaHteBuffer page
        ;;

        ;;
        ;; I/O-Bitmap Address A & B
        ;;        
        .IoBitmapAddressA       RESQ    1
        .IoBitmapPhyAddressA    RESQ    1
        .IoBitmapAddressB       RESQ    1
        .IoBitmapPhyAddressB    RESQ    1

        ;;
        ;; Virtual-APIC address
        ;;
        .VirtualApicAddress     RESQ    1
        .VirtualApicPhyAddress  RESQ    1

        ;;
        ;; MSR-Bitmap Address
        ;;
        .MsrBitmapAddress       RESQ    1
        .MsrBitmapPhyAddress    RESQ    1


        ;;
        ;; VM-exit MSR-store Address
        ;; 
        .VmEntryMsrLoadAddress:       
        .VmExitMsrStoreAddress          RESQ    1
        .VmEntryMsrLoadPhyAddress:
        .VmExitMsrStorePhyAddress       RESQ    1

        ;;
        ;; VM-exit MSR-load Address
        ;;        
        .VmExitMsrLoadAddress           RESQ    1
        .VmExitMsrLoadPhyAddress        RESQ    1

        ;;
        ;; IO ֵ�б� buffer��������¼ guest �� IO ��д���ֵ 
        ;;
        .IoVteBuffer                    RESQ    1
        .IoVteBufferPhyAddr             RESQ    1

        ;;
        ;; MSR ֵ�б� buffer��������¼ guest �� MSR д��ֵ
        ;;
        .MsrVteBuffer                   RESQ    1
        .MsrVteBufferPhyAddr            RESQ    1

        ;;
        ;; EPT page-fault �������̱� buffer��������¼ GPA �����⴦��
        ;;
        .GpaHteBuffer                   RESQ    1
        .GpaHteBufferPhyAddr            RESQ    1


        ;;
        ;; IO & MSR VTE ������¼
        ;;
        .IoVteCount                     RESD    1
        .MsrVteCount                    RESD    1
        .GpaHteCount                    RESD    1
        .IoVteIndex                     RESQ    1
        .MsrVteIndex                    RESQ    1
        .GpaHteIndex                    RESQ    1


        .VmEntryMsrLoadCount:
        .VmExitMsrStoreCount            RESD    1
        .VmExitMsrLoadCount             RESD    1


        ;;
        ;; IO ������־λ
        ;;
        .IoOperationFlags               RESD    1


        ;;
        ;; guest-GDT ������
        ;;       
        .GuestGmb                       RESB    GGMB_SIZE

        
        ALIGNB 4
        ;;
        ;;guest-IDT ������
        ;;
        .GuestImb                       RESB    GIMB_SIZE

        ;;
        ;; guest-TSS ������
        ;;
        .GuestTmb                       RESB    GTMB_SIZE


        ;;
        ;; guest-status ������
        ;; 
        .GuestSmb                       RESB    GSMB_SIZE


        ALIGNB 8
        ;;
        ;; EPTP list address
        ;;
        .EptpListAddress                RESQ    1


        ALIGNB 64
        ;;
        ;; posted-interrupt descriptor
        ;;
        PostedInterruptDesc             RESB    64


        VMB_SIZE                        EQU     $
        VMCS_MANAGE_BLOCK_SIZE          EQU     $
endstruc

%define IOP_FLAGS_8259_MASTER_INIT              1
%define IOP_FLAGS_8259_SLAVE_INIT               (1 << 1)


;;
;; VSB��VM Storage Block��������洢�飩
;;
struc VSB
        .Base                   RESQ            1
        .PhysicalBase           RESQ            1

        ;;
        ;; VM video buffer ������¼
        ;;
        .VmVideoBufferHead      RESQ            1
        .VmVideoBufferPtr       RESQ            1
        .VmVideoBufferLastChar  RESD            1
        .VmVideoBufferSize      RESD            1
        
        ;;
        ;; VM keyboard buffer ������¼
        ;;        
        ALIGNB 8
        .VmKeyBufferHead        RESQ            1
        .VmKeyBufferPtr         RESQ            1
        .VmKeyBufferSize        RESD            1
        

        ;;
        ;; guest �� context ��Ϣ
        ;;
        .Context:
        .Rax                    RESQ            1
        .Rcx                    RESQ            1
        .Rdx                    RESQ            1
        .Rbx                    RESQ            1
        .Rsp                    RESQ            1
        .Rbp                    RESQ            1
        .Rsi                    RESQ            1
        .Rdi                    RESQ            1
        .R8                     RESQ            1
        .R9                     RESQ            1
        .R10                    RESQ            1
        .R11                    RESQ            1
        .R12                    RESQ            1
        .R13                    RESQ            1
        .R14                    RESQ            1
        .R15                    RESQ            1        
        .Rip                    RESQ            1
        .Rflags                 RESQ            1 

        ;;
        ;; FPU ��Ԫ context
        ;;
        ALIGNB 16
        .FpuStateImage:
        .FpuEnvironmentImage:   RESB            28
        .FpuStackImage:         RESB            80

        ;;
        ;; XMM image ����ʹ���� FXSAVE/FXRSTOR ָ�512�ֽڣ�
        ;;
        ALIGNB 16
        .XMMStateImage:         RESB            512


        ;;
        ;; VM keyboard buffer ���򣬹� 256 ���ֽڣ����水��ɨ����
        ;;
        .VmKeyBuffer            RESB            256
        .Reserve                RESB            4096 - $


        ;;
        ;; VM video buffer ���򣬴�Ŵ���������Ļ��Ϣ
        ;; 4K�����Լ 25 * 80 * 2 ��Ϣ
        ;;
        .VmVideoBuffer          RESB            (4096 * 2 - $)

        

        VM_STORAGE_BLOCK_SIZE           EQU     $
        VSB_SIZE                        EQU     $        
endstruc


;;
;; I/O ֵ�б���ṹ
;;
struc IO_VTE
        .IoPort                         RESD    1
        .Value                          RESD    1

        IO_VTE_SIZE                     EQU     $
endstruc


;;
;; MSR ֵ�б���ṹ
;;
struc MSR_VTE
        .MsrIndex                       RESD    1
        .Value                          RESQ    1

        MSR_VTE_SIZE                    EQU     $
endstruc


;;
;; GPA handle table entry �ṹ
;;
struc GPA_HTE
        .GuestPhysicalAddress           RESQ    1
        .Handler                        RESQ    1

        GPA_HTE_SIZE                    EQU     $
endstruc


;;
;; �ⲿ�ж� route tabel entry
;;
struc EXTINT_RTE
        .ProcessorIndex                 RESD    1
        .Vector                         RESD    1
        .Flags                          RESD    1

        EXTINT_RTE_SIZE                 EQU     $
endstruc

%define RTE_8259_IRQ0                   1
%define RTE_8259_IRQ1                   (1 << 1)



;;
;; guest-IDT manage block
;;
struc GIMB
        .IdtPointer:
        .IdtLimit                       RESW    1
        .IdtBase                        RESQ    1

        .HookIdtLimit                   RESW    1
        .GpFlags                        RESD    1

        GIMB_SIZE                       EQU     $
endstruc


;;
;; guest-GDT manage block
;;
struc GGMB
        .GdtPointer:
        .GdtLimit                       RESW    1
        .GdtBase                        RESQ    1

        GGMB_SIZE                       EQU     $
endstruc


;;
;; guest-LDT manage block
;;
struc GLMB
        .LdtPointer:
        .LdtLimit                       RESW    1
        .LdtBase                        RESQ    1

        GLMB_SIZE                       EQU     $
endstruc


;;
;; guest-TSS manage block
;;
struc GTMB
        .TssSelector                    RESW    1
        .TssLimit                       RESD    1
        .TssAccessRights                RESD    1

        ALIGNB 8
        .TssBase                        RESQ    1


        GTMB_SIZE                       EQU     $
endstruc


;;
;; guest-status manage block
;;
struc GSMB
        .ProcessorStatus                RESD    1
        .InstructionStatus              RESD    1
        
        GSMB_SIZE                       EQU     $
endstruc





;;
;; posted-interrupt �������ṹ
;;
struc PI_DESC
        .Pir                            RESB    16
        .On:
        .Reserved                       RESB    16
endstruc



;;
;; ####### guest state area ############
;;

struc GUEST_STATE
        ;;
        ;; ***** guest register state *******
        ;;
        .Cr0                            RESQ            1
        .Cr3                            RESQ            1
        .Cr4                            RESQ            1
        .Dr7                            RESQ            1
        .Rsp                            RESQ            1
        .Rip                            RESQ            1
        .Rflags                         RESQ            1

        ;;
        ;; 16 λ selector ֵ��CS, SS, DS, ES, FS, GS, LDTR �� TR��
        ;;
        .CsSelector                     RESW            1
        .SsSelector                     RESW            1
        .DsSelector                     RESW            1
        .EsSelector                     RESW            1
        .FsSelector                     RESW            1
        .GsSelector                     RESW            1
        .LdtrSelector                   RESW            1
        .TrSelector                     RESW            1
        
        ;;
        ;; base ֵ��64 λֵ��
        ;;
        .CsBase                         RESQ            1
        .SsBase                         RESQ            1
        .DsBase                         RESQ            1
        .EsBase                         RESQ            1
        .FsBase                         RESQ            1
        .GsBase                         RESQ            1
        .LdtrBase                       RESQ            1
        .TrBase                         RESQ            1

        ;;
        ;; 32 λ Segment limit
        ;;
        .CsLimit                        RESD            1
        .SsLimit                        RESD            1
        .DsLimit                        RESD            1
        .EsLimit                        RESD            1
        .FsLimit                        RESD            1
        .GsLimit                        RESD            1
        .LdtrLimit                      RESD            1
        .TrLimit                        RESD            1

        ;;
        ;; 32 λ Access right
        ;;
        .CsAccessRight                  RESD            1
        .SsAccessRight                  RESD            1
        .DsAccessRight                  RESD            1
        .EsAccessRight                  RESD            1
        .FsAccessRight                  RESD            1
        .GsAccessRight                  RESD            1
        .LdtrAccessRight                RESD            1
        .TrAccessRight                  RESD            1

        ;;
        ;; GDTR/IDTR base��64λ�� �� limit��32λ��
        ;;
        .GdtrBase                       RESQ            1
        .IdtrBase                       RESQ            1
        .GdtrLimit                      RESD            1
        .IdtrLimit                      RESD            1


        ;;
        ;; 64 λ MSR
        ;; 
        .DebugCtlMsr                    RESQ            1
        .SysenterCsMsr                  RESQ            1
        .SysenterEspMsr                 RESQ            1
        .SysenterEipMsr                 RESQ            1
        .PerfGlobalCtlMsr               RESQ            1
        .PatMsr                         RESQ            1
        .EferMsr                        RESQ            1

        ;;
        ;; SMBASE �Ĵ���
        ;;
        .SmBase                         RESQ            1


        ;;
        ;; ******* guest non-register state *******
        ;;
        .ActivityState                  RESD            1
        .InterruptibilityState          RESD            1
        .PendingDebugException          RESQ            1
        .VmcsLinkPointer                RESQ            1
        .VmxPreemptionTimerValue        RESD            1
        .Pdpte0                         RESQ            1
        .Pdpte1                         RESQ            1
        .Pdpte2                         RESQ            1
        .Pdpte3                         RESQ            1

        ;;
        ;; guest interrupt status
        ;;
        .GuestInterruptStatus:
        .Rvi                            RESB            1
        .Svi                            RESB            1


        GUEST_STATE_SIZE                EQU             $
endstruc




;;
;; ####### host state area ############
;;

struc HOST_STATE
        ;;
        ;; 64 λ control registers
        ;;
        .Cr0                            RESQ            1
        .Cr3                            RESQ            1
        .Cr4                            RESQ            1
        .Rsp                            RESQ            1
        .Rip                            RESQ            1

        ;;
        ;; 16 λ selector ֵ��CS, SS, DS, ES, FS, GS �� TR��
        ;;
        .CsSelector                     RESW            1
        .SsSelector                     RESW            1
        .DsSelector                     RESW            1
        .EsSelector                     RESW            1
        .FsSelector                     RESW            1
        .GsSelector                     RESW            1
        .TrSelector                     RESW            1
        
        ;;
        ;; 64 λ base ֵ��FS, GS �� TR)
        ;;
        .FsBase                         RESQ            1
        .GsBase                         RESQ            1
        .TrBase                         RESQ            1


        ;;
        ;; GDTR/IDTR base��64λ��
        ;;
        .GdtrBase                       RESQ            1
        .IdtrBase                       RESQ            1


        ;;
        ;; 64 λ MSR
        ;; 
        .SysenterCsMsr                  RESQ            1
        .SysenterEspMsr                 RESQ            1
        .SysenterEipMsr                 RESQ            1
        .PerfGlobalCtlMsr               RESQ            1
        .PatMsr                         RESQ            1
        .EferMsr                        RESQ            1


        HOST_STATE_SIZE                 EQU             $
endstruc



;;
;; ####### VM-execution control fields ############
;;

struc EXECUTION_CONTROL
        .PinControl                     RESD            1
        .ProcessorControl1              RESD            1
        .ProcessorControl2              RESD            1
        .ExceptionBitmap                RESD            1
        .PfErrorCodeMask                RESD            1
        .PfErrorCodeMatch               RESD            1
        .IoBitmapAddressA               RESQ            1
        .IoBitmapAddressB               RESQ            1
        .TscOffset                      RESQ            1
        .Cr0GuestHostMask               RESQ            1
        .Cr0ReadShadow                  RESQ            1
        .Cr4GuestHostMask               RESQ            1
        .Cr4ReadShadow                  RESQ            1        
        .Cr3TargetCount                 RESD            1
        .Cr3Target0                     RESQ            1
        .Cr3Target1                     RESQ            1
        .Cr3Target2                     RESQ            1
        .Cr3Target3                     RESQ            1
        
        ;;
        ;; APIC virtualization �й���
        ;;
        .ApicAccessAddress              RESQ            1
        .VirtualApicAddress             RESQ            1
        .TprThreshold                   RESD            1
        .EoiBitmap0                     RESQ            1
        .EoiBitmap1                     RESQ            1
        .EoiBitmap2                     RESQ            1
        .EoiBitmap3                     RESQ            1
        .PostedInterruptVector          RESW            1
        ALIGNB 4
        .PostedInterruptDescriptorAddr  RESQ            1

        .MsrBitmapAddress               RESQ            1
        .ExecutiveVmcsPointer           RESQ            1
        .EptPointer                     RESQ            1
        .Vpid                           RESW            1

        ALIGNB 4
        .PleGap                         RESD            1
        .PleWindow                      RESD            1

        ALIGNB 4
        .VmFunctionControl              RESQ            1
        .EptpListAddress                RESQ            1

        
        EXECUTION_CONTROL_SIZE          EQU             $
endstruc


;;
;; ####### VM-exit control fields ############
;;
struc EXIT_CONTROL
        .VmExitControl                  RESD            1
        .MsrStoreCount                  RESD            1
        .MsrStoreAddress                RESQ            1
        .MsrLoadCount                   RESD            1
        .MsrLoadAddress                 RESQ            1

        EXIT_CONTROL_SIZE               EQU             $
endstruc


;;
;; ####### VM-entry control fields ############
;;
struc ENTRY_CONTROL
        .VmEntryControl                 RESD            1
        .MsrLoadCount                   RESD            1
        .MsrLoadAddress                 RESQ            1
        .InterruptionInfo               RESD            1
        .ExceptionErrorCode             RESD            1
        .InstructionLength              RESD            1
        
        ;;
        ;; vmware �ϵ�һ�� bug:
        ;; 1) VM-entry instruction length ��32λ�ֶ�
        ;; 2) �� vmware ��ȴ��д 64 λ!
        ;; 3) ������Ᵽ��һ�� dword ֵ
        ;;  
                                        RESD            1

        ENTRY_CONTROL_SIZE              EQU             $
endstruc



;;
;; ####### VM-exit information fields ############
;;
struc EXIT_INFO
        ;;
        ;; ������Ϣ
        ;;
        .ExitReason                     RESD            1
        .ExitQualification              RESQ            1
        .GuestLinearAddress             RESQ            1
        .GuestPhysicalAddress           RESQ            1

        ;;
        ;; �� event ���� VM exit ����Ϣ
        ;;
        .InterruptionInfo               RESD            1
        .InterruptionErrorCode          RESD            1        

        ;;
        ;; �� event delivery �ڼ����� VM exit ����Ϣ
        ;;
        .IdtVectoringInfo               RESD            1
        .IdtVectoringErrorCode          RESD            1

        ;;
        ;; ��ָ��ִ�ж����� VM exit ����Ϣ
        ;;
        .InstructionLength              RESD            1  
        .InstructionInfo                RESD            1

        .IoRcx                          RESQ            1
        .IoRsi                          RESQ            1
        .IoRdi                          RESQ            1
        .IoRip                          RESQ            1

        ;;
        ;; VMX ָ�������Ϣ
        ;;
        .InstructionError               RESD            1


        EXIT_INFO_SIZE                  EQU             $
endstruc







;-------------------------------------------------------
; SET_PINBASED_CTLS
; input:
;       %1 - control bit
; ����:
;       1���� Pinbased VM-execution control λ
;-------------------------------------------------------
%macro SET_PINBASED_CTLS 1
        GetVmcsField    CONTROL_PINBASED
        or eax, %1

%if __BITS__ == 64
        or eax, [gs: PCB.PinBasedCtls]
        and eax, [gs: PCB.PinBasedCtls + 4]
%else
        mov esi, PCB.PinBasedCtls
        or eax, [gs: esi]
        and eax, [gs: esi + 4]  
%endif
        SetVmcsField    CONTROL_PINBASED, eax
%endmacro



;-------------------------------------------------------
; CLEAR_PINBASED_CTLS
; input:
;       %1 - control bit
; ����:
;       1���� Pinbased VM-execution control λ
;-------------------------------------------------------
%macro CLEAR_PINBASED_CTLS 1
        GetVmcsField    CONTROL_PINBASED
        and eax, ~%1

%if __BITS__ == 64
        or eax, [gs: PCB.PinBasedCtls]
        and eax, [gs: PCB.PinBasedCtls + 4]
%else
        mov esi, PCB.PinBasedCtls
        or eax, [gs: esi]
        and eax, [gs: esi + 4]
%endif
        SetVmcsField    CONTROL_PINBASED, eax
%endmacro



;-------------------------------------------------------
; SET_PRIMARAY_PROCBASED_CTLS
; input:
;       %1 - control bit
; ����:
;       1���� primary processor-based VM-execution control λ
;-------------------------------------------------------
%macro SET_PRIMARY_PROCBASED_CTLS 1
        GetVmcsField    CONTROL_PROCBASED_PRIMARY
        or eax, %1

%if __BITS__ == 64
        or eax, [gs: PCB.ProcessorBasedCtls]
        and eax, [gs: PCB.ProcessorBasedCtls + 4]
%else
        mov esi, PCB.ProcessorBasedCtls
        or eax, [gs: esi]
        and eax, [gs: esi + 4]
%endif
        SetVmcsField    CONTROL_PROCBASED_PRIMARY, eax
%endmacro



;-------------------------------------------------------
; CLEAR_PRIMARY_PROCBASED_CTLS
; input:
;       %1 - control bit
; ����:
;       1���� primary processor-based VM-execution control λ
;-------------------------------------------------------
%macro CLEAR_PRIMARY_PROCBASED_CTLS 1
        GetVmcsField    CONTROL_PROCBASED_PRIMARY
        and eax, ~%1

%if __BITS__ == 64
        or eax, [gs: PCB.ProcessorBasedCtls]
        and eax, [gs: PCB.ProcessorBasedCtls + 4]
%else
        mov esi, PCB.ProcessorBasedCtls
        or eax, [gs: esi]
        and eax, [gs: esi + 4]
%endif
        SetVmcsField    CONTROL_PROCBASED_PRIMARY, eax
%endmacro



;-------------------------------------------------------
; SET_SECONDARY_PROCBASED_CTLS
; input:
;       %1 - control bit
; ����:
;       1���� secondary processor-based VM-execution control λ
;-------------------------------------------------------
%macro SET_SECONDARY_PROCBASED_CTLS 1
        GetVmcsField    CONTROL_PROCBASED_SECONDARY
        or eax, %1

%if __BITS__ == 64
        or eax, [gs: PCB.ProcessorBasedCtls2]
        and eax, [gs: PCB.ProcessorBasedCtls2 + 4]
%else
        mov esi, PCB.ProcessorBasedCtls2
        or eax, [gs: esi]
        and eax, [gs: esi + 4]
%endif
        SetVmcsField    CONTROL_PROCBASED_SECONDARY, eax
%endmacro


;-------------------------------------------------------
; CLEAR_SECONDARY_PROCBASED_CTLS
; input:
;       %1 - control bit
; ����:
;       1���� secondary processor-based VM-execution control λ
;-------------------------------------------------------
%macro CLEAR_SECONDARY_PROCBASED_CTLS 1
        GetVmcsField    CONTROL_PROCBASED_SECONDARY
        and eax, ~%1

%if __BITS__ == 64
        or eax, [gs: PCB.ProcessorBasedCtls2]
        and eax, [gs: PCB.ProcessorBasedCtls2 + 4]
%else
        mov esi, PCB.ProcessorBasedCtls2
        or eax, [gs: esi]
        and eax, [gs: esi + 4]
%endif
        SetVmcsField    CONTROL_PROCBASED_SECONDARY, eax
%endmacro


;-------------------------------------------------------
; SET_VM_EXIT_CTLS
; input:
;       %1 - control bit
; ����:
;       1���� VM-exit control λ
;-------------------------------------------------------
%macro SET_VM_EXIT_CTLS 1
        GetVmcsField    VMEXIT_CONTROL
        or eax, %1

%if __BITS__ == 64
        or eax, [gs: PCB.ExitCtls]
        and eax, [gs: PCB.ExitCtls + 4]
%else
        mov esi, PCB.ExitCtls
        or eax, [gs: esi]
        and eax, [gs: esi + 4]
%endif
        SetVmcsField    VMEXIT_CONTROL, eax
%endmacro


;-------------------------------------------------------
; CLEAR_VM_EXIT_CTLS
; input:
;       %1 - control bit
; ����:
;       1���� VM-exit control λ
;-------------------------------------------------------
%macro CLEAR_VM_EXIT_CTLS 1
        GetVmcsField    VMEXIT_CONTROL
        and eax, ~%1

%if __BITS__ == 64
        or eax, [gs: PCB.ExitCtls]
        and eax, [gs: PCB.ExitCtls + 4]
%else
        mov esi, PCB.ExitCtls
        or eax, [gs: esi]
        and eax, [gs: esi + 4]
%endif
        SetVmcsField    VMEXIT_CONTROL, eax
%endmacro


;-------------------------------------------------------
; SET_VM_ENTRY_CTLS
; input:
;       %1 - control bit
; ����:
;       1���� VM-entry control λ
;-------------------------------------------------------
%macro SET_VM_ENTRY_CTLS 1
        GetVmcsField    VMENTRY_CONTROL
        or eax, %1

%if __BITS__ == 64
        or eax, [gs: PCB.EntryCtls]
        and eax, [gs: PCB.EntryCtls + 4]
%else
        mov esi, PCB.EntryCtls
        or eax, [gs: esi]
        and eax, [gs: esi + 4]        
%endif

        SetVmcsField    VMENTRY_CONTROL, eax
%endmacro


;-------------------------------------------------------
; CLEAR_VM_ENTRY_CTLS
; input:
;       %1 - control bit
; ����:
;       1���� VM-entry control λ
;-------------------------------------------------------
%macro CLEAR_VM_ENTRY_CTLS 1
        GetVmcsField    VMENTRY_CONTROL
        and eax, ~%1

%if __BITS__ == 64
        or eax, [gs: PCB.EntryCtls]
        and eax, [gs: PCB.EntryCtls + 4]
%else
        mov esi, PCB.EntryCtls
        or eax, [gs: esi]
        and eax, [gs: esi + 4]
%endif
        SetVmcsField    VMENTRY_CONTROL, eax
%endmacro




;-------------------------------------------------------
; SET_EXCEPTION_BITMAP
; input:
;       %1 - vector
; ������
;       1) ���� Exception Bitmap λ
;-------------------------------------------------------
%macro SET_EXCEPTION_BITMAP 1
        GetVmcsField    CONTROL_EXCEPTION_BITMAP
        or eax, (1 << %1)
        SetVmcsField    CONTROL_EXCEPTION_BITMAP, eax
%endmacro


;-------------------------------------------------------
; CLEAR_EXCEPTION_BITMAP
; input:
;       %1 - vector
; ������
;       1) �� Exception Bitmap λ
;-------------------------------------------------------
%macro CLEAR_EXCEPTION_BITMAP 1
        GetVmcsField    CONTROL_EXCEPTION_BITMAP
        and eax, ~(1 << %1)
        SetVmcsField    CONTROL_EXCEPTION_BITMAP, eax
%endmacro



;-------------------------------------------------------
; MAKE_SEGMENT_UNUSABLE
; input:
;       %1 - vector
; ������
;       1) �� Exception Bitmap λ
;-------------------------------------------------------
%macro MAKE_SEGMENT_UNUSABLE    1
        GetVmcsField    %1
        or eax, SEG_U
        SetVmcsField    %1, eax     
%endmacro


%endif
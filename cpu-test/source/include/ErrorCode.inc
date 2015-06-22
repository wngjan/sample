;*************************************************
;* ErrorCode.inc                                 *
;* Copyright (c) 2009-2013 ��־                  *
;* All rights reserved.                          *
;*************************************************

%ifndef __ERRORCODE_INC
%define __ERRORCODE_INC

%define STATUS_SUCCESS                                  0
%define STATUS_UNSUCCESS                                1
%define STATUS_NO_RESOURCE                              80000001h
%define STATUS_PROCESSOR_INDEX_EXCEED                   80010001h

%define STATUS_NO_FOCUS                                 80020002h


;;
;; VMX ����Ĵ�����
;;
%define STATUS_VMXON_UNSUCCESS                          80020001h
%define STATUS_VMXOFF_UNSUCCESS                         80020002h
%define STATUS_VMX_UNEXPECT                             80020003h

;;
;; guest ����Ĵ���
;;
%define STATUS_GUEST_PAGING_ERROR                       81020001h


%endif
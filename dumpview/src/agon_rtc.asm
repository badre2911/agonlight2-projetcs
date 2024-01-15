;
; Title:	agon_rtc.asm
; Author:	Badre
; Created:	23/12/2023
;

include '../../lib/libc/mos_api.inc'
	
	section	.text,"ax",@progbits
	assume adl = 1        

	public	_getrtc
    
_getrtc:
    call _set0_sysvar_vpd_pflags    
    ld  a, 23
    call putc
    ld  a, 0
    call putc
    ld  a, 7
    call putc 
    ld  a, 0
    call putc   

_wait_set_flag_rtc:
    call _get_sysvar_vpd_pflags
    and  20h
    jr  z,  _wait_set_flag_rtc 
    ret

; Set pflahgs to 0
_set0_sysvar_vpd_pflags:
	push	ix
	ld		a, mos_sysvars
	rst.lil	08h
    ld      a, 00h
	ld	    (ix+sysvar_vpd_pflags), a 
	pop		ix
	ret

;Get sysvar pFlags
_get_sysvar_vpd_pflags:
	push	ix
	ld		a, mos_sysvars
	rst.lil	08h
	ld		a, (ix+sysvar_vpd_pflags)
	pop		ix
	ret

putc:
    rst.lil $10
    ret

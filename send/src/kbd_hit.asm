;
; Title:	kbd_hit.asm
; Author:	Badre
; Created:	23/12/2023
; Last Updated: 17/01/2024
; 
; Modinfo:
; 17/01/2024   Added kbd_code and read_key
;

include '../../lib/libc/mos_api.inc'
	
	section	.text,"ax",@progbits
	assume adl = 1        
			
	public	_kbd_hit
	public	_kbd_ascii
	public  _read_key
	public	_kbd_code
	
_kbd_hit:
	push ix
	ld a, mos_sysvars		; MOS call for mos_sysvars
	rst.lil 08h				; returns pointer to sysvars in ixu

	ld a, (ix+sysvar_vkeydown)
	ld l,a	
	pop ix
	ret				; return l as h:keyascii
	
_kbd_ascii:
	push ix
	ld a, mos_sysvars		; MOS call for mos_sysvars
	rst.lil 08h				; returns pointer to sysvars in ixu
	
	lea	hl, ix+sysvar_keyascii
    ld	a, (hl)	
	pop ix
	ret				; return l as h:keyascii	

_kbd_code:
	push ix
	ld a, mos_sysvars		; MOS call for mos_sysvars
	rst.lil 08h				; returns pointer to sysvars in ixu

	lea	hl, ix+sysvar_vkeycode
    ld	a, (hl)			
	pop ix
	ret				; return l as h:keyascii


_read_key:
        push    ix    
Read_Key:
        ld      a, mos_getkey
        rst.LIS 08h           		        
        or      A
		jr 	    z, Read_Key             ; Is 'ascii' code 0?		
        pop     ix
        ret

        

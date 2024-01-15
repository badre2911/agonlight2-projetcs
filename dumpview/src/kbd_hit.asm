;
; Title:	kbd_hit.asm
; Author:	Badre
; Created:	23/12/2023
;

include '../../lib/libc/mos_api.inc'
	
	section	.text,"ax",@progbits
	assume adl = 1        
			
	public	_kbd_hit
	public	_kbd_ascii
	
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

	ld a, (ix+sysvar_keyascii)
	ld l,a	
	pop ix
	ret				; return l as h:keyascii	
        

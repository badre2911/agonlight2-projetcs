;
; Title:	srcmode.asm
; Author:	Badre
; Created:	23/12/2023
;

include '../../lib/libc/mos_api.inc'
	
	section	.text,"ax",@progbits
	assume adl = 1        

	public	_saveScrMmode
    public	_setSavedScrMode


_saveScrMmode:
	push	ix
	ld		a, mos_sysvars			; MOS Call for mos_sysvars
	rst.lil	08h					; returns pointer to sysvars in ixu
	ld		a, (ix+27h)
    ld      (_original_scr_mode), a
	pop		ix
	ret

_setSavedScrMode:
	ld     a, 22
    call putc
    ld      a, (_original_scr_mode)
    call putc
	ret

putc:
	rst.lil $10
	ret

_original_scr_mode  db 0

        

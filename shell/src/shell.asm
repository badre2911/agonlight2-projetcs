;
; Title:	shell.asm
; Author:	Badre
; Created:	15/03/2024
;

include '../../lib/libc/mos_api.inc'
	
	section	.text,"ax",@progbits
	assume adl = 1        

	public	_shell


_shell:
	push ix    
    push bc
    push hl
    push de

    ld      ix,0
    add     ix, sp
    ld      hl, (ix+15)
    ld      de, 0
    ld      bc, 0
    MOSCALL mos_oscli

    ld      sp, ix
    pop de
    pop hl
    pop bc   
	pop ix
	ret				; return l as h:keyascii

macro 	MOSCALL func
		ld a, func
		rst.lil $08
		end macro	


        

;
; Title:		Main
; Author:	Badre
; Created:	14/01/2023
;
;
; Macro for calling the API
; Parameters:
; - function: One of the function numbers listed above
;
MOSCALL:	MACRO	function
			LD	A, 	function
			RST.LIS	08h
			ENDMACRO 	
        
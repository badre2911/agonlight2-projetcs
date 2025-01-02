;
; Title:	serial.asm
; Author:	Badre
; Created:	24/06/2024
;
UART0_PORT		:=	0xC0		; UART0
UART1_PORT		:=	0xD0		; UART1

UART0_REG_RBR		EQU	UART0_PORT+0		; Receive buffer
UART0_REG_THR		EQU	UART0_PORT+0		; Transmitter holding
UART0_REG_DLL		EQU	UART0_PORT+0		; Divisor latch low
UART0_REG_IER		EQU	UART0_PORT+1		; Interrupt enable
UART0_REG_DLH		EQU	UART0_PORT+1		; Divisor latch high
UART0_REG_IIR		EQU	UART0_PORT+2		; Interrupt identification
UART0_REG_FCT		EQU	UART0_PORT+2;		; Flow control
UART0_REG_LCR		EQU	UART0_PORT+3		; Line control
UART0_REG_MCR		EQU	UART0_PORT+4		; Modem control
UART0_REG_LSR		EQU	UART0_PORT+5		; Line status
UART0_REG_MSR		EQU	UART0_PORT+6		; Modem status
UART0_REG_SCR		EQU UART0_PORT+7		; Scratch

UART1_REG_RBR		EQU	UART1_PORT+0		; Receive buffer
UART1_REG_THR		EQU	UART1_PORT+0		; Transmitter holding
UART1_REG_DLL		EQU	UART1_PORT+0		; Divisor latch low
UART1_REG_IER		EQU	UART1_PORT+1		; Interrupt enable
UART1_REG_DLH		EQU	UART1_PORT+1		; Divisor latch high
UART1_REG_IIR		EQU	UART1_PORT+2		; Interrupt identification
UART1_REG_FCT		EQU	UART1_PORT+2		; Flow control
UART1_REG_LCR		EQU	UART1_PORT+3		; Line control
UART1_REG_MCR		EQU	UART1_PORT+4		; Modem control
UART1_REG_LSR		EQU	UART1_PORT+5		; Line status
UART1_REG_MSR		EQU	UART1_PORT+6		; Modem status
UART1_REG_SCR		EQU UART1_PORT+7		; Scratch

TX_WAIT		EQU	16384 		; Count before a TX times out

UART_LSR_ERR	:=  0x80			; Error
UART_LSR_ETX	:=  0x40			; Transmit empty
UART_LSR_ETH	:=	0x20			; Transmit holding register empty
UART_LSR_RDY	:=	0x01			; Data ready


			ASSUME	ADL = 1
			SECTION .TEXT
			
; Write a byte to UART1
; A: Data byte to write
; Returns:
; A = 0 if timed out
; A = 1 if written
			PUBLIC _uart0_putch
			PUBLIC _uart0_handler
			PUBLIC _uart0_kbhit
			PUBLIC _uart0_flush
			PUBLIC _uart0_read
			PUBLIC _uart1_putch
			PUBLIC _uart1_handler
			PUBLIC _uart1_kbhit
			PUBLIC _uart1_flush
			PUBLIC _uart1_read

_uart0_handler:		
			DI
			PUSH	AF			
			IN0		A,(UART0_REG_LSR)			; Get the line status register
			AND		UART_LSR_RDY		; Check for characters in buffer
			JR		Z, _uart0_no_receive				; Nothing received
			LD		A,1
			LD		(uart0_received),A			
			IN0		A,(UART0_REG_RBR)			; Read the character from the UART receive buffer
			LD		(uart0_buffer),A
			POP		AF
			EI
			RETI.L
_uart0_no_receive:			; No character received
			XOR 	A,A			
			LD		(uart0_received),A			
			POP		AF
			EI
			RETI.L

_uart1_handler:		
			DI
			PUSH	AF			
			IN0		A,(UART1_REG_LSR)			; Get the line status register
			AND		UART_LSR_RDY		; Check for characters in buffer
			JR		Z, no_receive				; Nothing received
			LD		A,1
			LD		(uart1_received),A			
			IN0		A,(UART1_REG_RBR)			; Read the character from the UART receive buffer
			LD		(uart1_buffer),A
			POP		AF
			EI
			RETI.L
no_receive:			; No character received
			XOR 	A,A			
			LD		(uart1_received),A			
			POP		AF
			EI
			RETI.L

_uart0_putch:
			PUSH	BC			; Stack BC
			PUSH	AF 			; Stack AF
			LD		BC,TX_WAIT	; Set CB to the transmit timeout
_uart0_loop:		IN0		A,(UART0_REG_LSR)	; Get the line status register
			AND 	UART_LSR_ETX; Check for TX empty
			JR		NZ, _uart0_tx		; If set, then TX is empty, goto transmit
			DEC		BC
			LD		A, B
			OR		C
			JR		NZ, _uart0_loop
			POP		AF			; We've timed out at this point so
			POP		BC			; Restore the stack
			XOR		A,A			; Return timeout
			RET	
_uart0_tx:	POP		AF			; Good to send at this point, so
			OUT0	(UART0_REG_THR),A	; Write the character to the UART transmit buffer
			POP		BC			; Restore BC
			LD		A,1			; Return succes, also works when A was 0 originally
			RET 

; Blocking read a byte from UART0
; Returns:
; A = byte read
			PUBLIC _uart0_getch
_uart0_getch:
			LD		A,(uart0_received)
			OR		A
			JR		Z, _uart0_getch
			DI		; atomic acknowledge byte read
			XOR		A,A
			LD		(uart0_received),A
			LD		A,(uart0_buffer)
			EI
			RET

_uart0_kbhit:			
			LD		A,(uart0_received) 	
			AND		A
			RET		Z			
			LD		A,1	
			RET

_uart0_read:			
			LD		A,(uart0_received)
			OR		A
			RET		Z
			DI		; atomic acknowledge byte read
			XOR		A,A
			LD		(uart0_received),A;
			LD		A,(uart0_buffer)
			EI
			RET

_uart0_flush:			
uart0_flush:	CALL 	_uart0_kbhit
				OR		A
				JR		Z, uart0_endflush
				DI		; atomic acknowledge byte read
				XOR		A,A
				LD		(uart0_received),A;			
				LD		A,(uart0_buffer)
				EI
				JP		uart0_flush		
uart0_endflush:	RET


_uart1_putch:
			PUSH	BC			; Stack BC
			PUSH	AF 			; Stack AF
			LD		BC,TX_WAIT	; Set CB to the transmit timeout
loop:		IN0		A,(UART1_REG_LSR)	; Get the line status register
			AND 	UART_LSR_ETX; Check for TX empty
			JR		NZ, tx		; If set, then TX is empty, goto transmit
			DEC		BC
			LD		A, B
			OR		C
			JR		NZ, loop
			POP		AF			; We've timed out at this point so
			POP		BC			; Restore the stack
			XOR		A,A			; Return timeout
			RET	
tx:			POP		AF			; Good to send at this point, so
			OUT0	(UART1_REG_THR),A	; Write the character to the UART transmit buffer
			POP		BC			; Restore BC
			LD		A,1			; Return succes, also works when A was 0 originally
			RET 

; Blocking read a byte from UART1
; Returns:
; A = byte read
			PUBLIC _uart1_getch
_uart1_getch:
			LD		A,(uart1_received)
			OR		A
			JR		Z, _uart1_getch
			DI		; atomic acknowledge byte read
			XOR		A,A
			LD		(uart1_received),A
			LD		A,(uart1_buffer)
			EI
			RET

_uart1_kbhit:			
			LD		A,(uart1_received) 	
			OR		A
			RET		Z			
			LD		A,1	
			RET

_uart1_read:			
			LD		A,(uart1_received)
			OR		A
			RET		Z
			DI		; atomic acknowledge byte read
			XOR		A,A
			LD		(uart1_received),A;
			LD		A,(uart1_buffer)						
			EI
			RET

_uart1_flush:			
	CALL 	_uart1_kbhit
	OR		A
	JP		Z, endflush
	call    _uart1_read
;	DI		; atomic acknowledge byte read
;	XOR		A,A
;	LD		(uart1_received),A;			
;	LD		A,(uart1_buffer)
;	EI
	JP		_uart1_flush			
endflush:	
	RET

			
uart0_buffer:		
DB	1	; 64 byte receive buffer
uart0_received:		
DB	1	; boolean
uart1_buffer:		
DB	1	; 64 byte receive buffer
uart1_received:		
DB	1	; boolean

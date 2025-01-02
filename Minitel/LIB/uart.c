/*
 Title:		uart.c
 Author:	Badre
 Created:	23/12/2023 
 Last Updated: 22/01/2024

 Modinfo:
 22/01/2024   add _uartx_writeZ
 
*/
#include "uart.h"

#define PORTPIN_ZERO		0x01			//!< The bit mask for Port bit 0.
#define PORTPIN_ONE			0x02			//!< The bit mask for Port bit 1.
#define PORTPIN_TWO			0x04			//!< The bit mask for Port bit 2.
#define PORTPIN_THREE		0x08			//!< The bit mask for Port bit 3.

//UART0
#define PD_DR 		0xA2
#define PD_DDR 		0xA3
#define PD_ALT1 	0xA4
#define PD_ALT2 	0xA5
#define UART0_LCTL 	0xC3
#define UART0_BRG_L 0xC0
#define UART0_BRG_H 0xC1
#define UART0_MCTL 	0xC4
#define UART0_FCTL 	0xC2
#define UART0_IER 	0xC1

//UART1
#define PC_DR 0x9E
#define PC_DDR 0x9F
#define PC_ALT1 0xA0
#define PC_ALT2 0xA1
#define UART1_LCTL 0xD3
#define UART1_BRG_L 0xD0
#define UART1_BRG_H 0xD1
#define UART1_MCTL 0xD4
#define UART1_FCTL 0xD2
#define UART1_IER 0xD1

/* UART_LCTL bits */
/*#define UART_LCTL_DLAB					((unsigned char)0x80)		//!< DLAB bit in LCTL.
#define UART_LCTL_SB					((unsigned char)0x40)		//!< Send Break bit in LCTL.
#define UART_LCTL_FPE					((unsigned char)0x20)		//!< Force Parity Error bit in LCTL.
#define UART_LCTL_EPS					((unsigned char)0x10)		//!< Even Parity Select bit in LCTL.
#define UART_LCTL_PEN					((unsigned char)0x08)		//!< Parity Enable bit in LCTL.
#define UART_LCTL_2STOPBITS				((unsigned char)0x04)
#define UART_LCTL_5DATABITS				((unsigned char)0x00)
#define UART_LCTL_6DATABITS				((unsigned char)0x01)
#define UART_LCTL_7DATABITS				((unsigned char)0x02)
#define UART_LCTL_8DATABITS				((unsigned char)0x03)*/

#define	RESETREG( x, y )	( (x) &= (0xFF ^ (y)) )
#define SETREG( x, y )		( (x) |= (y) )

// Set the Line Control Register for data, stop and parity bits
//
// UART1_LCTL
#define SETREG_LCR1(out, data, stop, parity) (out = ((char)(((data)-(char)5)&(char)0x3)|(char)((((stop)-(char)0x1)&(char)0x1)<<(char)0x2)|(char)((parity)<<(char)0x3)))
// UART0_LCTL
#define SETREG_LCR0(out, data, stop, parity) (out = ((char)(((data)-(char)5)&(char)0x3)|(char)((((stop)-(char)0x1)&(char)0x1)<<(char)0x2)|(char)((parity)<<(char)0x3)))
// Open UART
// Parameters:
// - pUART: Structure containing the initialisation data
//



unsigned char get_port(uint8_t port) {
    unsigned char output;
    __asm__ volatile (
        "ld b, 0 \n"
        "ld c, %1 \n"
        "in a, (c) \n"
        "ld %0, a"
        : "=d"(output)
        : "d"(port)
        : "cc", "memory", "b", "c", "a"
    );
    return output;
}


void set_port(uint8_t port, uint8_t value) {
    __asm__ volatile (
        "ld b, 0 \n"
        "ld a, %1 \n"
        "ld c, %0 \n"
        "out (c), a"
        :
        : "r"(port), "r"(value)
        : "cc", "memory", "b", "c", "a"
    );
}

void init_UART1() {
	__asm__ volatile (
		"ld a, 0xff\n"
		"out0	(0x9E), a\n"
		"out0	(0x9F), a\n"
		"ld a, 0\n"
		"out0	(0xA0), a\n"
		"out0	(0xA1), a\n"
	);
}

void init_UART0() {
	__asm__ volatile (
		"ld a, 0xff\n"
		"out0	(0xA2), a\n"
		"out0	(0xA3), a\n"
		"ld a, 0\n"
		"out0	(0xA4), a\n"
		"out0	(0xA5), a\n"
	);
}

uint8_t open_UART0(xUART *pUART) {
	uint16_t	br = MASTERCLOCK / (CLOCK_DIVISOR_16 * pUART->baudRate);//! Compute the baudrate generator value;
	uint8_t	pins = PORTPIN_ZERO | PORTPIN_ONE;						//! The transmit and receive pins
	// pins = 0x01 | 0x02

	unsigned char pd_ddr = get_port(PD_DDR);
	SETREG(pd_ddr, pins);
	set_port(PD_DDR, pd_ddr);

	unsigned char pd_alt1 = get_port(PD_ALT1);
	RESETREG(pd_alt1, pins);
	set_port(PD_ALT1, pd_alt1);

	unsigned char pd_alt2 = get_port(PD_ALT2);
	SETREG(pd_alt2, pins);
	set_port(PD_ALT2, pd_alt2);

	pd_ddr = get_port(PD_DDR);
	SETREG(pd_ddr, PORTPIN_THREE);									//! Set Port C bit 3 (CTS) for input
	set_port(PD_DDR, pd_ddr);

	pd_alt1 = get_port(PD_ALT1);
	RESETREG(pd_alt1, PORTPIN_THREE);
	set_port(PD_ALT1, pd_alt1);
	
	pd_alt2 = get_port(PD_ALT2);
	RESETREG(pd_alt2, PORTPIN_THREE);
	set_port(PD_ALT2, pd_alt2);
	
	unsigned char uart0_lctl = get_port(UART0_LCTL);
	uart0_lctl |= UART_LCTL_DLAB ;									//! Select DLAB to access baud rate generators
	set_port(UART0_LCTL, uart0_lctl);

	unsigned char uart0_brg_l = get_port(UART0_BRG_L); 
	uart0_brg_l = (br & 0xFF) ;										//! Load divisor low
	set_port(UART0_BRG_L, uart0_brg_l);
	
	unsigned char uart0_brg_h = get_port(UART0_BRG_H);
	uart0_brg_h = (char)(( br & 0xFF00 ) >> 8) ;					//! Load divisor high
	set_port(UART0_BRG_H, uart0_brg_h);

	uart0_lctl = get_port(UART0_LCTL);
	uart0_lctl &= (~UART_LCTL_DLAB) ; 								//! Reset DLAB; dont disturb other bits
	set_port(UART0_LCTL, uart0_lctl);

	set_port(UART0_MCTL, pUART->hwFlowControl);
	
	set_port (UART0_FCTL, 0x07);												//! Disable hardware FIFOs.
	
	set_port(UART0_IER, pUART->interrupts);
	//set_port(UART0_IER, 0);

	unsigned char lcr0;
	SETREG_LCR0(lcr0, pUART->dataBits, pUART->stopBits, pUART->parity);	//! Set the line status register.
	set_port(UART0_LCTL, lcr0);

	return 0 ;
}

uint8_t open_UART1(xUART *pUART) {
	uint16_t	br = MASTERCLOCK / (CLOCK_DIVISOR_16 * pUART->baudRate);//! Compute the baudrate generator value;
	uint8_t	pins = PORTPIN_ZERO | PORTPIN_ONE;						//! The transmit and receive pins
	// pins = 0x01 | 0x02

	unsigned char pc_ddr = get_port(PC_DDR);
	SETREG(pc_ddr, pins);
	set_port(PC_DDR, pc_ddr);

	unsigned char pc_alt1 = get_port(PC_ALT1);
	RESETREG(pc_alt1, pins);
	set_port(PC_ALT1, pc_alt1);

	unsigned char pc_alt2 = get_port(PC_ALT2);
	SETREG(pc_alt2, pins);
	set_port(PC_ALT2, pc_alt2);

	pc_ddr = get_port(PC_DDR);
	SETREG(pc_ddr, PORTPIN_THREE);									//! Set Port C bit 3 (CTS) for input
	set_port(PC_DDR, pc_ddr);

	pc_alt1 = get_port(PC_ALT1);
	RESETREG(pc_alt1, PORTPIN_THREE);
	set_port(PC_ALT1, pc_alt1);
	
	pc_alt2 = get_port(PC_ALT2);
	RESETREG(pc_alt2, PORTPIN_THREE);
	set_port(PC_ALT2, pc_alt2);
	
	unsigned char uart1_lctl = get_port(UART1_LCTL);
	uart1_lctl |= UART_LCTL_DLAB ;									//! Select DLAB to access baud rate generators
	set_port(UART1_LCTL, uart1_lctl);

	unsigned char uart1_brg_l = get_port(UART1_BRG_L); 
	uart1_brg_l = (br & 0xFF) ;										//! Load divisor low
	set_port(UART1_BRG_L, uart1_brg_l);
	
	unsigned char uart1_brg_h = get_port(UART1_BRG_H);
	uart1_brg_h = (char)(( br & 0xFF00 ) >> 8) ;					//! Load divisor high
	set_port(UART1_BRG_H, uart1_brg_h);

	uart1_lctl = get_port(UART1_LCTL);
	uart1_lctl &= (~UART_LCTL_DLAB) ; 								//! Reset DLAB; dont disturb other bits
	set_port(UART1_LCTL, uart1_lctl);

	set_port(UART1_MCTL, pUART->hwFlowControl);
	
	set_port (UART1_FCTL, 0x07);												//! Disable hardware FIFOs.
	
	set_port(UART1_IER, pUART->interrupts);
	//set_port(UART1_IER, 0);

	unsigned char lcr1;
	SETREG_LCR1(lcr1, pUART->dataBits, pUART->stopBits, pUART->parity);	//! Set the line status register.
	set_port(UART1_LCTL, lcr1);

	return 0 ;
}

void close_UART0( void ) {
	set_port(UART0_IER, 0);
	//UART1_IER = 0 ;													//! Disable all UART1 interrupts.
	
	set_port(UART0_LCTL,0) ;												//! Bring line control register to reset value.
	set_port(UART0_MCTL, 0x00) ;												//! Bring modem control register to reset value.
	set_port(UART0_FCTL, 0) ;												//! Bring FIFO control register to reset value.
	return ;
}

void close_UART1( void ) {
	set_port(UART1_IER, 0);
	//UART1_IER = 0 ;													//! Disable all UART1 interrupts.
	
	set_port(UART1_LCTL,0) ;												//! Bring line control register to reset value.
	set_port(UART1_MCTL, 0x00) ;												//! Bring modem control register to reset value.
	set_port(UART1_FCTL, 0) ;												//! Bring FIFO control register to reset value.
	return ;
}

void uart0_puts(char *str)
{
	while(*str)	uart0_putch(*str++);
}

void uart1_puts(char *str)
{
	while(*str)	uart1_putch(*str++);
}

#include <mos_api.h>
#include <stdbool.h>
#include "agon_timer.h"

volatile void *timer0_prevhandler;
volatile uint24_t ticker = 0;
extern void timer_handler();

void on_tick()
{
	ticker++;
}

uint24_t gettick()
{
    return ticker;
}

/*static unsigned char get_port(uint8_t port) {
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
}*/

static void set_port(uint8_t port, uint8_t value) {
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

void timer0_begin(uint16_t reload_value, uint16_t clk_divider) {

	//timer0_period (in SECONDS) = (reload_value * clk_divider) / system_clock_frequency (which is 18432000 Hz)
   
	unsigned char clkbits = 0;
    unsigned char ctl;

    timer0_prevhandler = mos_setintvector(PRT0_IVECT, timer_handler);

    switch (clk_divider) {
        case 4:   clkbits = 0x00; break;
        case 16:  clkbits = 0x04; break;
        case 64:  clkbits = 0x08; break;
        case 256: clkbits = 0x0C; break;
    }
    ctl = 0x53 | clkbits; // Continuous mode, reload and restart enabled, and enable the timer    

    set_port(TMR0_CTL, 0x00); // Disable the timer and clear all settings
	ticker = 0;
    set_port(TMR0_RR_L, (unsigned char)(reload_value));
    set_port(TMR0_RR_H, (unsigned char)(reload_value >> 8));
    set_port(TMR0_CTL, ctl);

}

void timer0_end() {
	set_port(TMR0_CTL, 0x00);
	set_port(TMR0_RR_L, 0x00);
	set_port(TMR0_RR_H, 0x00);
	mos_setintvector(PRT0_IVECT, timer0_prevhandler);
	ticker = 0;
}

void delayms(uint24_t timeout)
{
    ticker = 0;
	timer0_begin(4608, 4);	
	uint24_t old_ticker = ticker;

    while(1) 
    {
        if((ticker - old_ticker) >= timeout) break;	
    }
    timer0_end();
}

void delay_cents(uint16_t ticks_end) { //100ms ticks	
	uint16_t ticks = 0;
	ticks_end *= 6;
	while(true) {
		
		waitvblank();
		ticks++;
		if(ticks >= ticks_end) break;		
	}	
}

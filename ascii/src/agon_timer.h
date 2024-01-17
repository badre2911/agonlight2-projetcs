#ifndef _AGON_TIMER_H
#define _AGON_TIMER_H

#include <stdint.h>


#define TMR0_CTL		0x80
#define TMR0_DR_L		0x81
#define TMR0_RR_L		0x81
#define TMR0_DR_H		0x82
#define TMR0_RR_H		0x82
#define PRT0_IVECT		0x0A



extern volatile uint24_t ticker;

void on_tick();
void timer0_begin(uint16_t reload_value, uint16_t clk_divider); //4608 * 4 / 18432000 = 1ms
void timer0_end();
void delayms(uint24_t timeout);
void delay_cents(uint16_t ticks_end);
uint24_t gettick();

#endif
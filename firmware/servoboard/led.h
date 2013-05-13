#ifndef LED_H
#define LED_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include "twi.h"
#include "servo.h"

#define led1_aus PORTD|=(1<<PD2)
#define led2_aus PORTD|=(1<<PD3)
#define led3_aus PORTD|=(1<<PD4)
#define led1_an PORTD&=~(1<<PD2)
#define led2_an PORTD&=~(1<<PD3)
#define led3_an PORTD&=~(1<<PD4)
#define led1_toggle PORTD = PORTD ^ (1<<PD2)
#define led2_toggle PORTD = PORTD ^ (1<<PD3)

extern uint8_t led_controlled_by_user;

void led_init(void);
void led_update(void);

#endif

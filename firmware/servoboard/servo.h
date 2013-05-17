#ifndef SERVO_H
#define SERVO_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define servos_on PORTB|=(1<<PB0)
#define servos_off PORTB&=~(1<<PB0)
#define check_servo_power (PORTB&(1<<PB0))

#define MILLISECONDBASE 8000

#define SERVO_MAX_VALUE 8000

extern uint16_t servos_angular[8];

void servo_init(void);

#endif

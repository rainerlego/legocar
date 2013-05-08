#ifndef SERVO_H
#define SERVO_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define servos_on PORTB|=(1<<PB0)
#define servos_off PORTB&=~(1<<PB0)
#define check_servo_power PORTB&(1<<PB0)

#define MILLISECONDBASE 8000

uint16_t servos_angular[8];
uint8_t * servoports[8+1]; //+1 for dummy servo
uint8_t servopins[8+1]; //+1 for dummy servo
uint8_t current_servo;
uint16_t pulsetime;


void servo_init(void);

#endif

#ifndef TWI_H
#define TWI_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include "led.h" //for controlling leds on twi bus activity
#include "servo.h" //for writing to servo angular array

#define TWIADDR (1<<TWA0) //adresse für i2c slave
#define TWIPREAMBLE 255
#define TWIESCAPE 254
#define TWIESC_is_preamble 1
#define TWIESC_is_escape 2

#define TWIBUFFERSIZE 16

#define TWISTATE_standby 0
#define TWISTATE_waitfordata 1

#define RECVcommand 0
#define RECVangular 1
#define RECVangular2 2

#define CMD_SERVO 0
#define CMD_LED 1
#define CMD_SERVOSonoff 2

extern uint8_t twi_activity;
extern uint8_t twi_dataactivity;

void i2cinit(void);
void twi_handle(uint8_t data);

#endif

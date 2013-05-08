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

uint8_t twi_activity;
uint8_t twi_dataactivity;

uint8_t twi_data_buffer[TWIBUFFERSIZE];
uint8_t twi_buffer_nr;  //next read index for buffer
uint8_t twi_buffer_nw;  //next write index for buffer
uint8_t twi_buffer_empty;

uint8_t twistate = TWISTATE_standby;
uint8_t recvstate = RECVcommand;
uint8_t data_complete;

uint8_t angularh;
uint8_t servo_waiting_for_data;


void i2cinit(void);
void twi_handle(void);

#endif

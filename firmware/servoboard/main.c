#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <stdint.h>
//#include <stdlib.h>
#include <util/delay.h>
//#include <inttypes.h>


#include "led.h"
#include "twi.h"
#include "servo.h"

int main (void)
{
  led_init();
  i2cinit();
  servo_init(); //this will initialize servo control, but power supply of all servos will be disabled for safety reasons. first set the correct angulars of all servos over twi and issue a servo power up afterwards (CMD_SERVOSonoff)

  led1_an;
  _delay_ms(500);
  led1_aus;
  _delay_ms(500);
  led1_an;
  _delay_ms(500);
  led1_aus;
  led2_aus;
  sei();  //enable global interrupts
	while (1) { 
    //try to read data from twi buffer:
    //twi_handle();
    
    //handle status leds
    //led_update();

	}
}

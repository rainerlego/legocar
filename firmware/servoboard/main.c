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
  //NOTE: Servo power control can be overridden by the jumper near the mosfet (jumper plugged in = override)

  led1_an;
  _delay_ms(500);
  led1_aus;
  _delay_ms(500);
  led1_an;
  _delay_ms(500);
  led1_aus;
  led2_aus;
  sei();  //enable global interrupts

  longcount = 0;
  TCCR2 = (1<<CS22) | (1<<CS21) | (1<<CS20); // servo watchdog prescaler 1024

	while (1) { 
    if(TCNT2>100){ // >=0.0128s
      TCNT2 = 0;
      longcount++;
      if(longcount>40){ //>=0.512 s
        longcount = 0;
        uint8_t i;
        //reset servos
        for(i = 0; i<8; i++){
          servos_angular[i] = 4000;
        }

      }

    }
    //handle status leds
    led_update();

	}
}

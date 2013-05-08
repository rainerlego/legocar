#include "led.h"

void led_init(){
  //configure pins for leds as output
  DDRD |= (1<<PD2) |(1<<PD3) |(1<<PD4);  //LED 1,2,3 und S7, S8

  //enable interrupt for timer0 (8bit)
  //used for led clearance
  TIMSK |= (1<<TOIE0);
}

//used for clearing leds
ISR(TIMER0_OVF_vect){ //32ms at 8MHz
  if((led_controlled_by_user & (1<<0)) ==0 ){
    led1_aus;
  }
  if((led_controlled_by_user & (1<<1)) ==0 ){
    led2_aus;
  }
}

void led_update(){
  if( (twi_activity == 1)  && ( ( led_controlled_by_user & (1<<0) ) == 0 )  ){
    led1_an;
    //configure timer0 to clear the leds for us
    TCNT0 = 0;
    TCCR0 = (1<<CS02) | (1<<CS00); //8bit timer0 prescaler to 1024
  }
  if( (twi_dataactivity == 1) && ( (led_controlled_by_user & (1<<1)) == 0) ){
    led2_an;
    //configure timer0 to clear the leds for us
    TCNT0 = 0;
    TCCR0 = (1<<CS02) | (1<<CS00); //8bit timer0 prescaler to 1024
  }
  if( (led_controlled_by_user & (1<<2)) == 0 ){
    if(check_servo_power !=0){
      led3_an;
    }else{
      led3_aus;
    }
  }
}

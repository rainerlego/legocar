#include "servo.h"
uint16_t servos_angular[8];
volatile uint8_t * servoports[8+1]; //+1 for dummy servo
uint8_t servopins[8+1]; //+1 for dummy servo
uint8_t current_servo;
uint16_t pulsetime;

void servo_init(){
  //configure servo relevant pins as outputs:
  DDRC |= (1<<PC0) |(1<<PC1) |(1<<PC2) |(1<<PC3); //S3, S4, S5, S6
  DDRB |= (1<<PB0) |(1<<PB1) |(1<<PB2); //Son/off and S1, S2
  DDRD |= (1<<PD0) |(1<<PD1); //S7, S8
  //configure mapping of servos to pins:
  //FIXME: this could be made static constants
  servoports[0] = &PORTB;
  servopins[0] = PB1;

  servoports[1] = &PORTB;
  servopins[1] = PB2;

  servoports[2] = &PORTC;
  servopins[2] = PC0;

  servoports[3] = &PORTC;
  servopins[3] = PC1;

  servoports[4] = &PORTC;
  servopins[4] = PC2;

  servoports[5] = &PORTC;
  servopins[5] = PC3;

  servoports[6] = &PORTD;
  servopins[6] = PD1;

  servoports[7] = &PORTD;
  servopins[7] = PD1;

  //dummy servo:
  servoports[8] = &PORTD;
  servopins[8] = PD1;

  //all servos to middle position, pulse disabled
  uint8_t i;
  for(i = 0; i<8; i++){
    servos_angular[i] = 4000;
    *servoports[i] &= ~(1<<servopins[i]);
  }
  current_servo = 8; //start with dummy servo... next one will be servo0 
  pulsetime = 20000; //simulate, that the previous pulse was complete

  servos_off; //servos have no power supply enabled

  
  TIMSK |= (1<<OCIE1A); //enable compare match (A) interrupt for Timer1 (16bit)
  TCNT1 = 0; //reset timer
  OCR1A = 4000; //next pulse will start in 1ms
  TCCR1B = (1<<CS10); //prescaler 1 start timer
}


ISR(TIMER1_COMPA_vect){
  //aktuellen servopuls beenden
  *servoports[current_servo] &= ~(1<<servopins[current_servo]);

  //stop and reset timer:
  TCCR1B = 0; //stop
  TCNT1=0;    //reset

  //configure for next servo
  current_servo = (current_servo + 1) % (8+1); //one dummy servo for 20ms correction

  if(current_servo!=8){ //real servo
    sei();//disable interrupts //FIXME: can interrupts happen here at all?
    OCR1A = MILLISECONDBASE + servos_angular[current_servo];
    cli();//re enable interrupts
    pulsetime+=OCR1A/8;
    
    //start pulse of next servo:
    *servoports[current_servo] |= (1<<servopins[current_servo]);
  }else{ //dummy servo will fill up time to 20ms
      uint16_t timeremaining = (20000-pulsetime);
      if(timeremaining<8192){ //dummy servo can compensate remaining time
        OCR1A = timeremaining*8;
        pulsetime = 0;
      }else{
        OCR1A = 0xffff;
        pulsetime+=8191;
        current_servo = 7; //next time we will have a dummy servo again :)
      }
  }
  //start timer
  TCCR1B = (1<<CS10); //prescaler 1
}

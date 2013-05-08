#define F_CPU 8000000UL
#include <avr/io.h>
//#include <stdint.h>
//#include <stdlib.h>
#include <util/delay.h>
//#include <inttypes.h>

#include <avr/interrupt.h>

#define led1_aus PORTD|=(1<<PD2)
#define led2_aus PORTD|=(1<<PD3)
#define led3_aus PORTD|=(1<<PD4)
#define led1_an PORTD&=~(1<<PD2)
#define led2_an PORTD&=~(1<<PD3)
#define led3_an PORTD&=~(1<<PD4)

#define servos_on PORTB|=(1<<PB0)
#define servos_off PORTB&=~(1<<PB0)

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

#define MILLISECONDBASE 8000


uint8_t twi_activity;
uint8_t twi_dataactivity;

uint8_t twi_data_buffer[TWIBUFFERSIZE];
uint8_t twi_buffer_nr = 0;  //next read index for buffer
uint8_t twi_buffer_nw = 0;  //next write index for buffer
uint8_t twi_buffer_empty = 1;

uint8_t twistate = TWISTATE_standby;
uint8_t recvstate = RECVcommand;
uint8_t data_complete;

uint8_t angularh;
uint8_t servo_waiting_for_data;

uint8_t led_controlled_by_user = 0;

uint16_t servos_angular[8];
uint8_t * servoports[8+1]; //+1 for dummy servo
uint8_t servopins[8+1]; //+1 for dummy servo
uint8_t current_servo;
uint16_t pulsetime;

void buffer_write(uint8_t data){ //if executed on full buffer, data will be dropped
  uint8_t nw_new = (twi_buffer_nw+1)%TWIBUFFERSIZE;
  if(nw_new!=twi_buffer_nr){ //buffer is not full
    twi_data_buffer[twi_buffer_nw]=data; //write data
    twi_buffer_nw = nw_new; //configure buffer for next byte

    twi_buffer_empty = 0;
  }
}

uint8_t buffer_read(){ //should NOT BE CALLED IF BUFFER IS EMPTY!!!
  uint8_t nr_old = twi_buffer_nr;
  twi_buffer_nr = (twi_buffer_nr+1)%TWIBUFFERSIZE;
  if(twi_buffer_nr == twi_buffer_nw ){
    twi_buffer_empty = 1; //we will read the last emelent in buffer now!
  }
  return twi_data_buffer[nr_old];
}

void servo_init(){
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

  TCNT1 = 0; //reset timer
  OCR1A = 4000; //next pulse will start in 1ms
  TCCR1B = (1<<CS10); //prescaler 1 start timer
}

void port_init(){
  DDRD = (1<<PD0) |(1<<PD1) |(1<<PD2) |(1<<PD3) |(1<<PD4);  //LED 1,2,3 und S7, S8
  DDRC = (1<<PC0) |(1<<PC1) |(1<<PC2) |(1<<PC3); //S3, S4, S5, S6
  DDRB = (1<<PB0) |(1<<PB1) |(1<<PB2); //Son/off and S1, S2
}


void i2cinit(){
  TWAR = TWIADDR; //slave adresse setzen
  TWAR |= (1<<TWGCE); //wir wollen general calls auch akzeptieren

  TWCR  = (1<<TWIE); //enable twi interrupt generation
  TWCR |= (1<<TWEA); //sende acks wenn daten an mich adressiert sind
  TWCR |= (1<<TWEN); //ENABLE TWI

  
}

//Protocol description:
// 1st Packet: Preamble
// 2nd Packet: Command (4 msb) + Extention (4 lsb)
// optional following packets: command specific

//packets (not preamble) should not contain preamble
//if packet (not preamble) is preamble (255) or escape (254) a follow up packet is expected, whose content is aded to the first packet modulo 256
//ATTENTION: if preamble is sent as data package result might be corrupt if original preamble was missed. hence is is recommended never to send preamble as data package
//example: packet1=254 packet2=1  => new packet will be 255
//example2: packet1=254 packet2=0 => new packet will be 254
//example3: packet1=254 packet2=2 => new packet will be 1


void twi_handle(){
  if(twi_buffer_empty == 1){
    return;
  }
  //FIXME: kann buffer_read unterbrochen werden (insbesondere von buffer_write)?? ich denke schon... nciht sicher
  uint8_t data = buffer_read();
  switch (twistate){
    case TWISTATE_standby:
      if(data==TWIPREAMBLE){
        twistate = TWISTATE_waitfordata;
        data_complete = 0;
      }
      break;
    case TWISTATE_waitfordata:
      if(data == TWIPREAMBLE || data == TWIESCAPE){
        data_complete += data;
        return;
      }
      //here we have collected 8 bit of data
      switch (recvstate){
        case RECVcommand:
          switch(data_complete & (0xf<<4)){
            case CMD_SERVO: //control servo
              servo_waiting_for_data = (data_complete<<4);
              recvstate = RECVangular; //we expect angular to be transmitted as the next byte
              break;
            case CMD_LED: //control LED
              if(data_complete&(1<<4)==0){ //led ausschalten
                switch(data_complete<<5){
                  case 0:
                    led_controlled_by_user = 1;
                    led1_aus;
                    break;
                  case 1:
                    led_controlled_by_user = 2;
                    led2_aus;
                    break;
                  case 2:
                    led3_aus;
                    break;
                }
              }else{
                switch(data_complete<<5){
                  case 0:
                    led_controlled_by_user = 1;
                    led1_an;
                    break;
                  case 1:
                    led_controlled_by_user = 2;
                    led2_an;
                    break;
                  case 2:
                    led3_an;
                    break;
                }
              }
              twistate = TWISTATE_standby; //goback to standby mode awaiting new transmission (recvstate should stay untouched)
          }
          break;
        case RECVangular:
          //receive the msbs of angular
          angularh = data_complete;
          //servos_angular[servo_waiting_for_data] = data_complete;

          recvstate = RECVangular2; //next data byte will be the lsbs of angular
          break;
        case RECVangular2:
          //FIXME: am i shifting right here (8bit vs 16 bit??) hope i am not losing bits here
          servos_angular[servo_waiting_for_data] = (((uint16_t)angularh)<<8) | (uint16_t)data_complete;

          recvstate = RECVcommand; //next data byte will be a command
          twistate = TWISTATE_standby; //goback to standby mode awaiting new transmission
          break;
      }

      break;
  }
}

ISR(TWI_vect){ //TWI interrupt handler //FIXME: what happens if i can not read data fast enough?
  uint8_t status = TWSR;
  twi_activity = 1;
  if(status == 0x80 || status == 0x90){ //daten empfangen an eigene adresse oder an general address
    twi_dataactivity = 1;
    buffer_write(TWDR); //daten in buffer schreiben
  }
}


//used for clearing leds
ISR(TIMER0_OVF_vect){ //32ms at 8MHz
  switch (led_controlled_by_user){
    case 0:
      led1_aus;
      led2_aus;
      break;
    case 1:
      led2_aus;
      break;
    case 2:
      led1_aus;
      break;
  }
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

void pwm_init(void) {
  PORTB = 0;
  //Fast PWM 13bit, Prescaler=1
  ICR1 = 0x1fff; //set top value for pwm
	OCR1A = 0;
	TCCR1A = (1<<COM1A1) | (1<<WGM11);
	TCCR1B = (1<<CS10) | (1<<WGM13) | (1<<WGM12);
}

void timer_init(){
  
  //enable interrupt for timer0 (8bit)
  //used for led clearance
  TIMSK |= (1<<TOIE0);

  //enable compare match (A) interrupt for Timer1 (16bit)
  TIMSK |= (1<<OCIE1A);
}

int main (void)
{
  port_init();
  timer_init;
  i2cinit();
  
  servo_init();
  sei();  //enable global interrupts
	while (1) { 
  
    //try to read data from twi buffer:
    twi_handle();

    //handle status leds
    if(twi_activity == 1 && led_controlled_by_user!=1){
      led1_an;
      //configure timer0 to clear the leds for us
      TCNT0 = 0;
      TCCR0 = (1<<CS02) | (1<<CS00); //8bit timer0 prescaler to 1024
    }
    if(twi_dataactivity == 1 && led_controlled_by_user!=2){
      led2_an;
      //configure timer0 to clear the leds for us
      TCNT0 = 0;
      TCCR0 = (1<<CS02) | (1<<CS00); //8bit timer0 prescaler to 1024
    }


	}
}

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

#define CMD_SERVO 0
#define CMD_LED 1

uint8_t twi_activity;
uint8_t twi_dataactivity;

uint8_t twi_data_buffer[TWIBUFFERSIZE];
uint8_t twi_buffer_nr = 0;  //next read index for buffer
uint8_t twi_buffer_nw = 0;  //next write index for buffer
uint8_t twi_buffer_empty = 1;

uint8_t twistate = TWISTATE_standby;
uint8_t recvstate = RECVcommand;
uint8_t data_complete;

uint8_t servo_waiting_for_data;
uint8_t servos_angular[8];

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

//2nd packet should not contain preamble or escape sequence
//packets 3 and following can contain preamble and escape sequence, when escaped correctly.


void twi_handle(){
  if(twi_buffer_empty == 1){
    return;
  }
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
                    led1_aus;
                    break;
                  case 1:
                    led2_aus;
                    break;
                  case 2:
                    led3_aus;
                    break;
                }
              }else{
                switch(data_complete<<5){
                  case 0:
                    led1_an;
                    break;
                  case 1:
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
          servos_angular[servo_waiting_for_data] = data_complete;

          recvstate = RECVcommand; //next data bit will be a command
          twistate = TWISTATE_standby; //goback to standby mode awaiting new transmission
          break;
      }

      break;
  }
}

ISR(TWI_vect){ //TWI interrupt handler
  uint8_t status = TWSR;
  twi_activity = 1;
  if(status == 0x80 || status == 0x90){ //daten empfangen an eigene adresse oder an general address
    twi_dataactivity = 1;
    buffer_write(TWDR); //daten in buffer schreiben
  }
}

void pwm_init(void) {
  PORTB = 0;
  //Fast PWM 13bit, Prescaler=1
  ICR1 = 0x1fff; //set top value for pwm
	OCR1A = 0;
	TCCR1A = (1<<COM1A1) | (1<<WGM11);
	TCCR1B = (1<<CS10) | (1<<WGM13) | (1<<WGM12);
}

int main (void)
{
  port_init();
  i2cinit();

  sei();  //enable global interrupts
	while (1) { 
    if(twi_activity == 1){
      led1_an;
    }
    if(twi_dataactivity == 1){
      led2_an;
    }
    _delay_ms(20);
	}
}

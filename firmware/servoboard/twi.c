#include "twi.h"

uint8_t twi_activity;
uint8_t twi_dataactivity;

uint8_t recvstate = RECVcommand;
uint8_t data_complete;

uint8_t angularh;
uint8_t servo_waiting_for_data;

/*
uint8_t twi_data_buffer[TWIBUFFERSIZE];
uint8_t twi_buffer_nr;  //next read index for buffer
uint8_t twi_buffer_nw;  //next write index for buffer
uint8_t twi_buffer_empty;
void buffer_write(uint8_t data); //if executed on full buffer, data will be dropped
uint8_t buffer_read(void); //should NOT BE CALLED IF BUFFER IS EMPTY!!!
*/

/*
void buffer_write(uint8_t data){ //if executed on full buffer, data will be dropped
  uint8_t nw_new = (twi_buffer_nw+1)%TWIBUFFERSIZE;
  if(nw_new!=twi_buffer_nr){ //buffer is not full
    twi_data_buffer[twi_buffer_nw]=data; //write data
    twi_buffer_nw = nw_new; //configure buffer for next byte

    twi_buffer_empty = 0;
  }
}


//FIXME: wir sollten nie gleichzeitig lesen und schreiben
uint8_t buffer_read(){ //should NOT BE CALLED IF BUFFER IS EMPTY!!!
  uint8_t nr_old = twi_buffer_nr;
  twi_buffer_nr = (twi_buffer_nr+1)%TWIBUFFERSIZE;
  if(twi_buffer_nr == twi_buffer_nw ){
    twi_buffer_empty = 1; //we will read the last emelent in buffer now!
  }
  return twi_data_buffer[nr_old];
}
*/
#define led1_toggle PORTD = PORTD ^ (1<<PD2)
#define led2_toggle PORTD = PORTD ^ (1<<PD3)

void i2cinit(){
  /*
  twi_buffer_nr = 0;
  twi_buffer_nw = 0;
  twi_buffer_empty = 1;
  */
  TWAR = TWIADDR; //slave adresse setzen
  TWAR |= (1<<TWGCE); //wir wollen general calls auch akzeptieren

  TWCR  = (1<<TWIE); //enable twi interrupt generation
  TWCR |= (1<<TWEA); //sende acks wenn daten an mich adressiert sind
  TWCR |= (1<<TWEN); //ENABLE TWI
  TWCR &= ~(1<<TWINT);
}

//TODO: evtl das "wir sind zu langsam" implementieren
ISR(TWI_vect){ //TWI interrupt handler //FIXME: what happens if i can not read data fast enough?
  led1_toggle;
  uint8_t status = TWSR;
  twi_activity = 1;
  if(status == 0x80 || status == 0x90){ //daten empfangen an eigene adresse oder an general address
    led2_toggle;
    twi_dataactivity = 1;
    //buffer_write(TWDR); //daten in buffer schreiben
    twi_handle(TWDR);
  }
  TWCR |= (1<<TWINT);
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


void twi_handle(uint8_t data){
  /*
  if(twi_buffer_empty == 1){
    return;
  }
  uint8_t data = buffer_read();
  */
  data_complete += data;

  if(data==TWIPREAMBLE){
    recvstate = RECVcommand;
    data_complete = 0;
    return;
  }

  if(data == TWIESCAPE){
    //Do not reset data_complete
    //Just return
    //"Real data" will be available next time, when next value is added to TWIESCAPE
    return;
  }

  //If no escape or preample was received: interpret as data
  //here we have collected 8 bit of data
  switch (recvstate){
    case RECVcommand: //data wird interpretiert als command
      switch(data_complete & (0xf<<4)){
        case CMD_SERVO: //control servo
          servo_waiting_for_data = (data_complete<<4);
          recvstate = RECVangular; //we expect angular to be transmitted as the next byte
          break;
        case CMD_LED: //control LED
          if((data_complete&(1<<4))==0){ //led ausschalten
            switch(data_complete<<5){
              case 0:
                led_controlled_by_user |= (1<<0);
                led1_aus;
                break;
              case 1:
                led_controlled_by_user |= (1<<1);
                led2_aus;
                break;
              case 2:
                led_controlled_by_user |= (1<<2);
                led3_aus;
                break;
            }
          }else{
            switch(data_complete<<5){
              case 0:
                led_controlled_by_user |= (1<<0);
                led1_an;
                break;
              case 1:
                led_controlled_by_user |= (1<<1);
                led2_an;
                break;
              case 2:
                led_controlled_by_user |= (1<<2);
                led3_an;
                break;
            }
          }
          break;
        case CMD_SERVOSonoff:
          if((data_complete&(1<<4))==0){ //servos ausschalten
            servos_off;
          }else{
            servos_on;
          }
          
          break;
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
      break;
  }//switch recvstate

  data_complete = 0;
}


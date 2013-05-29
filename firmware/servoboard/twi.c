#include "twi.h"

uint8_t twi_activity;
uint8_t twi_dataactivity;

uint8_t recvstate = RECVcommand;
uint8_t data_complete;

uint8_t angularh;
uint8_t servo_waiting_for_data;

uint8_t twi_transmit_buffer_index = 0;
uint8_t twi_transmit_buffer[TWITRANSMITBUFFERSIZE];

uint8_t add_to_transmit_buffer(uint8_t data){
  if(twi_transmit_buffer_index >= TWITRANSMITBUFFERSIZE){ //buffer is full -> drop data
    return 0;
  }
  twi_transmit_buffer[twi_transmit_buffer_index]=data;
  twi_transmit_buffer_index++;
  return 1;
}

uint8_t get_from_transmit_buffer(){
  if(twi_transmit_buffer_index>0){
    twi_transmit_buffer_index--;
  }
  return twi_transmit_buffer[twi_transmit_buffer_index];
}

void i2cinit(){
  TWAR = TWIADDR; //slave adresse setzen
  TWAR |= (1<<TWGCE); //wir wollen general calls auch akzeptieren

  TWCR  = (1<<TWIE); //enable twi interrupt generation
  TWCR |= (1<<TWEA); //sende acks wenn daten an mich adressiert sind
  TWCR |= (1<<TWEN); //ENABLE TWI
  TWCR &= ~(1<<TWINT);
}

ISR(TWI_vect){
  //led1_toggle;
  twi_activity = 1;
  uint8_t status = TWSR;
  switch (status){
    case 0x80:
    case 0x90: //daten empfangen an eigene adresse oder an general address
      //led2_toggle;
      twi_dataactivity = 1;
      //buffer_write(TWDR); //daten in buffer schreiben
      twi_handle(TWDR);
      break;
    case 0xA8:
    case 0xB0:
    case 0xB8: //data was requested from master => give him some data :)
      TWDR = get_from_transmit_buffer();
      break;
    default:
      TWDR = 0xbc;

  }
  
  TWCR |= (1<<TWINT);
}

//Protocol description:
// 1st Packet: Preamble
// 2nd Packet: Command (4 msb) + Extention (4 lsb)
// optional following packets: command specific
//
// Transmission format:
// H      L   H      L   H      L
// 11111111   CCCCeeee   dddddddd ...
// preamble   com  ext   data


//packets (not preamble) should NOT contain preamble
//if a escape (254) packet is received, a follow up packet is expected, whose content is added to the first packet modulo 256
//ATTENTION: if preamble is sent as data package the current transmission is reset and a new command is expected as next packet
//example: packet1=254 packet2=1  => new packet will be 255
//example2: packet1=254 packet2=0 => new packet will be 254
//example3: packet1=254 packet2=2 => new packet will be 1

//Available commands:
//
// - CMD_SERVO (0)
//  Description:
//    Sets 16 bit angular value of specific servo
//  Parameters:
//    4 bit (extension)
//      ServoID. Allowed values: 0-7
//    8 bit (follow up 1)
//      Most significant bits of the servo angular
//    8 bit (follow up 2)
//      Least significant bits of the servo angular
//  Note:
//    The save range of angular values is 0 to 8000
//  Example:
//    0xff 0x05 0x01 0x23
//    Servo #5 will be set to position 0x0123
//
// - CMD_LED (1)
//  Description:
//    Controls the three onboard LEDs. By default the onboard LEDs are controlled
//    by hardware to display TWI bus activity and servo power state. Once a LED
//    receives a command from TWI control is handed over to the user for this
//    (and only this) LED.
//  Parameters:
//    1 bit (msb)
//      Switch the LED on (1) or off (0)
//    3 bit (lsbs)
//      The LEDs corresponding to the bits, which are set, will now be controlled by the user
//      Additionally, they will be set on or off depending on the value of the msb of the 4-bit extenstion 
//  Example:
//    0xff 0x10
//    LED #2 (the third LED) will be switched on (0x10 = 0b1010)
//    
// - CMD_SERVOonoff (2)
//  Description:
//    Enable/disable powersupply of ALL servos. Can be overridden by the jumper
//    near the mosfet on the board. (jumper present -> servos always powered)
//  Parameters:
//    4 bit (extension)
//      power off (0) or "power on" (1-255)
//  Example:
//    0xff 0x21
//    Power ON
//
// - CMD_RESET_TRANSMIT_BUFFER (3)
//  Description:
//    Resets the Transmit buffer to empty
//  Parameters:
//    none
//  Example:
//    0xff 0x30
//
// - CMD_GET_SERVO (4)
//  Description:
//    Put the 16 bit angular value of the selected servo into the transmit buffer.
//    Order: First MSB then LSB
//  Parameters:
//    4 bit (extension)
//      Servo ID (allowed values: 0-7)
//  Example:
//    0xff 0x43
//    Angular value of servo #3 will be written to transmit buffer
//
// - CMD_GET_LEDS (5)
//  Description:
//    Write the status of the three onboard LEDs to the transmit buffer. One byte
//    is written to the buffer. The three least significant bits represent the state
//    of the leds (1 = on, 0 = off) LED #0 = Bit0 (lsb), #1 = Bit1, #2 = Bit2. 
//  Parameters:
//    none
//  Example:
//    0xff 0x50
//
// - CMD_GET_SERVOonoff (6)
//  Description:
//    8 bit to buffer: 1 when power on, 0 when power not on
//  Parameters:
//    none
//  Example:
//    0xff 0x60


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
      longcount = 0; //reset watchdog!
      switch((data_complete & (0xf0))>>4 ){
        case CMD_PING:
          break;
        case CMD_SERVO: //control servo
          servo_waiting_for_data = data_complete & (0x0f);
          recvstate = RECVangular; //we expect angular to be transmitted as the next byte
          break;
        case CMD_LED: //control LED
          led_controlled_by_user |= data_complete & ((1<<2|1<<1|1<<0)); //alle leds die in dem command vorkommen werden ab sofort vom "user" bedient und sind abgekoppelt von der "hart verdrahteten" steuerung

          if((data_complete&(1<<3))==0){ //ausschalten
            if(data_complete&(1<<0))
              led1_aus;
            if(data_complete&(1<<1))
              led2_aus;
            if(data_complete&(1<<2))
              led3_aus;
          } else { //einschalten
            if(data_complete&(1<<0))
              led1_an;
            if(data_complete&(1<<1))
              led2_an;
            if(data_complete&(1<<2))
              led3_an;
          }
          break;
        case CMD_SERVOSonoff:
          if((data_complete&(1<<4))==0){ //servos ausschalten
            servos_off;
          }else{
            servos_on;
          }
          
          break;
        
        //handle transmit requests:
        case CMD_RESET_TRANSMIT_BUFFER:
          twi_transmit_buffer_index =0;
          break;
        case CMD_GET_SERVO:
          {
            uint16_t tmp =  servos_angular[(data_complete<<4)%8];
            add_to_transmit_buffer((uint8_t)tmp);
            add_to_transmit_buffer((uint8_t)(tmp<<8));
          }
          break;
        case CMD_GET_SERVOonoff:
          add_to_transmit_buffer(check_servo_power!=0);
          break;
        case CMD_GET_LEDS:
          add_to_transmit_buffer( ~((PORTD & ((1<<PD2)|(1<<PD3)|(1<<PD4)) ) >> PD2)  ); //last 3 bits now represent the three leds
          break;
      }
      break;
    case RECVangular:
      //receive the msbs of angular
      angularh = data_complete;

      recvstate = RECVangular2; //next data byte will be the lsbs of angular
      break;
    case RECVangular2:
      {
        uint16_t speed = ((uint16_t)angularh)<<8 | (uint16_t)data_complete;
        if (speed > SERVO_MAX_VALUE) {
          speed = SERVO_MAX_VALUE;
        }
        servos_angular[servo_waiting_for_data] = speed;

        recvstate = RECVcommand; //next data byte will be a command
        break;
      }//scope wird gebraucht für deklaration von lokaler variable speed
  }//switch recvstate
  data_complete = 0;
}


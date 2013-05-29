#include <stdint.h>

#define TWI_PREAMBLE 0xff
#define CMD_SERVO 0
#define CMD_LED 1
#define CMD_SERVOSonoff 2
#define CMD_RESET_TRANSMIT_BUFFER 3
#define CMD_GET_SERVO 4
#define CMD_GET_LEDS 5
#define CMD_GET_SERVOonoff 6
#define CMD_PING 7

#define SERVOBOARD_ADDR 0x03
#define SERVOBOARD_I2C_CHARDEV "/dev/i2c-1"

#define LOWBYTE(v) ((unsigned char) (v))
#define HIGHBYTE(v) ((unsigned char)(((unsigned int) (v)) >> 8))

int servoboard_open();
void servoboard_close();
int servoboard_setservo ( uint8_t servoNr, uint16_t servoPos );
int servoboard_setleds ( uint8_t onoff, uint8_t leds );
int servoboard_ping();

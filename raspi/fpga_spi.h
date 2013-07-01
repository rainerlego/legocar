#include <stdint.h>

#define CMD_SERVO 0
#define CMD_LED 1
#define CMD_SERVOSonoff 2
#define CMD_RESET_TRANSMIT_BUFFER 3
#define CMD_GET_SERVO 4
#define CMD_GET_LEDS 5
#define CMD_GET_SERVOonoff 6
#define CMD_PING 7
#define CMD_SPEED 8

#define FPGA_SPI_DEV "/dev/spidev1.1"
#define SPI_MODE 0 //könnte falsch sein
#define SPI_DELAY 1 //könnte falsch sein
#define SPI_SPEED_HZ 500000
#define SPI_BITS_PER_WORD 8

#define LOWBYTE(v) ((unsigned char) (v))
#define HIGHBYTE(v) ((unsigned char)(((unsigned int) (v)) >> 8))

int fpga_open();
void fpga_close();
int fpga_setservo ( uint8_t servoNr, uint16_t servoPos );
int fpga_setleds ( uint8_t onoff, uint8_t leds );
int fpga_setspeed (uint16_t speed );
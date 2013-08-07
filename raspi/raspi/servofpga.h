#ifndef servofpgah
#define servofpgah

#include <stdint.h>

#include "commands.h"

#define FPGA_SPI_DEV "/dev/spidev0.1"
#define SPI_MODE 0
#define SPI_DELAY 1 //könnte falsch sein
#define SPI_SPEED_HZ 500000
#define SPI_BITS_PER_WORD 8
#define SPI_PREAMBLE 0xff

#define LOWBYTE(v) ((unsigned char) (v))
#define HIGHBYTE(v) ((unsigned char)(((unsigned int) (v)) >> 8))

int fpga_open();
void fpga_close();
int fpga_setservo ( uint8_t servoNr, uint16_t servoPos );
int fpga_setleds ( uint8_t onoff, uint8_t leds );
int fpga_setspeedv (uint16_t vspeed, uint16_t vsteering );
int fpga_setspeedacc (uint8_t speed_intead_acc );

#endif

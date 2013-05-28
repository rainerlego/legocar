#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

#include "servo.h"
#include "servoboard.h"

//fd ist global
//damit wir das nicht immer mitschleppen müssen
//kann leicht geändert werden, falls wir mal mehr als 1 servoboard haben
int fd;

int servoboard_init()
{
  return 0;
}

int servoboard_open()
{
	printf ( "N: Servo: I will use I2C to send servo commands to %02x via %s\n", SERVOBOARD_ADDR, SERVOBOARD_I2C_CHARDEV );

  if ((fd = open(SERVOBOARD_I2C_CHARDEV, O_RDWR)) < 0) {
    printf("E: Servoboard: Failed to open i2c port\n");
    return -1;
  }
  
  if (ioctl(fd, I2C_SLAVE, SERVOBOARD_ADDR) < 0) {
    printf("E: Servoboard: Unable to get bus access to talk to slave\n");
    return -1;
  }

  return 0;
}

void servoboard_close()
{
  close(fd);
}

int servoboard_ping()
{
  unsigned char buf[2];

  buf[0] = TWI_PREAMBLE;
  buf[1] = CMD_PING;


  if ((twisend(buf, 2)) != 2) {
    printf("E: Servoboard: Could not write specified amount of bytes to i2c chardev\n");
    return -1;
  }

  return 0;
}

int servoboard_setservo ( uint8_t servoNr, uint16_t servoPos )
{
  unsigned char buf[4];

  buf[0] = TWI_PREAMBLE;
  buf[1] = (CMD_SERVO<<4) | (servoNr & 0xf);
  buf[2] = HIGHBYTE(servoPos);
  buf[3] = LOWBYTE(servoPos);
  
  if ((twisend(buf, 4)) != 4) {
    printf("E: Servoboard: Could not write specified amount of bytes to i2c chardev\n");
    return -1;
  }

  return 0;
}

int servoboard_setleds ( uint8_t onoff, uint8_t leds )
{
  unsigned char buf[2];

  buf[0] = TWI_PREAMBLE;
  buf[1] = 0;

  buf[1] = (CMD_LED<<4) | (leds & ((1<<2)|(1<<1)|(1<<0)));
  if ( onoff )
    buf[1] |= (1<<3);
  
  if ((twisend(buf, 2)) != 2) {
    printf("E: Servoboard: Could not write specified amount of bytes to i2c chardev\n");
    return -1;
  }

  return 0;
}

int twisend ( char*buf, int len )
{
  return write(fd,buf,len);
}

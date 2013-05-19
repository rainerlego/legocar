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

//fd ist global
//damit wir das nicht immer mitschleppen müssen
//kann leicht geändert werden, falls wir mal mehr als 1 servoboard haben
int fd;
pthread_mutex_t servo_mutex;

int servoboard_init()
{
  pthread_mutex_init ( &servo_mutex, NULL );
  return 0;
}

int servoboard_open()
{
  if ((fd = open(SERVOBOARD_I2C_CHARDEV, O_RDWR)) < 0) {
    printf("Failed to open i2c port\n");
    return -1;
  }
  
  if (ioctl(fd, I2C_SLAVE, SERVOBOARD_ADDR) < 0) {
    printf("Unable to get bus access to talk to slave\n");
    return -1;
  }

  return 0;
}

void servoboard_close()
{
  close(fd);
}

int servoboard_setservo ( uint8_t servoNr, uint16_t servoPos )
{
  unsigned char buf[4];

  if ( (servoPos < 0) || (servoPos > 8000 ) ) {
    printf ( "E: servoPos (%d) out of range\n", servoPos );
    return -1;
  }
  if ( ( servoNr < 0) || ( servoNr > 7) ) {
    printf ( "E: servoNr (%d) out of range\n", servoNr );
    return -1;
  }
  
  buf[0] = TWI_PREAMBLE;
  buf[1] = (CMD_SERVO<<4) | (servoNr & 0xf);
  buf[2] = HIGHBYTE(servoPos);
  buf[3] = LOWBYTE(servoPos);
  
  pthread_mutex_lock ( &servo_mutex );
  if ((write(fd, buf, 4)) != 4) {
    printf("E: Could not write specified amount of bytes to i2c chardev\n");
    pthread_mutex_unlock ( &servo_mutex );
    return -1;
  }
  pthread_mutex_unlock ( &servo_mutex );

  return 0;
}

int servoboard_setleds ( uint8_t onoff, uint8_t leds )
{
  unsigned char buf[2];

  buf[0] = TWI_PREAMBLE;
  buf[1] = 0;

  buf[1] = leds & ((1<<2)|(1<<1)|(1<<0));
  if ( onoff )
    buf[1] |= (1<<3);
  
  pthread_mutex_lock ( &servo_mutex );
  if ((write(fd, buf, 2)) != 2) {
    printf("E: Could not write specified amount of bytes to i2c chardev\n");
    pthread_mutex_unlock ( &servo_mutex );
    return -1;
  }
  pthread_mutex_unlock ( &servo_mutex );

  return 0;
}

void servoboard_testservos()
{
  if ( servoboard_open ( SERVOBOARD_I2C_CHARDEV, SERVOBOARD_ADDR ) )
  {
    printf ( "E: Could not open i2c chardev\n" );
    exit(-1);
  }

  printf("Moving servo left\n");
  servoboard_setservo(0,0);
  sleep(2);
  printf("Moving servo centre\n");
  servoboard_setservo(0,4000);
  sleep(2);
  printf("Moving servo right\n");
  servoboard_setservo(0,8000);
  sleep(2);
  printf("Moving servo centre\n");
  servoboard_setservo(0,4000);
  
  servoboard_close();
}

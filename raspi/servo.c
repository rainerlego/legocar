#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

#include "config.h"
#include "servo.h"
#include "servoboard.h"
#include "servosim.h"

pthread_mutex_t servo_mutex;

int servo_init()
{
  pthread_mutex_init ( &servo_mutex, NULL );
  return 0;
}

int servo_open()
{
#if SERVO_M == SERVO_BOARD
  return servoboard_open();
#elif SERVO_M == SERVO_SIM
  return servosim_open();
#endif
}

void servo_close()
{
#if SERVO_M == SERVO_BOARD
  servoboard_close();
#elif SERVO_M == SERVO_SIM
  servosim_close();
#endif
}

int servo_setservo ( uint8_t servoNr, uint16_t servoPos )
{
  int ret;

  if ( (servoPos < 0) || (servoPos > 8000 ) ) {
    printf ( "E: Servo: servoPos (%d) out of range\n", servoPos );
    return -1;
  }
  if ( ( servoNr < 0) || ( servoNr > 7) ) {
    printf ( "E: Servo: servoNr (%d) out of range\n", servoNr );
    return -1;
  }

  pthread_mutex_lock ( &servo_mutex );
#if SERVO_M == SERVO_BOARD
  ret = servoboard_setservo(servoNr, servoPos);
#elif SERVO_M == SERVO_SIM
  ret = servosim_setservo(servoNr, servoPos);
#endif
  pthread_mutex_unlock ( &servo_mutex );

  return ret;
}

int servo_setleds ( uint8_t onoff, uint8_t leds )
{
  int ret;

  if ( onoff < 0 || onoff > 1 )
  {
    printf ( "E: Servo: setled: onoff wrong: %d\n", onoff );
    return -1;
  }

  if ( leds < 0 || leds > (int)((1<<0)|(1<<1)|(1<<2)) )
  {
    printf ( "E: Servo: setled: mask wrong: %d\n", leds );
    return -1;
  }


  pthread_mutex_lock ( &servo_mutex );
#if SERVO_M == SERVO_BOARD
  ret = servoboard_setleds(onoff, leds);
#elif SERVO_M == SERVO_SIM
  ret = servosim_setleds(onoff, leds);
#endif
  pthread_mutex_unlock ( &servo_mutex );

  return ret;
}

void servo_testservos()
{
  if ( servo_open() )
  {
    printf ( "E: Servo: Could not open\n" );
    exit(-1);
  }

  printf("Moving servo left\n");
  servo_setservo(0,0);
  sleep(2);
  printf("Moving servo centre\n");
  servo_setservo(0,4000);
  sleep(2);
  printf("Moving servo right\n");
  servo_setservo(0,8000);
  sleep(2);
  printf("Moving servo centre\n");
  servo_setservo(0,4000);
  
  servo_close();
}

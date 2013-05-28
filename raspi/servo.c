#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h> //for threading , link with lpthread
#include <sys/time.h>

#include "config.h"
#include "servo.h"
#include "servoboard.h"
#include "servosim.h"

pthread_mutex_t servo_mutex;

struct timeval tservo1[8], tservo2[8];
struct timeval t1,t2;
double diff;

int perm_src, perm_port;

int servo_init()
{
	perm_src = SERVO_PERM_TCP;
	perm_port = 123;
  pthread_mutex_init ( &servo_mutex, NULL );
  gettimeofday(&t1,NULL);
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

int servo_checkperm ( int src, int port )
{
	if ( (perm_src == src) && (perm_port == port) )
	{
		return 0;
	} else {
		return -1;
	}
}

int servo_getperm ( int src, int port )
{
  pthread_mutex_lock ( &servo_mutex );
	perm_src = src;
	perm_port = port;
  pthread_mutex_unlock ( &servo_mutex );
}

int servo_setservo ( uint8_t servoNr, uint16_t servoPos, int force, int src, int port )
{
  int ret;

	if ( servo_checkperm ( src, port ) )
	{
		printf ( "W: Servo: noperm\n" );
		return -1;
	}

  if ( (servoPos < 0) || (servoPos > 8000 ) ) {
    printf ( "E: Servo: servoPos (%d) out of range\n", servoPos );
    return -1;
  }
  if ( ( servoNr < 0) || ( servoNr > 7) ) {
    printf ( "E: Servo: servoNr (%d) out of range\n", servoNr );
    return -1;
  }

  pthread_mutex_lock ( &servo_mutex );

	/*
  gettimeofday(&t2,NULL);
  diff =  ((t2.tv_sec)*1000000+(t2.tv_usec))
        - ((t1.tv_sec)*1000000+(t1.tv_usec));

  if ( (!force) && (diff < 100000) )
  {
    printf ( "N: servo: Only %010.0fus have passed since last write; Ignoring this command\n", diff );
  } else {
    printf ( "N: servo: %010.0fus have passed since last write; OK\n", diff );
    gettimeofday(&t1,NULL);
		*/
#if SERVO_M == SERVO_BOARD
    ret = servoboard_setservo(servoNr, servoPos);
#elif SERVO_M == SERVO_SIM
    ret = servosim_setservo(servoNr, servoPos);
#endif
  //}

  pthread_mutex_unlock ( &servo_mutex );

  return ret;
}

int servo_setleds ( uint8_t onoff, uint8_t leds, int force, int src, int port )
{
  int ret;

	if ( servo_checkperm ( src, port ) )
	{
		printf ( "W: Servo: noperm\n" );
		return -1;
	}

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

	/*
  printf("Moving servo left\n");
  servo_setservo(0,0,0);
  sleep(2);
  printf("Moving servo centre\n");
  servo_setservo(0,4000,0);
  sleep(2);
  printf("Moving servo right\n");
  servo_setservo(0,8000,0);
  sleep(2);
  printf("Moving servo centre\n");
  servo_setservo(0,4000,0);
	*/
  
  servo_close();
}

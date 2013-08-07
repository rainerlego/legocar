#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //for threading , link with lpthread

#include <bluetooth/bluetooth.h>
#include <cwiid.h>

#include "servo.h"
#include "wiimote.h"

int wii_connected;
cwiid_wiimote_t *wiimote;
struct cwiid_state state;
bdaddr_t bdaddr;

struct timeval t1,t2;
double diff;

int accmode;

int speedacc;
int as;

unsigned int wii_watchdog;
pthread_t wiitping;

void *wii_thread_ping(void * v)
{
  struct cwiid_state s;
	while(1)
	{
		wii_watchdog++;
		if (wii_watchdog>400)
		{
			wii_watchdog = 0;
			if (wii_connected)
      {
        //r = cwiid_get_state(wiimote,&s);
          servo_ping();
      }
		}
    usleep(1000);
	}
}

void wii_start_ping_thread()
{
  if( pthread_create( &wiitping , NULL ,  wii_thread_ping , NULL) < 0)
  {
    perror("E: wii: could not create ping thread\n");
  }
}

int wii_to_servo ( double wii, int invert )
{
  double ed;

  ed = (wii-106)*8000.0/52.0;

  if ( ed < 0 )
    ed = 0;
  if ( ed > 8000 )
    ed = 8000;

  if (invert)
    ed = 8000 - ed;

  return (int)ed;
}

//cwiid_mesg_callback_t cwiid_callback;
void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count, union cwiid_mesg mesg[], struct timespec *timestamp)
{
  int i, j;
  int valid_source;
  int servo_steering, servo_accel;
  unsigned int btn;

  for (i=0; i < mesg_count; i++)
  {
    //printf ( "wii msg%d\n", mesg[i].type );
    switch (mesg[i].type) {
      case CWIID_MESG_STATUS:
        printf("Status Report: battery=%d\n", mesg[i].status_mesg.battery);
        break;
      case CWIID_MESG_BTN:
        //printf("Button Report: %.4X\n", mesg[i].btn_mesg.buttons);
        btn = mesg[i].btn_mesg.buttons;

				if ( btn & CWIID_BTN_PLUS )
				{
					printf ("wii: get perm\n" );
					servo_getperm ( SERVO_PERM_WII, 0 );
          wii_watchdog = 0;
				}

				if ( btn & CWIID_BTN_MINUS )
				{
					if (as==0)
						as = 1;
					printf ("wii: enable/disable schlupf to %d\n", as);
					servo_as ( as, 0, SERVO_PERM_WII, 0 );
          wii_watchdog = 0;
				}

				if ( btn & CWIID_BTN_HOME )
				{
					if (speedacc=0)
						speedacc=1;
					printf ("wii: switch speed/acc to %d\n", speedacc);
					servo_setspeedacc ( speedacc, 0, SERVO_PERM_WII, 0 );
          wii_watchdog = 0;
				}

        if ( btn & CWIID_BTN_A )
        {
          if ( accmode == WII_ACCMODE_TILT )
          {
            accmode = WII_ACCMODE_BTN;
            cwiid_set_led(wiimote, 0x02);
          } else {
            accmode = WII_ACCMODE_TILT;
            cwiid_set_led(wiimote, 0x01);
          }
        }

        if ( accmode == WII_ACCMODE_BTN )
        {
          if ( (btn & CWIID_BTN_1) && (btn &CWIID_BTN_2) )
          {
            servo_setservo ( 0, 7000, 0, SERVO_PERM_WII, 0 );
            wii_watchdog = 0;
            //printf ( "AB\n" );
          } else {
            if ( btn & (CWIID_BTN_2) )
            {
              //printf ( "B\n" );
              servo_setservo ( 0, 8000, 0, SERVO_PERM_WII, 0 );
              wii_watchdog = 0;
            } else if ( btn & (CWIID_BTN_1) ) {
              //printf ( "A\n" );
              servo_setservo ( 0, 5500, 0, SERVO_PERM_WII, 0 );
              wii_watchdog = 0;
            } else {
              if ( btn & CWIID_BTN_LEFT)
              {
                servo_setservo ( 0, 2000, 0, SERVO_PERM_WII, 0 );
                wii_watchdog = 0;
              } else {
                //printf ( "0\n" );
                servo_setservo ( 0, 4000, 0, SERVO_PERM_WII, 0 );
                wii_watchdog = 0;
              }
            }
          }
        }

        break;
      case CWIID_MESG_ACC:
        //printf("Acc Report: x=%d, y=%d, z=%d\n", mesg[i].acc_mesg.acc[CWIID_X], mesg[i].acc_mesg.acc[CWIID_Y], mesg[i].acc_mesg.acc[CWIID_Z]);
        gettimeofday(&t2,NULL);
        diff =  ((t2.tv_sec)*1000000+(t2.tv_usec))
              - ((t1.tv_sec)*1000000+(t1.tv_usec));

        if ( (diff < 100000) )
        {
          //printf ( "N: servo: Only %010.0fus have passed since last write; Ignoring this command\n", diff );
        } else {
          //printf ( "N: servo: %010.0fus have passed since last write; OK\n", diff );
          gettimeofday(&t1,NULL);
          servo_accel = wii_to_servo ( mesg[i].acc_mesg.acc[CWIID_X], 0 );
          servo_steering = wii_to_servo ( mesg[i].acc_mesg.acc[CWIID_Y], 0 );
          //printf ( "servo: accel: %d, steer: %d\n", servo_accel, servo_steering );
          if ( accmode == WII_ACCMODE_TILT )
          {
            servo_setservo ( 0, servo_accel, 0, SERVO_PERM_WII, 0 );
            wii_watchdog = 0;
          }
          servo_setservo ( 1, servo_steering, 0, SERVO_PERM_WII, 0 );
          wii_watchdog = 0;
        }
        break;
      case CWIID_MESG_ERROR:
        wii_connected = 0;
        //reset servos
        servo_setservo ( 0, 4000, 0, SERVO_PERM_WII, 0 );
        servo_setservo ( 1, 4000, 0, SERVO_PERM_WII, 0 );
        fprintf(stderr, "E: wii: received error, disconnecting...\n");
        wii_close();
        break;
    } //switch msg.type
  } //foreach msg
}

int wii_open()
{
  bdaddr = *BDADDR_ANY;

  accmode = WII_ACCMODE_BTN;

	as = 0;
	speedacc = 0;

  gettimeofday(&t1,NULL);

  printf ( "N: wii: trying to find wiimote...\n" );
  if (!(wiimote = cwiid_open(&bdaddr, 0))) {
    fprintf(stderr, "E: wii: Unable to connect to wiimote\n");
    return -1;
  }
  printf ("N: wii: open\n" );
  if (cwiid_set_mesg_callback(wiimote, cwiid_callback)) {
    fprintf(stderr, "E: wii: Unable to set message callback\n");
    cwiid_close(wiimote);
    return -1;
  }
  printf ("N: wii: callback\n" );
  if (cwiid_set_led(wiimote, 0x02)) {
    fprintf(stderr, "Error setting LEDs \n");
    cwiid_close(wiimote);
    return -1;
  }
  printf ("N: wii: led\n" );
  if (cwiid_set_rpt_mode(wiimote, CWIID_RPT_BTN | CWIID_RPT_ACC)) {
    fprintf(stderr, "E: wii: could not set report mode\n");
    cwiid_close(wiimote);
    return -1;
  }
  printf ("N: wii: report mode set\n" );

  if (cwiid_enable(wiimote, CWIID_FLAG_MESG_IFC | CWIID_FLAG_CONTINUOUS)) {
    fprintf(stderr, "E: wii: could not enable reporting\n");
    cwiid_close(wiimote);
    return -1;
  }
  printf ("N: wii: reporting enabled\n" );

  wii_connected = 1;


  return 0;
}

int wii_close()
{
  wii_connected = 0;
  if (cwiid_close(wiimote)) {
    fprintf(stderr, "E: wii: Error on wiimote disconnect\n");
    return -1;
  }
  return 0;
}

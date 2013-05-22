#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <bluetooth/bluetooth.h>
#include <cwiid.h>

#include "servo.h"

int wii_connected;
cwiid_wiimote_t *wiimote;
struct cwiid_state state;
bdaddr_t bdaddr;

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

  for (i=0; i < mesg_count; i++)
  {
    switch (mesg[i].type) {
      case CWIID_MESG_STATUS:
        printf("Status Report: battery=%d\n", mesg[i].status_mesg.battery);
        break;
      case CWIID_MESG_BTN:
        printf("Button Report: %.4X\n", mesg[i].btn_mesg.buttons);
        break;
      case CWIID_MESG_ACC:
        //printf("Acc Report: x=%d, y=%d, z=%d\n", mesg[i].acc_mesg.acc[CWIID_X], mesg[i].acc_mesg.acc[CWIID_Y], mesg[i].acc_mesg.acc[CWIID_Z]);
				servo_accel = wii_to_servo ( mesg[i].acc_mesg.acc[CWIID_X], 0 );
				servo_steering = wii_to_servo ( mesg[i].acc_mesg.acc[CWIID_Y], 1 );
				//printf ( "servo: accel: %d, steer: %d\n", servo_accel, servo_steering );
				servo_setservo ( 0, servo_accel );
				servo_setservo ( 1, servo_steering );
        break;
      case CWIID_MESG_ERROR:
        fprintf(stderr, "E: wii: received error, disconnecting...\n");
				wii_close();
        break;
    } //switch msg.type
  } //foreach msg
}

int wii_open()
{
  bdaddr = *BDADDR_ANY;

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
  if (cwiid_set_led(wiimote, 0x05)) {
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

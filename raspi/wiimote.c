#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <bluetooth/bluetooth.h>
#include <cwiid.h>

#include "servo.h"

cwiid_wiimote_t *wiimote;
struct cwiid_state state;
bdaddr_t bdaddr;

//cwiid_mesg_callback_t cwiid_callback;
void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count, union cwiid_mesg mesg[], struct timespec *timestamp)
{
  int i, j;
  int valid_source;

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
        printf("Acc Report: x=%d, y=%d, z=%d\n",
        mesg[i].acc_mesg.acc[CWIID_X],
        mesg[i].acc_mesg.acc[CWIID_Y],
        mesg[i].acc_mesg.acc[CWIID_Z]);
        break;
      case CWIID_MESG_ERROR:
        fprintf(stderr, "E: wii: received error, disconnecting...\n");
        if (cwiid_close(wiimote))
          fprintf(stderr, "Error on wiimote disconnect\n");
        break;
    } //switch msg.type
  } //foreach msg
}

int wii_open()
{
  bdaddr = *BDADDR_ANY;

  if (!(wiimote = cwiid_open(&bdaddr, 0))) {
    fprintf(stderr, "E: wii: Unable to connect to wiimote\n");
    return -1;
  }
  if (cwiid_set_mesg_callback(wiimote, cwiid_callback)) {
    fprintf(stderr, "E: wii: Unable to set message callback\n");
    cwiid_close(wiimote);
    return -1;
  }
  if (cwiid_set_led(wiimote, 0x05)) {
    fprintf(stderr, "Error setting LEDs \n");
    cwiid_close(wiimote);
    return -1;
  }
  if (cwiid_set_rpt_mode(wiimote, CWIID_RPT_BTN | CWIID_RPT_ACC)) {
    fprintf(stderr, "E: wii: could not set report mode\n");
    cwiid_close(wiimote);
    return -1;
  }

  return 0;
}

int wii_close()
{
  if (cwiid_close(wiimote)) {
    fprintf(stderr, "Error on wiimote disconnect\n");
    return -1;
  }
  return 0;
}

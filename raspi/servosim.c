#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

#include "servo.h"
#include "servosim.h"

int servosim_init()
{
  return 0;
}

int servosim_open()
{

  return 0;
}

void servosim_close()
{
  //close(fd);
}

int servosim_setservo ( uint8_t servoNr, uint16_t servoPos )
{
  return 0;
}

int servosim_setleds ( uint8_t onoff, uint8_t leds )
{
  return 0;
}

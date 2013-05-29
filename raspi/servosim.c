#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
//#include <netdb.h>
#include <arpa/inet.h>  //inet_addr

#include "servo.h"
#include "servosim.h"

int fd;
struct sockaddr_in serv_addr;

int servosim_init()
{
  return 0;
}

int servosim_open()
{
	printf ( "N: Servo: I will use TCP to send servo commands to %s:%d\n", SIM_IP, SIM_PORT );

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
  {
    printf ( "E: Servosim: Could not create socket\n" );
    return -1;
  }
  
  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SIM_PORT);
  inet_aton(SIM_IP, &(serv_addr.sin_addr));
 
  if (connect(fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
  {
    printf ( "E: Servosim: Could not connect to %s:%d\n", SIM_IP, SIM_PORT );
    return -1;
  }

	printf ( "N: Servosim: Connected to %s:%d\n", SIM_IP, SIM_PORT );

  return 0;
}

void servosim_close()
{
  close(fd);
}

int servosim_setservo ( uint8_t servoNr, uint16_t servoPos )
{
  int len, buflen;
  char buf[200];

  buflen = snprintf ( buf, 200, "servo set %d %d\n", servoNr, servoPos );
  len = write(fd, buf, buflen);

  if (len!=buflen)
  {
    printf("E: Servosim: setservo: Could not write specified amount of bytes to socketfd\n");
    return -1;
  }

  return 0;
}

int servosim_setleds ( uint8_t onoff, uint8_t leds )
{
  int len, buflen;
  char buf[200];

  buflen = snprintf ( buf, 200, "servo led %d %d\n", onoff, leds );
  len = write(fd, buf, buflen);

  if (len!=buflen)
  {
    printf("E: Servosim: setled: Could not write specified amount of bytes to socketfd\n");
    return -1;
  }

  return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <pthread.h> //for threading , link with lpthread

#include "tcpserver.h"
#include "wiimote.h"
#include "servo.h"
#include "config.h"

void *wii_thread(void * v)
{
  wii_open();
}

int main()
{
  struct tcpserver ts;

  pthread_t wiit;

  servoboard_init();
  servoboard_open();

  if( pthread_create( &wiit , NULL ,  wii_thread , NULL) < 0)
  {
    perror("could not create wii thread\n");
    return 1;
  }

  ts.port = SERVER_PORT;
  printf ( "main: before ts start\n" );
  tcpserver_start ( &ts );
  printf ( "main: after ts start\n" );

  return 0;
}

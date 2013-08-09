#include <stdio.h>
#include <unistd.h>
#include <pthread.h> //for threading , link with lpthread
#include<signal.h>
#include <stdlib.h>

#include "tcpserver.h"
#include "wiimote.h"
#include "servo.h"
#include "config.h"
#include "servofpga.h"

void sig_handler(int signo)
{
  if (signo == SIGINT)
  { 
    printf("received SIGINT\n");
    fclose(logfd);
    exit(0);
  }

}

void *wii_thread(void * v)
{
	while(1)
	{
		if (!wii_connected)
		{
			if (wii_open())
				sleep(2);
		} else {
			sleep(2);
		}
	}
}

void *speedpollthread(void * v)
{
  while(1)
  {
    fpga_pollspeed();
    usleep(100000);
  }
}

int main()
{
  struct tcpserver ts;
  pthread_t wiit;
  pthread_t speedpollt;

  signal(SIGINT, sig_handler);

  servo_init();
  if ( servo_open() )
  {
    printf("E: main: could not open servo interface. Exiting...\n" );
    return -1;
  }

#if WII_ENABLED == 1
  if( pthread_create( &wiit , NULL ,  wii_thread , NULL) < 0)
  {
    perror("E: main: could not create wii thread\n");
    return -1;
  }
	//wii_start_ping_thread();
#endif

#if SERVO_M == SERVO_FPGA
  if( pthread_create( &speedpollt , NULL ,  speedpollthread , NULL) < 0)
  {
    perror("E: main: could not create speedpoll thread\n");
    return -1;
  }
#endif

  ts.port = SERVER_PORT;
  tcpserver_start ( &ts );

	printf("E: main: tcpserver returned. This should not happen\n" );

  return 0;
}

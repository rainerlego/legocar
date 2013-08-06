#ifndef TS_H
#define TS_H

#include<sys/socket.h>
#include<arpa/inet.h>  //inet_addr

struct tcpserver {
  int port;
};

#define TSMP 7
#define TSMPL 50

struct cconn {
  int fd;
	struct sockaddr_in sin;
  char s[TSMP][TSMPL];
  int l[TSMP];
  int curp;
  int curpp;
};


int tcpserver_start ( struct tcpserver * ts );
#endif

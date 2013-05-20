#ifndef TS_H
#define TS_H

struct tcpserver {
  int port;
};

#define TSMP 7
#define TSMPL 50

struct cconn {
  int fd;
  char speicher[TSMP][TSMPL];
  int l[TSMP];
  int curp;
  int curpp;
};


int tcpserver_start ( struct tcpserver * ts );
#endif

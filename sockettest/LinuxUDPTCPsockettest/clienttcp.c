


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg){
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]){
  printf("hallo\n");
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  portno = 4363;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");


  //server = gethostbyname("10.0.0.7");
  //if (server == NULL)
  //{
  //  fprintf(stderr,"ERROR, no such host");
  //  exit(0);
  //}
  
  bzero((char *) &serv_addr, sizeof(serv_addr));

  //bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_family = AF_INET;
  inet_aton("10.0.0.7", &serv_addr.sin_addr.s_addr);
  serv_addr.sin_port = htons(portno);
 
  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  n = write(sockfd, "Hello World from TCP!\n", 22);
  if (n < 0)
    error("ERROR writing to socket");
  printf("written %d bytes of data to socket\n", n);
}

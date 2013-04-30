

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg){
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]){
  int sockfd; //socket file descriptor
  int newsockfd; //connected socket file descriptor
  int portno; //port to listen to
  int clilen; //client address length
  int n;  //nomber of bytes read/written

  char buffer[256];

  struct sockaddr_in dest_addr;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0){
    error("ERROR opening socket");
  }

  bzero((char *) &dest_addr, sizeof(dest_addr));
  portno = 4363;

  dest_addr.sin_family = AF_INET;

  dest_addr.sin_port = htons(portno);

  inet_aton("127.0.0.1", &dest_addr.sin_addr.s_addr);

  //if ( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ){
  //  error("ERROR on binding");
  //}
  
  sendto(sockfd, "Hello World!\n", 13, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

}

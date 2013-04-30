
#include <pthread.h>

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

void * socket_listener(void * msockfd){
  //printf("Hello i am listening on a socket!\n");
  int sockfd = * ((int*)msockfd);
  int n;  //number of bytes read/written
  char buffer[256];
  while(1){
    //n = read(sockfd, buffer, 256);
    struct sockaddr_in sender_addr;

    socklen_t addrlen;
    n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *)&sender_addr, &addrlen);

    int ipAddr = sender_addr.sin_addr.s_addr;
    char str[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
    printf(" from %s : ", str);

    if(n==0){
      printf(" client disconnected!\n");
      free( (int*)msockfd );
      return;
    }
    //printf(" read returned %d", n);
    int i;
    for(i=0; i<n; i++){
      printf("%c",  buffer[i]);
    }
    fflush(stdout); // lol stdout normally only gets written to, when newline character appears -> force flush

  }
}

int create_udp_socket(){
  int sockfd; //socket file descriptor
  int newsockfd; //connected socket file descriptor
  int portno; //port to listen to
  int clilen; //client address length


  struct sockaddr_in serv_addr, cli_addr;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0){
    error("ERROR opening socket");
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = 4363;

  serv_addr.sin_family = AF_INET;

  serv_addr.sin_port = htons(portno);

  serv_addr.sin_addr.s_addr = INADDR_ANY;

  if ( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ){
    error("ERROR on binding");
  }
  
  return sockfd;
}

void * tcp_master_listener(void * msockfd){
  int sockfd = * ((int*)msockfd);
  printf("listening for tcp connections...\n");
  while(1){
    listen(sockfd,5);
    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);

    int * clientsocketfd = malloc(sizeof(int));

    *clientsocketfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (*clientsocketfd < 0)
      error("ERROR on accept");

    pthread_t t_tcp_client;
    int ipAddr = cli_addr.sin_addr.s_addr;
    char str[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
    printf(" %s conected!\n", str);
    pthread_create(&t_tcp_client, NULL, socket_listener, (void*)clientsocketfd );
    
  }

}

int create_tcp_socket(){
  int sockfd; //socket file descriptor
  int newsockfd; //connected socket file descriptor
  int portno; //port to listen to
  int clilen; //client address length

  struct sockaddr_in serv_addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  bzero((char *) &serv_addr, sizeof(serv_addr));

  portno = 4363;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portno);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr *) &serv_addr,                   sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  printf("new tcp socket created\n");
  return sockfd;
}

int main(int argc, char *argv[]){

  int sockudp =create_udp_socket();
  pthread_t t_socket_listener;
  pthread_create(&t_socket_listener, NULL, socket_listener, (void*)&sockudp );

  int socktcp =create_tcp_socket();
  pthread_t t_tcp_listener;
  pthread_create(&t_tcp_listener, NULL, tcp_master_listener, (void*)&socktcp );
  

  pthread_join(t_socket_listener, NULL);
  pthread_exit(NULL);
}

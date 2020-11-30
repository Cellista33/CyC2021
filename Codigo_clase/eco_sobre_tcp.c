#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinetlin.h>
#include<arpa/inet.h>
#define SERV_TCP_PORT 7500 //< Puerto donde está el servidor de eco 
#define SERV_HOST_ADDR "157.88.201.95" //< Host donde se encuentra el proceso servidor
#define MAXLINE 512   //< Tamaño máximo de la cola de mensajes que se puede mandar
char *pname; //< 
main(argc, argv)
int argc;
char argv[];
{
  int sockfd; //< Variable utilizada para almacenar el valor de devuelven las funciones asociadas al socket
  struct sockaddr_in serv_addr;
  char DATA[MAXLINE];
  pname = argv[0];
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
  serv_addr.sin_port = htons(SERV_TCP_PORT);
  if ( (sockfd = socket( ) ) < 0)
  perror("client: can't open stream socket");
  if (connect( ) < 0)
  perror("client: can't connect to server");
  printf("INTRODUCIR MENSAJE: \n");
  gets(DATA);
  if (write(sockfd, DATA, sizeof(DATA) < 0)
  perror("writing on stream socket");
  if (read(sockfd, DATA, sizeof(DATA) < 0)
  perror("reading stream message");
  printf("eco recibido: %s\n", DATA);
  close(sockfd);
  exit(0);
}

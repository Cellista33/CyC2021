#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#define SERV_TCP_PORT 7500 //< Puerto donde está el servidor de eco
#define SERV_HOST_ADDR "157.88.201.95" //< Host donde se encuentra el proceso servidor
#define MAXLINE 512   //< Tamaño máximo de la cola de mensajes que se puede mandar
char *pname; //<
main(argc, *argv)
int argc;
char argv[];
{
  int sockfd, tam; //< Variable utilizada para almacenar el valor de devuelve socket, que es un descriptor de fichero
  struct sockaddr_in serv_addr;
  char DATA[MAXLINE];
  pname = argv[0];
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); //<La función inet_addt pasa un string a la notación de punto utilizada en IPv4
  serv_addr.sin_port = htons(SERV_TCP_PORT);




  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0){
    perror("client: can't open stream socket");

  }

  if (connect(sockfd,  (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
    perror("client: can't connect to server");
  }

  printf("INTRODUCIR MENSAJE: \n");
  fgets(DATA, MAXLINE, stdin);
  printf ("\n Ha introducido algo ");
  if ((tam = write(sockfd, DATA, sizeof(DATA))) < 0){
      perror("writing on stream socket");
  }
  printf("\nse ha enviado un mensaje de %d bytes.", tam);

  if (read(sockfd, DATA, sizeof(DATA)) < 0){
    perror("reading stream message");
    printf("eco recibido: %s\n", DATA);

  }

printf("eco recibido: %s\n", DATA);


  close(sockfd);
  exit(0);
}

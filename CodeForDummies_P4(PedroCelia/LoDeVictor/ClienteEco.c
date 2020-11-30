/*
* Ejemplo de cliente de eco sobre el protocolo TCP.
*/
#include <unistd.h> //perror
#include <stdlib.h>

#include <strings.h> //bzero

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  //inet_addr
#define SERV_TCP_PORT 7500
#define SERV_HOST_ADDR "157.88.201.95"
#define MAXLINE 512
char *pname;

main(int argc,char *argv[])
{
	printf("Servidor: %s , Puerto: %d \n", SERV_HOST_ADDR , SERV_TCP_PORT );

    //conectamos el socket
    int sockfd;
    char DATA[MAXLINE];
    char DATABACK[MAXLINE];
    pname = argv[0];
    struct sockaddr_in serv_addr;

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror("Error al crear el socket");
    else
        printf("Socket creado correctamente \n" );

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr) ) < 0)
        perror("client: can't connect to server");
    else
        printf("Conexión con el servidor establecida \n" );
    //escribimos el mensaje
    printf("Introduzca un mensaje: \n");
    fgets(DATA, MAXLINE, stdin); //gets esta obsoleto, ahora se usa fgets que tiene mas variables
    //enviamos el mensaje
    if (write(sockfd, DATA, sizeof(DATA)) < 0)
    	perror("writing on stream socket");

    //leemos el mensaje
    if (read(sockfd, DATABACK, sizeof(DATABACK)) < 0)
    	perror("reading stream message");

    //sacamos por pantalla el mensaje
    printf("mensaje recibido: %s\n", DATABACK);

    close(sockfd);

    exit(0);
}


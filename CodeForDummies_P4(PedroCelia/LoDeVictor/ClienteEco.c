/*
* Ejemplo de cliente de eco sobre el protocolo TCP.
*/
#include <stdlib.h>     // Biblioteca estándar de C
#include <unistd.h>     // Permite usar perror
#include <strings.h>    // Permite usar bzero
#include <stdio.h>      // Entradas y salidas
#include <sys/types.h>  // Definiciones de tipos de datos

// Bibliotecas para comunicacion por sockets
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Definiciones de datos para conexión
#define SERV_TCP_PORT 7500
#define SERV_HOST_ADDR "157.88.201.95"
#define MAXLINE 512
char *pname;

main(int argc,char *argv[])
{
	printf("Servidor: %s , Puerto: %d \n", SERV_HOST_ADDR , SERV_TCP_PORT );

    // Declaraciones
    int sockfd;
    char DATA[MAXLINE];
    char DATABACK[MAXLINE];
    pname = argv[0];
    struct sockaddr_in serv_addr;

    // Inicialización de la estructura serv_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    // Creacion del socket y tratamiento de errores
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror("Error al crear el socket");
    else
        printf("Socket creado correctamente \n" );

    // Conexion con el socket y tratamiento de errores
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr) ) < 0)
        perror("client: can't connect to server");
    else
        printf("Conexión con el servidor establecida \n" );

    //Introducción del mensaje para ECO por teclado
    printf("Introduzca un mensaje: \n");
    fgets(DATA, MAXLINE, stdin);

    // Escritura en socket
    if (write(sockfd, DATA, sizeof(DATA)) < 0)
    	perror("writing on stream socket");

    // Lectura del socket
    if (read(sockfd, DATABACK, sizeof(DATABACK)) < 0)
    	perror("reading stream message");

    // Mostrar mensaje por pantalla
    printf("mensaje recibido: %s\n", DATABACK);

    // Cierre de socket
    close(sockfd);

    exit(0);
}
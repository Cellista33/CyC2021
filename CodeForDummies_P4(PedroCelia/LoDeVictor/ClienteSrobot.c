//----------------------------------------------------------------------------//
//	    ClienteSrobot: al recibir una petición de un cliente devuelve         //
//                     información sobre los datos del robot		  	          //
//----------------------------------------------------------------------------//

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
#define SERV_TCP_PORT 8500              // Pueto usado
#define SERV_HOST_ADDR "157.88.201.95"  // Dirección del host
#define MAXLINE 512                     // Tamaño máximo del mensaje
char *pname;                            // Mensaje

// Estructura de coordenadas de posición
struct posicion
{
    float X;
    float Y;
};

// Estructura para almacenar datos del robot
struct datos_robot {
    char modo;
    char motor_activo;
    char tipo_interpolacion_G;
    float vel_F;
    float radio_giro_R;
    struct posicion pos_XY_final;
    struct posicion pos_XY_cmd;
    struct posicion pos_XY_real;
};

int main (int argc, char *argv[])
{
    // Declaraciones
    int sockfd;                   // Socket
    struct sockaddr_in serv_addr; // Estructura tipo sockaddr_in
    struct datos_robot DATABACK;  // Almacenamiento de datos robot
    char BUFFER[MAXLINE];         // Tamaño reservado
    pname = argv[0];              // Recibe entradas por teclado

    bzero ((char *) &serv_addr, sizeof(serv_addr));         // Inicialización a 0 de la estructura
    serv_addr.sin_family = AF_INET;                         // Protocolo de la famila IPv4
    serv_addr.sin_addr.s_addr = inet_addr (SERV_HOST_ADDR); // Dirección del host convertida
    serv_addr.sin_port = htons(SERV_TCP_PORT);              // Número de puerto convertido

    // Se intenta crear el socket
     if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("ERROR al crear el socket\n");
        exit(EXIT_FAILURE);
     }
    else 
        printf("Socket creado correctamente\n" );

    // Se intenta conectar con el socket
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr) ) < 0) {
        perror("No se ha podido conectar con el servidor\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Conexión establecida con el servidor\n" );

    // Se lee el mansaje recibido desde el socket
    if (read(sockfd, &DATABACK, sizeof(DATABACK)) < 0)
    	perror("Leyendo mensaje...\n");

    // Conversión de datos de big a little endian
    DATABACK.vel_F = ntohs(DATABACK.vel_F);
    DATABACK.radio_giro_R = ntohs(DATABACK.radio_giro_R);
    DATABACK.pos_XY_final.X = ntohs(DATABACK.pos_XY_final.X);
    DATABACK.pos_XY_final.Y = ntohs(DATABACK.pos_XY_final.Y);
    DATABACK.pos_XY_cmd.X = ntohs(DATABACK.pos_XY_cmd.X);
    DATABACK.pos_XY_cmd.Y = ntohs(DATABACK.pos_XY_cmd.Y);
    DATABACK.pos_XY_real.X = ntohs(DATABACK.pos_XY_real.X);
    DATABACK.pos_XY_real.Y = ntohs(DATABACK.pos_XY_real.Y);

    // Sacamos datos por pantalla

    // Modo de funcionamiento
    printf("Modo: ");
    if (DATABACK.modo == 'x')
    printf("Ningun modo activo\n");
    if (DATABACK.modo == 'm')
    printf("Manual sin HOME\n");
    if (DATABACK.modo == 'M')
    printf("Manual con HOME\n");
    if (DATABACK.modo == 'D')
    printf("MDI\n");
    if (DATABACK.modo == 'A')
    printf("Automatico\n");
    printf("\nMotor activo: ");

    // Motor activo
    if (DATABACK.motor_activo == '0')
    printf ("Ningun motor activo\n");

    if (DATABACK.motor_activo == '1' || DATABACK.motor_activo == '2' || DATABACK.motor_activo == '3')
    printf ("El motor %s esta activo\n", DATABACK.motor_activo );

    if (DATABACK.motor_activo == '4')
    printf ("Todos los motores activos\n");

    // Informacion de la trayectoria
    printf("\nTipo de interpolacion G: %c\n", DATABACK.tipo_interpolacion_G);
    printf("\nVelocidad lineal: %f\n", DATABACK.vel_F);
    printf("\nRadio de giro: %f\n", DATABACK.radio_giro_R);
    printf("\nPosicion final de la trayectoria actual: ( %f , %f )\n", DATABACK.pos_XY_final.X, DATABACK.pos_XY_final.Y);
    printf("\nPosicion interpolada: ( %f , %f )\n", DATABACK.pos_XY_cmd.X, DATABACK.pos_XY_cmd.Y);
    printf("\nPosicion real: ( %f , %f )\n", DATABACK.pos_XY_real.X, DATABACK.pos_XY_real.Y);

    // Cerramos el socket y finalizamos el programa
    close (sockfd);
    printf ("Conexion cerrada\n");
    exit(0);
}
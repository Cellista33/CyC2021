#include "header.h"

int main (int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in serv_addr;
    struct datos_robot RESPUESTA;
    char BUFFER[MAXLINE];
    pname = argv[0];

    bzero ((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr (SERV_HOST_ADDR);
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    //Apertura del socket
     if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror("Error al crear el socket");
    else
        printf("Socket creado correctamente \n" );

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr) ) < 0)
        perror("client: can't connect to server");
    else
        printf("Conexión con el servidor establecida \n" );
        //Recibo la informaci�n desde el servidor srobot
    if (read(sockfd, &RESPUESTA, sizeof(RESPUESTA)) < 0)
    	perror("reading stream message");


    //Conversion de los datos float recibidos de big endian a little endian
    RESPUESTA.vel_F = reverseFloat(RESPUESTA.vel_F);
    RESPUESTA.radio_giro_R = reverseFloat(RESPUESTA.radio_giro_R);
    RESPUESTA.pos_XY_final.X = reverseFloat(RESPUESTA.pos_XY_final.X);
    RESPUESTA.pos_XY_final.Y = reverseFloat(RESPUESTA.pos_XY_final.Y);
    RESPUESTA.pos_XY_cmd.X = reverseFloat(RESPUESTA.pos_XY_cmd.X);
    RESPUESTA.pos_XY_cmd.Y = reverseFloat(RESPUESTA.pos_XY_cmd.Y);
    RESPUESTA.pos_XY_real.X = reverseFloat(RESPUESTA.pos_XY_real.X);
    RESPUESTA.pos_XY_real.Y = reverseFloat(RESPUESTA.pos_XY_real.Y);

    ///Mostramos los datos por pantalla
    printf("Modo: ");
    if (RESPUESTA.modo == 'x')
    printf("Ningun modo activo\n");
    if (RESPUESTA.modo == 'm')
    printf("Manual sin HOME\n");
    if (RESPUESTA.modo == 'M')
    printf("Manual con HOME\n");
    if (RESPUESTA.modo == 'D')
    printf("MDI\n");
    if (RESPUESTA.modo == 'A')
    printf("Automatico\n");
    printf("\nMotor activo: ");

    if (RESPUESTA.motor_activo == '0')
    printf ("Ningun motor activo\n");

    if (RESPUESTA.motor_activo == '1' || RESPUESTA.motor_activo == '2' || RESPUESTA.motor_activo == '3')
    printf ("El motor %s está activo\n",RESPUESTA.motor_activo );

    if (RESPUESTA.motor_activo == '4')
    printf ("Todos los motores activos\n");

    printf("\nTipo de interpolacion G: %c\n", RESPUESTA.tipo_interpolacion_G);
    printf("\nVelocidad lineal: %f\n", RESPUESTA.vel_F);
    printf("\nRadio de giro: %f\n", RESPUESTA.radio_giro_R);
    printf("\nPosicion final de la trayectoria actual: ( %f , %f )\n", RESPUESTA.pos_XY_final.X, RESPUESTA.pos_XY_final.Y);
    printf("\nPosicion interpolada: ( %f , %f )\n", RESPUESTA.pos_XY_cmd.X, RESPUESTA.pos_XY_cmd.Y);
    printf("\nPosicion real: ( %f , %f )\n", RESPUESTA.pos_XY_real.X, RESPUESTA.pos_XY_real.Y);

    close (sockfd);
    printf ("conexión cerrada\n");

    exit(0);
}
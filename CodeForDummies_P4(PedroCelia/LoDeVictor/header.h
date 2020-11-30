#ifndef CABECERA_H_INCLUDED
#define CABECERA_H_INCLUDED

#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SERV_TCP_PORT 8500 // puerto de estado del robot en el servidor
#define SERV_HOST_ADDR "157.88.201.95" //IP de Gredos
#define MAXLINE 512 //tamaÃ±o mÃ¡ximo del mensaje
char *pname;

float reverseFloat (const float floatBE)
{
    float floatLE;
    char *floatBE_char = (char *)&floatBE;
    char *floatLE_char = (char *) &floatLE;

    ///Intercambio los bytes
    floatLE_char[0] = floatBE_char [3];
    floatLE_char[1] = floatBE_char [2];
    floatLE_char[2] = floatBE_char [1];
    floatLE_char[3] = floatBE_char [0];
    return floatLE;
}
struct posicion
{
    float X;
    float Y;
};
struct datos_robot{
    char modo;
    char motor_activo;
    char tipo_interpolacion_G;
    float vel_F;
    float radio_giro_R;
    struct posicion pos_XY_final;
    struct posicion pos_XY_cmd;
    struct posicion pos_XY_real;
};
#endif // CABECERA_H_INCLUDED

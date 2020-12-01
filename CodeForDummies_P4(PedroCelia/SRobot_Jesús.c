///////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------//
//-------------------------------------------Práctica 4------------------------------------------//
//------------------------------Control y Comunicaciones Industriales----------------------------//
//----------------------Jesús Santamaría Mateos---Iván Rodríguez Gobernado-----------------------//
//---------------------------------------Curso 2020/21-------------------------------------------//
//---------------------------------------Cliente_Robot-------------------------------------------//
//-----------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////


#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<time.h>

#define SERV_TCP_PORT 8500 //Puerto para el socket (UDP)
#define SERV_HOST_ADDR "157.88.201.95" //IP de destino
#define MAXLINE 512  //Máxima cantidad de datos

float ntohf (float);
time_t ntohft (time_t);

//---------------------------------------------------------------------------------------------//
struct posicion
{
    float X;
    float Y;
};
//---------------------------------------------------------------------------------------------//


struct datos_robot //Definición estructura de datos del robot
{
  char modo; //
  char motor_activo;
  char tipo_interpolacion_G;
  float vel_F;
  float radio_giro_R;
  struct posicion pos_XY_final;
  struct posicion pos_XY_cmd;
  struct posicion pos_XY_real;
  struct timeval timestamp;
};

//---------------------------------------------------------------------------------------------//
int main()
{


  int sockfd; //Almacen de la información del socket(Debe ser un entero)
  struct sockaddr_in serv_addr; //Declaración de la estructura para la conexión
  char DATA[MAXLINE]=" ";//Definición del bloque de datos
  char BUFFER[MAXLINE];//Definición de bufer
  struct datos_robot datos;


  static const struct sockaddr_in EmptyStruct;//Creación de una estructura vacía
  struct sockaddr from;

  serv_addr=EmptyStruct;//Se iguala la estructura del socket con una estructura vacía
  serv_addr.sin_family = AF_INET;//Asignación del tipo de identificador a usar
  serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);//Establecimiento de la IP del socket
  //inet_addr() transforma una dirección en formato IPv4 (000.000.000.000)a un formato binario
  //con el mismo orden de bytes que la red (big_endian)
  serv_addr.sin_port = htons(SERV_TCP_PORT);//Asignación del puerto a emplear
  int length_from=sizeof(from);

  if ((sockfd = socket(serv_addr.sin_family,  SOCK_DGRAM, 0) ) < 0)
  {
    //Asignación del puerto del socket a la variable homónima. Necesita de:
    //Dominio (Domínio que se va a emplear, en este caso IPv4)
    //Tipo: tipo de puerto a emplear (TDP/UDP) ---->(SOCK_STREAM/SOCK_DGRAM)-->SOCK_DGRAM
    //Protocolo: protocolo que va ha ser empleado para la transmisión (IP=0)
      perror("client: can't open stream socket");//Error al establecer la conexión con el socket
  }

  //printf("INTRODUCIR MENSAJE: \n");
  //gets(DATA);//Almcenamiento del dato a transmitir
  //Lectura/escritura a través del socket Tratamiento de errores;
  //===========================================================================================//

  if (sendto(sockfd, DATA, sizeof(DATA), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
  //Envio del paquete al socket
  //Socket empleado--Dato a enviar -- Longitud del dato --
  {
      perror("writing on stream socket");//Error en la escritura a través del socket
  }
  if (recvfrom(sockfd,(struct datos_robot*) &datos, sizeof(datos), 0, (struct sockaddr *) &from, &length_from) < 0)
  {
      perror("reading stream message");//Error en la lectura a través del socket
  }
  //===========================================================================================//
  //datos=DATA;
  //  printf("%d\n", BUFFER);
  close(sockfd);//Liberación del socket
  printf("Modo: %c\n", datos.modo);
  printf("Motor activado: %c\n", datos.motor_activo);
  printf("Tipo de interpolacion: %c\n", datos.tipo_interpolacion_G);
  printf("Velocidad: %f\n", ntohf(datos.vel_F));
  printf("Radio de giro %f\n", ntohf (datos.radio_giro_R));
  printf("Posicion final: \n");
  printf("          X = %f\n", ntohf (datos.pos_XY_final.X));
  printf("          Y = %f\n", ntohf (datos.pos_XY_final.Y));
  printf("Posicion comandada:\n");
  printf("          X = %f\n", ntohf (datos.pos_XY_cmd.X));
  printf("          Y = %f\n", ntohf (datos.pos_XY_cmd.Y));
  printf("Posicion real: \n");
  printf("          X = %f\n", ntohf (datos.pos_XY_real.X));
  printf("          Y = %f\n", ntohf (datos.pos_XY_real.Y));
  time_t tiempo;
  
 // uint64_t tiemp;
 // uint64_t t;
 // tiempo=datos.timestamp.tv_sec;
 // t=(unsigned long int)tiempo;
 // t=ntohl(t);
 // tiempo=(time_t)t; 

  
 // printf("Prueba : %s \n", ctime(&tiempo)); 
 // printf("Prueba 2: %ld \n", t); 
 // printf("Timestamp: \n");
  printf("          Secs = %ld \n", ntohft(datos.timestamp.tv_sec));  
  printf("          uSecs = %ld \n", ntohft(datos.timestamp.tv_usec));
  
  return 0;
}


float ntohf (float dato)
{
  int dato_int = ntohl(*(unsigned int*)&dato);
  return *(float*)&dato_int;
}

time_t ntohft (time_t tiempo)
{
  int i=0;
  unsigned char valor[8];
  unsigned char valor2[8];
  time_t tiempob;
  
  for (i=0; i==7; i++)
  {
    valor[i]=tiempo>>8*i;
  }
  i=0;
  for (i=0; i==7; i++)
  {
    valor2[i]=valor[1-i];
  }
  return(tiempob=strtol(valor2, NULL, 10));
}

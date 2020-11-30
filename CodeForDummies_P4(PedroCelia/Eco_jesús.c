///////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------//
//-------------------------------------------Práctica 4------------------------------------------//
//------------------------------Control y Comunicaciones Industriales----------------------------//
//----------------------Jesús Santamaría Mateos---Iván Rodríguez Gobernado-----------------------//
//---------------------------------------Curso 2020/21-------------------------------------------//
//----------------------------------------Cliente_eco--------------------------------------------//
//-----------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////


#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define SERV_TCP_PORT 7500 //Puerto para el socket (TDP)
#define SERV_HOST_ADDR "157.88.201.95" //IP de destino
#define MAXLINE 512  //Máxima cantidad de datos


main()

{
  int sockfd; //Almacen de la información del socket(Debe ser un entero)
  struct sockaddr_in serv_addr; //Declaración de la estructura para la conexión
  char DATA[MAXLINE];//Definición del bloque de datos
  static const struct sockaddr_in EmptyStruct;


  serv_addr=EmptyStruct;//Se rellena toda la estrucutra de ceros
  serv_addr.sin_family = AF_INET;//Asignación del tipo de identificador a usar (IPv4)
  serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);//Establecimiento de la IP del socket
  //inet_addr() transforma una dirección en formato IPv4 (000.000.000.000)a un formato binario
  //con el mismo orden de bytes que la red (big_endian)
  serv_addr.sin_port = htons(SERV_TCP_PORT);//Asignación del puerto a emplear

  //Establecimiento de conexión y tratamiento de errores
  //===========================================================================================//
  if ((sockfd = socket(serv_addr.sin_family, SOCK_STREAM, 0) ) < 0)
  {
    //Asignación del puerto del socket a la variable homónima. Necesita de:
    //Dominio (Domínio que se va a emplear, en este caso IPv4)
    //Tipo: tipo de puerto a emplear (TDP/UDP) ---->(SOCK_STREAM/SOCK_DGRAM)-->SOCK_STREAM
    //Protocolo: protocolo que va ha ser empleado para la transmisión
    //al poner 0 en este campo se emplea la confifuración del socket por defecto (IP)

      perror("client: can't open stream socket");//Error al establecer la conexión con el socket
  }

  if (connect(sockfd,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
  {
    // Conectar el socket con estructura sockaddr, almacenado en serv_addr del tamaño
    // de serv_addr
      perror("client: can't connect to server");//Error en la conexión con el servidor
  }
  //===========================================================================================//

  printf("INTRODUCIR MENSAJE: \n");
  fgets(DATA, sizeof(DATA),stdin);//Almcenamiento del dato a transmitir

  //Lectura/escritura a través del socket Tratamiento de errores;
  //===========================================================================================//

  if (write(sockfd, DATA, sizeof(DATA) < 0))//Escritura del dato en el socket
  {
      perror("writing on stream socket");//Error en la escritura a través del socket
  }
  if (read(sockfd, DATA, sizeof(DATA) < 0))//Lectura del dato en el socket
  {
      perror("reading stream message");//Error en la lectura a través del socket
  }
  //===========================================================================================//


  printf("eco recibido: %s\n", DATA);//Salida por pantalla del mensaje eco
  close(sockfd);//Cierre del socket
  
};

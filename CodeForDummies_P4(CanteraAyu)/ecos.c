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
char *pname; //<Puntero asociado a los argumentos de entrada de main.
main(argc, *argv)
int argc;
char argv[];
{
  int sockfd, tam; //< Variable utilizada para almacenar el valor de devuelve socket, que es un descriptor de fichero
  struct sockaddr_in serv_addr; //< Estructura donde almacenamos la dirección y puerto con la que nos comunicaremos
  char DATA[MAXLINE]; //< definimos la cadena de caracteres donde almacenaremos el mensaje que introducirá el usuario
  pname = argv[0];
  bzero((char *) &serv_addr, sizeof(serv_addr)); //< Ponemos a 0 las direcciones de memoria donde se alberga toda la estructura asociada a las direcciones de memoria y puertos
  serv_addr.sin_family = AF_INET; //<Especificamos que realizaremos una comunicación con otra máquina a traves de internet
  serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); //<La función inet_addt pasa un string a la notación de punto utilizada en IPv4
  serv_addr.sin_port = htons(SERV_TCP_PORT); //< almacenamos el puerto donde se encuentra el servidor con el que nos queremos conectar


  /**
  * \brief socket
  *
  * Función para crear el socket que utilizaremos como método de comunicación
  * @param AF_INET Definimos que la comunicación será a través de inet con una
  * máquina externa
  * @param SOCK_STREAM  Permite mandar el mensaje como un flujo de bytes
  * @return 0=si se ha creado correctamente; -1= ha hbaido algún error
  */
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0){
    perror("client: can't open stream socket");

  }

/**
* @brief <brief>
* @param [in] <sockfd> <Identificadro único del fichero donde almacenaremos las
*                        transmisiones que se hagan por sockets>
*@☺param [in] z
* @return <return_description>
* @details <details>
*/

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

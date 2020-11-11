#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include "usuario1.h"

//Definimos las varibales asociadas a la comunicación

  //Cola de mensajes
  cola_msg  cola_A;
  int ID_colaA;



 /*void almacen( *argv[]){

   cola_A.origen = getpid();
   cola_A.destino = atoi(argv[1]);
   cola_A.modo
 }
*/

void cli_eco(){
  cola_A.opcion=0;
  printf("\n\t\tMODO ECO");

  puts("Introduzca un texto para enviar");
  if (fgets(cola_A.datos, MAXDATA_COLA, stdin) == NULL){
    perror("Error al insertar el texto");
    exit(EXIT_FAILURE);
  }

  saca_datos();
  
  printf("\nEnviando mensaje por la cola");
	msgsnd(ID_colaA, &cola_A, sizeof(cola_msg)-sizeof(long), 0);
	printf("\nEl mensaje se ha enviado por la cola");

	printf("\nEsperando mensaje en modo ECO");
	msgrcv(ID_colaA, (cola_msg *) &cola_A, sizeof(cola_msg) - sizeof(long), (long)getpid(), 0);
		
	/* Se muestra el mensaje recibido de usuario2 */
	printf("\nRecibido desde el Usuario 2:\n");
  saca_datos();

	/* Salimos del programa */
	exit(EXIT_SUCCESS); 

}


void cli_tf(){

  cola_A.opcion=1;

  	puts("\nIntroduzca un texto para enviar: ");

		if (fgets(cola_A.datos, MAXDATA, stdin) == NULL)
		{
			printf("Error al insertar el texto");
			exit(1);
		}
    saca_datos();
    opciones_tf();
    switch (cola_A.cadena){
      case 'dir':



      case 'cd':
        printf("\nIntroduzca el directorio que desea abrir:");
        scanf("%s", cola_A.datos);

      case 'get':
        printf("\nIntroduzca el fichero que desea mover");
        scanf("%s", cola_A.datos);

      case 'quit':
        printf("\nCerrando el programa");
        cierre_cola();

    }

    


}

void inicio_cola(){
  if(( ID_colaA = msgget(CLAVE1, 0 ) < 0){
    perror("No se ha podido obtener la id de la cola");
    exit(EXIT_FAILURE);
  }
}

void saca_datos(){
  /* Mostramos por pantalla parametros interesantes */
	printf("\nEnviado desde el Usuario 1:\n");
	printf("\torigen: %i\n",cola_A.origen);
	printf("\tdestino: %i\n", cola_A.destino);
	printf("\tdatos: %s\n", cola_A.datos);


  
	printf("\n\n");
  
}


void opciones_tf(){
    printf("\n--------------------------------------------------");
    printf("\n\t\tOpciones disponibles:");
    printf("\n\tdir");
    printf("\n\tcd");
    printf("\n\tget");
    printf("\n\tquit");

    printf("\nAccion a realizar:");
    scanf("%s", cola_A.cadena);
    while ((cola_A.cadena != "cd") && ((cola_A.cadena != "get") && (cola_A.cadena != "quit"){
      printf("\nError. Ha introducido un comando no valido.");
      printf("\nIntroduzca una opcion valida:");
      scanf("%s", cola_A.cadena);
    }

  }
  

}
void main (int argc, char *argv[]){

   //comp(arc, *argv[]);

   if(argc < 3 || argc > 4) {
		perror("Numero de argumentos incorrecto");
		/* Salimos del programa indicando fallo al entorno */
		exit(EXIT_FAILURE); 
   }

   //almacen(*argv[]);

   cola_A.origen = getpid();
   cola_A.destino = atoi(argv[1]);
   cola_A.modo = *argv[2];
   cola_A.protocolo = atoi(argv[3]);


  

  if ((argc == 3) || !(strcmp(argv[4], "eco")) || !(strcmp(argv[4], "ECO")) ){
    inicio_cola();
    while(1){
      cli_eco(); //Cliente eco determina la finalización del programa.
    }

  } else if(!(strcmp(argv[4], "ftp")) || !(strcmp(argv[4], "FTP"))) {
    inicio_cola();
    while (1){
      cli_tf(); //Cliente ftp determina la finalización del programa.
    }    
  }

  
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "usuario1.h"

//Definimos las varibales asociadas a la comunicación

  //Cola de mensajes
  cola_msg  cola_A;
  int ID_colaA;
  
  //Datos a almacenar.
  
  struct datos_usuario info;




void cli_eco(){

	//cola_A = cola_A;

	
	cola_A.type = 1L;
  	cola_A.dat_que.cli = 0;
  printf("\n\t\tMODO ECO");

  puts("Introduzca un texto para enviar");  
  //Como no nos ha dado tiempo a poder implementar la segmentación del mensaje
  //	directamente ponemos como límite que lo máxiom que se pueda leer sea el 
  //	límite del campo datos de la estructura (1014 bytes)
  if (fgets(info.data, MAXDATA_U, stdin) == NULL){
    perror("Error al insertar el texto");
    exit(EXIT_FAILURE);
  }
  
  cola_A.dat_que = info;
  
  

  saca_datos();
  
  printf("\nEnviando mensaje por la cola");
	msgsnd(ID_colaA, &cola_A, sizeof(cola_msg)-sizeof(long), 0);
	printf("\nEl mensaje se ha enviado por la cola");
	
	//sleep(10);

	printf("\nEsperando mensaje en modo ECO");
	msgrcv(ID_colaA, (cola_msg *) &cola_A, sizeof(cola_msg) - sizeof(long), (long)getpid(), 0);
		
	/* Se muestra el mensaje recibido de usuario2 */
	printf("\nRecibido desde el Usuario 2:\n");
  	saca_datos();

	/* Salimos del programa */
	//exit(EXIT_SUCCESS); 

}


void cli_tf(){

  cola_A.dat_que.cli=1;
  
  printf("\nEsta opcion no se encuentra disponible");
  exit(0);
/*
  	puts("\nIntroduzca un texto para enviar: ");

		if (fgets(cola_A.datos, MAXDATA_COLA, stdin) == NULL)
		{
			printf("Error al insertar el texto");
			exit(1);
		}
    saca_datos();
    opciones_tf();
    /*
    switch (atoi(cola_A.cadena)){
      case 'atoi(dir)':



      case 'atoi(cd)':
        printf("\nIntroduzca el directorio que desea abrir:");
        scanf("%s", cola_A.datos);

      case 'atoi(get)':
        printf("\nIntroduzca el fichero que desea mover");
        scanf("%s", cola_A.datos);

      case 'atoi(quit)':
        printf("\nCerrando el programa");
        //cierre_cola();

    }
    */

    


}

void inicio_cola(){
	cola_A.type = 1L;
  if (( ID_colaA = msgget(CLAVE1, 0 )) < 0){
    perror("No se ha podido obtener la id de la cola");
    exit(EXIT_FAILURE);
  }
}

void saca_datos(){
  /* Mostramos por pantalla parametros interesantes */
	printf("\nEnviado desde el Usuario 1:\n");
	printf("\torigen: %i\n",cola_A.origen);
	printf("\tdestino: %i\n", cola_A.destino);
	printf("\tdatos: %s\n", cola_A.dat_que.data);


  
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
    }
    
    /* scanf("%s", cola_A.da);
    while ((cola_A.cadena != "cd") && (cola_A.cadena != "get") && (cola_A.cadena != "quit")){
      printf("\nError. Ha introducido un comando no valido.");
      printf("\nIntroduzca una opcion valida:");
      scanf("%s", cola_A.cadena);
    }

  }
*/


int salida(){
	int op;


	printf("\n\n\n\t\t\t Introduzca una de las siguientes opciones:");
	printf("\n\t1 - Realizar otra operacion del mismo tipo");
	printf("\n\t0 - Salir del programa");
	printf("\nOpcion a realizar:");
	scanf("%d", &op);
	
	return(op);
	 
}

void main (int argc, char *argv[]){

	int aux;

  
   printf("\nUsuario 1 iniciado");
   printf("\n Numero de argumentos: %i", argc);
   
   if(argc < 3 || argc > 4) {
		perror("Numero de argumentos incorrecto");
		// Salimos del programa indicando fallo al entorno 
		exit(EXIT_FAILURE); 
   }
	
  

   cola_A.origen = getpid();
   cola_A.destino = atoi(argv[1]);
   cola_A.modo = *argv[2];
   //cola_A.protocolo = atoi(argv[3]);
   
   	printf("\nEnviado desde el Usuario 1:\n");
	printf("\n\torigen: %i\n",cola_A.origen);
	printf("\n\tdestino: %i\n", cola_A.destino);
	printf("\n\tmodo: %c", cola_A.modo);
	//printf("\n\tprotocolo: %i", cola_A.protocolo);
	
   

  if (argc == 3 || !(strcmp(argv[3], "eco")) || !(strcmp(argv[3], "ECO")) ){
    inicio_cola();
    
      cli_eco();
      aux = cola_A.origen;
      cola_A.origen = cola_A.destino;
      cola_A.destino = aux;
      
      
    

  } else if(!(strcmp(argv[3], "ftp")) || !(strcmp(argv[3], "FTP"))) {
    inicio_cola();
    do{
      cli_tf();
    } while(salida() == 1);   
  }
    
}

/************************************************************************************/
/*																					*/
/*								Usuario A:											*/
/*	Tiene el control de la comunicacion pudiendo hacer peticiones de ECO o  		*/
/*  					servicios FTP a un servidor									*/
/*																					*/
/*																					*/
/*																					*/
/************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include "msg.h"
#include "msgq.h"

/* Se crea estructura para la cola */
msg_C msg_c;


void main (int argc, char *argv[]){

	/* Variable que almacenará el ID de la cola 1 */
	int interfaz1;

	/* Comprobamos que el numero de argumentos introducido es correcto */
	if(argc < 4 || argc > 5) {
		printf("Error: el numero de argumentos es incorrecto\n");
		/* Salimos del programa indicando fallo al entorno */
		exit(EXIT_FAILURE); 
	}
	
	/* Guardamos el PID de nuestro Usuario 1 */
	msg_c.msgorigen = getpid(); 
	
	/* Guardamos el PID de Usuario 2 */
	msg_c.msgdestino = atoi(argv[1]); 
	
	/* Guardamos el modo del tipo de IPC usado (SHM o SOCKETS) */
	msg_c.msgmodo = *argv[2]; 

	/* Protocolo unidireccional (1) o bidireccional (2) */
	msg_c.msgprotocolo = atoi(argv[3]); 

	/* Comprobamos si trabajamos en modo ECO o FTP */
	/* Se comprueba si las cadenas son iguales solo si el numero de argumentos es mayor
	 * que 4, sino da un error "segmentation fault (core dumped)" */
	if(argc > 4) {
		if (!(strcmp(argv[4], "ftp")) || !(strcmp(argv[4], "FTP"))) {

			/* Estamos en modo ftp */
			msg_c.msgopcion=1;
			printf("\nModo ftp.");
		}
	}
	else{
		/* Es una funcion de eco, porque tenemos solo 4 argumentos (el nombre del programa es el 0) */
		msg_c.msgopcion=0;
		printf("\nModo eco.");
	}

	/* Nos intentamos conectar a la cola 1 */
	if ( (interfaz1 = msgget(MKEYQ1, 0)) <0) {
		printf("El Usuario 1 no se ha podido conectar a la cola 1");
		exit(EXIT_FAILURE);
	}

	/*Bucle maestro de programa*/
	while(1){
			/* Por defecto antes de cada bucle se inicia el flag de final */
	msg_c.msgfinal = 0;
	/* SE FIJA EL CAMPO TIPO EN SENTIDO USUARIO -> SERVIDOR */
	msg_c.msgtype = 1L;    

	if (msg_c.msgopcion){
		/* Modo FTP*/
		
		printf("\nIntroduzca que funcion desea realizar:\n\t=>dir\n\t=>cd\n\t=>get\n\t=>quit");
		printf("\nSu comando es: ");
		/* Leemos por teclado un texto */
		scanf("%s",msg_c.msgcadena);

		/*Si se introduce un comando no valido se pide uno de nuevo */
		while(strcmp(msg_c.msgcadena, "cd") && strcmp(msg_c.msgcadena, "dir") && strcmp(msg_c.msgcadena,"get") && strcmp(msg_c.msgcadena, "quit")){
			printf("La funcion introducidad no es correcta");
			printf("\nPor favor, introduzca una de estas:\n\t=>dir\n\t=>cd\n\t=>get\n\t=>quit");
			printf("\nSu comando es:");
			scanf("%s",msg_c.msgcadena);
		}

		/* Actuamos en funcion del comando introducido */
		/* Si el comando es "cd" */
		if(strcmp(msg_c.msgcadena,"cd") == 0) {
			printf("\nIntroduzca el directorio que desea abrir: ");
			scanf("%s",msg_c.msgdatos);

		}
		
		/* Si el comando es "get" se pide el nombre del fichero */
		if(strcmp(msg_c.msgcadena,"get") == 0 ){ // Si introducimos get por teclado
			printf("\nIntroduzca el directorio que desea abrir y que te envien los archivos que contiene: ");
			scanf("%s",msg_c.msgdatos);

		}
		
		/* Si el comando es "quit" se envia para el cierre de todos y se cierra*/
		if(strcmp(msg_c.msgcadena,"quit") == 0 ){ //Si introducimos quit
			printf("\nSe cerrara el programa");
			msgsnd(interfaz1, &msg_c, sizeof(msg_C)-sizeof(long), 0);
			sleep(2);
			exit(0);
		}

	}
	else{
		/* Modo eco */
		puts("\nIntroduzca un texto para enviar: ");

		/* Si el tamaño introducido es mayor que es admitido ERROR */
		if (fgets(msg_c.msgdatos, MAXDATA, stdin) == NULL)
		{
			printf("Error al insertar el texto");
			exit(1);
		}
	}

	/* Mostramos por pantalla parametros interesantes */
	printf("\nEnviado desde el Usuario 1:\n");
	printf("\torigen: %i\n",msg_c.msgorigen);
	printf("\tdestino: %i\n", msg_c.msgdestino);
	printf("\tdatos: %s\n", msg_c.msgdatos);

	printf("\n\n");

	/* Enviamos la trama por la cola */
	printf("\nEnviando mensaje por la cola");
	msgsnd(interfaz1, &msg_c, sizeof(msg_C)-sizeof(long), 0);
	printf("\nEl mensaje se ha enviado por la cola");

	/* Hasta que en el campo msg_c.msgopcion no tengo un 1 imprime por pantalla */
	if (msg_c.msgopcion){
		/* MODO FTP */
		printf("\nEsperando mensaje en modo FTP");
		
		/* Se recibe mensaje por la cola */
		printf("\nEsperando mensaje en la cola");
		msgrcv(interfaz1, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), (long)getpid(), 0);
		
		/* Se muestra el mensaje recibido de usuario2 */
		printf("\nRecibido desde el Usuario 2:\n");
		printf("\torigen: %i\n",msg_c.msgorigen);
		printf("\tdestino: %i\n", msg_c.msgdestino);
		printf("\tdatos: %s\n", msg_c.msgdatos);

		/* Se recibe mensaje por la cola hasta que no llegue el ultimo mensaje */
		while (msg_c.msgfinal !=1){
			msgrcv(interfaz1, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), (long)getpid(), 0);
			printf("\t       %s\n", msg_c.msgdatos);
		}
		
	}
	else{
		/* MODO ECO */
		
		/* Se recibe mensaje por la cola */
		printf("\nEsperando mensaje en modo ECO");
		msgrcv(interfaz1, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), (long)getpid(), 0);
		
		/* Se muestra el mensaje recibido de usuario2 */
		printf("\nRecibido desde el Usuario 2:\n");
		printf("\torigen: %i\n",msg_c.msgorigen);
		printf("\tdestino: %i\n", msg_c.msgdestino);
		printf("\tdatos: %s\n", msg_c.msgdatos);
		
		/* Salimos del programa */
		exit(EXIT_SUCCESS); 

	}

	/* Se vuelve a dar la vuelta al pid de tu proceso origen y tu proceso destino */
	msg_c.msgtype=msg_c.msgdestino;
	msg_c.msgdestino=msg_c.msgorigen;
	msg_c.msgorigen=msg_c.msgtype;
	}
}



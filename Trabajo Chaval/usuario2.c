/************************************************************************************/
/*																					*/
/*								Usuario B:											*/
/*	Recibe por la cola lo que le envia la entidad 2, lo trata, si es de eco 		*/
/*  da la vuelta al origen y destino y lo envia, y si es ftp, depende				*/
/*	de si es cd, dir, get, hace un ls y lo manda, abre un fichero y lo manda,		*/
/*  o abre un un directorio, lo que manda lo manda a traves de la cola				*/
/*	a la entidadB, para que le llegue al usuario1.									*/
/*																					*/
/************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include "msg.h"
#include "msgq.h"

/* Se crea estructura para la cola */
msg_C msg_c; 

void main ()
{

	/* Variable que almacenará el ID de la cola 2 */
	int interfaz2;
	
	/* Mostramos el pid del usuario2 por pantalla */
	printf("El pid del usuario B es: %d\n", getpid());

	/* Se intenta conectar a la cola 2 con la entidad 2*/
	if( (interfaz2 = msgget(567L, 0)) < 0 ){
		perror ("Servidor: No pudo conectarse a la cola");
		exit(1);
	}

	/* se fija el campo tipo en el sentido Entidad 2 -> usuario B */
	msg_c.msgtype=getpid();
	
	while(1){	
		/* Como se usan funciones dir, tenemos que declarar un puntero 
		 * para que apunte a dir, y una estructura */
		DIR *fdir;
		struct dirent directorio, *p_dir;
		p_dir = &directorio;

		/* Recibe mensajes por la cola de mensajes */
		printf("\nEsperando a que la entidad2 me envie cosas.");
		msgrcv(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), (long)getpid(), 0);
		printf("\nRecibido por la cola de entidad 2.");
		
		/* Invierte el sentido del flujo de la cola */
		msg_c.msgtype=msg_c.msgorigen;
		msg_c.msgorigen=msg_c.msgdestino;
		msg_c.msgdestino=msg_c.msgtype;
		msg_c.msgtype=2L;

		/* Si es un eco */ 
		if(msg_c.msgopcion == 0){

			/* Se muestran por pantalla los resultados si es de tipo eco */
			printf("\nModo ECO."); 

			/* Se envian los datos */
			msgsnd(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), 0);
			printf("\nDatos enviado a la entidadB.");

			/* Y se envía el ultimo mensaje */
			msg_c.msgfinal=1;
			strcpy(msg_c.msgdatos,"");
			msgsnd(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), 0);

		}
		/* Si es ftp */
		else{
			
			/* El flag del mensaje se resetea */
			msg_c.msgfinal = 0;

			/* Si recibe un "quit" se cierra el programa */
			if(strcmp(msg_c.msgcadena,"quit") == 0) {
				exit(0);
			}

			/* Si se recibe "cd" */
			if(strcmp(msg_c.msgcadena,"cd") == 0) {

				/* Abrimos el directorio que nos ha dicho el usuario */
				printf("Abriendo el directorio que ha introducido el usuario: %s",msg_c.msgdatos);
				chdir(msg_c.msgdatos);
				fdir = opendir(".");
				
				/* Se muestra por pantalla del usuario B lo que hay en el directorio
				 * para que se vea que se ha cambiado de directorio */
				while ((p_dir = readdir(fdir)) != NULL){
					printf("%s\n", p_dir->d_name);
					fflush(stdout);
				}

				/* Se comprueba que se ha abierto bien el directorio */
				if(chdir(msg_c.msgdatos)==0){
					
					/* Se envia por la cola que se ha abierto correctamente el directorio */
					strcpy(msg_c.msgdatos,"Directorio abierto correctamente");
					msgsnd(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), 0);
					
					/* Se envia el mensaje final */
					msg_c.msgfinal=1;
					strcpy(msg_c.msgdatos,"");
					msgsnd(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), 0);
					printf("\n\nEl directorio se ha abierto correctamente\n\n");
				}
				else{
					/* Se envia el mensaje de mala apertura */
					strcpy(msg_c.msgdatos,"El fichero se ha podido abrir");
					msgsnd(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), 0);
					
					/* Se envia el mensaje final */
					msg_c.msgfinal=1;
					strcpy(msg_c.msgdatos,"");
					msgsnd(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), 0);
					printf("\nSe ha podido abrir el directorio\n");
				}

			}else
				/* Si se recibe "dir" */
				if(strcmp(msg_c.msgcadena,"dir") == 0 ){
					
					fdir = opendir(".");
					
					while ((p_dir = readdir(fdir)) != NULL){
						/* Se muestran los elementos del directorio actual */
						printf("%s\n", p_dir->d_name);
						strcpy(msg_c.msgdatos,p_dir->d_name);
						fflush(stdout);
						/* Se envian por la cola */
						msgsnd(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), 0);
					}

					if((p_dir = readdir(fdir))== NULL){
						/* Se envia el mensaje de fin */
						strcpy(msg_c.msgdatos,"");
						msg_c.msgfinal=1;
						msgsnd(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), 0);
					}
					
					printf("\nDatos enviados al usuario1.");
					
					/* Se cierra el directorio */
					closedir(fdir);			

				
				}else{

					/* Si se recibe "get" */
					chdir(msg_c.msgdatos);
					fdir = opendir(".");
					
					/* Se comprubea que el archivo exista */
					if(chdir(msg_c.msgdatos)==0){ 
						/* Se envian los datos del fichero */
						while ((p_dir = readdir(fdir)) != NULL){
							strcpy(msg_c.msgdatos,p_dir->d_name);
							fflush(stdout);
							msgsnd(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), 0);
						}

						/* Se envia el mensaje de fin */
						if((p_dir = readdir(fdir))== NULL){
							
							strcpy(msg_c.msgdatos,"");
							msg_c.msgfinal=1;
							msgsnd(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), 0);
							printf("\n\nDatos enviados al usuarioA\n\n");
						}
					}else{
						/* Se envia como datos que no eciste el archivo */
						strcpy(msg_c.msgdatos,"El archivo no existe");
						msgsnd(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), 0);
						
						/* Se envia el mensaje de fin */
						msg_c.msgfinal=1;
						strcpy(msg_c.msgdatos,"");
						msgsnd(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), 0);
						printf("\nEl archivo no existe."); 
					}
					closedir(fdir);
				}

		}
	}
}


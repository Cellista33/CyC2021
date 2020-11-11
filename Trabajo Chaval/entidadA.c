/******************************************************************************/
/*																			  */
/*								entidadA.c:									  */
/*																			  */
/*	Se encarga de recibir por la INTERFAZ1 las instrucciones del usuario1     */
/*  enviarselas por la memoria compartida a la entidad2.					  */
/*																			  */
/******************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "msg.h"
#include "msgq.h"

/* Se crea estructura para la cola */
msg_C msg_c;
/* Identificadores de memoria compartida y semaforos */
int shm, sem, sem2;
/* Puntero a la estructura de memoria compartida */
msg_M  *msg_ptr;


/* Definimos la estructura del semaforo */
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
} semctl_arg;


/*Definimos las funciones para tomar el semaforo y para liberar el semaforo */
/* El sem 0 se usa para que la entidadA acceda, mientras que el otro para
 * que la entidadB entre */
void libera_sem(int num) {
        struct sembuf operasem;

        /* Semaforo a liberar */
        operasem.sem_num = num; 
		/* Incrementa 1, LIBERA */
        operasem.sem_op = 1;   
		/* Numero para el que bloquea */
        operasem.sem_flg = 0; 

        if(semop(sem, &operasem, 1) < 0){
                perror("semop");
        }
}

void toma_sem(int num) {
        struct sembuf operasem;

        /* Semaforo a tomar */
        operasem.sem_num = num;
		/* Decrementa 1, TOMA */
        operasem.sem_op = -1; 
		/* Numero para el que bloquea */
        operasem.sem_flg = 0;

        if(semop(sem, &operasem, 1) < 0){
                perror("semop");
        }
}



main (){
	
	/* Variable que almacenará el ID de la cola 1 */
	int interfaz1;

	/* Creamos la cola de mensajes */
	if( (interfaz1=msgget(MKEYQ1, PERMS|IPC_CREAT)) < 0 ){
		perror("Servidor: No puedo obtener id de la cola");
		exit(1);
	}

	/* Conectamos a la memora compartida */
	if ( (shm = shmget(123L, sizeof(msg_M),0)) < 0) {
		perror("No se pudo conectar a la memoria compartida");
		exit(1);
	}
	
	/* Asociamos un puntero al segmento de memoria compartida */
	if ( (msg_ptr = (msg_M *) shmat(shm, (char *)0, 0)) == (msg_M *) -1){
		perror("No se puede asociar el puntero al segmento de memoria compartida");
		exit(1);
	}
	
	/* Se crea el canal de semaforos*/
	if(( sem = semget(234L, 2, 0)) <0){
		perror("semget");
		exit(1);
	}
        
	while(1){
		/* Por defecto antes de saber lo que elige usuario1 se toma
		 * como modo el memoria compartida */
		msg_c.msgmodo = 'm';
		
		/* Por defecto antes de cada bucle se inicia el flag de final */
		msg_ptr->msgfinal = 0;
		
		
		/* Recibimos del usuario A a traves de la cola */
		printf("\nEn espera peticion de usuario A");
		if ((msgrcv(interfaz1, &msg_c, sizeof(msg_C)-sizeof(long), 1L, 0)) < 0 )
		{
			perror("msgrcv");
			printf("Error al recibir por la cola");fflush(stdout);
			exit(1);
		}
		/* Mensaje de control */
		printf("\nHa recibido de usuario A.");

		/* Si el mecanismo de comunicacion entre los servidores es memoria compartida */
		if(msg_c.msgmodo == 'm'){ 

			/* Toma el semaforo0 */
			printf("\nVa a tomar el semaforo 0");
			toma_sem(0);

			/*Guardamos en la memoria compartida los datos que obtenemos del usuario A mediante la cola 1.*/
			printf("\nEscribiendo en memoria compartida.");

			msg_ptr->msgorigen = msg_c.msgorigen;
			msg_ptr->msgdestino = msg_c.msgdestino;  
			msg_ptr->msgprotocolo = msg_c.msgprotocolo; 
			msg_ptr->msgopcion = msg_c.msgopcion;
			strcpy(msg_ptr->msgcadena, msg_c.msgcadena);
			strcpy(msg_ptr->msgdatos, msg_c.msgdatos);

			printf("\nYa ha escrito en memoria compartida.");

			/*Liberamos el semaforo 1 para que la entidad2 pueda acceder a la memoria */
			libera_sem(1);
			printf("\nYa ha liberado el SEM1.");

			/* Si el comando introducido el "quit" se sale del programa, ya se ha 
			 * comunicado a los demás procesos por la memoria compartida */
			if(strcmp(msg_c.msgcadena,"quit") == 0) {
				if ((msgctl(interfaz1, IPC_RMID, (struct msqid_ds *) NULL)) < 0){
					printf("Error");
					exit(1);
				}
				exit(0);
			}


			/* Mientras no reciba el flag de fin de mensaje */
			while(msg_ptr->msgfinal != 1){

				printf("\nEntra en el bucle de lectura");
				printf("\nVa a tomar SEM0 para leer mem.");
				toma_sem(0);
					
				/* Lee de memoria compartida */
				msg_c.msgorigen = msg_ptr->msgorigen; 
				msg_c.msgdestino = msg_ptr->msgdestino;
				msg_c.msgprotocolo = msg_ptr->msgprotocolo;
				msg_c.msgtype = msg_ptr->msgdestino;
				strcpy(msg_c.msgdatos, msg_ptr->msgdatos);

				/* Envia por la interfaz1 a usuario1 */
				msgsnd(interfaz1, &msg_c, sizeof(msg_C) - sizeof(long), 0);
				printf("\nEnviado por la cola a Usuario A.");
				
				/* Si no se ha llegado a final del mensaje se libera SEM1 */
				if(msg_ptr->msgfinal != 1) {
					printf("\nLeido, va a liberar SEM1.");
					libera_sem(1);
					printf("\nLiberado SEM 1.");
				}
				
			}
			
			/* Limpieza de variables antes del ultimo mensaje */
			strcpy(msg_c.msgdatos,"");
			msg_c.msgfinal=1;
			
			/* Envia el ultimo mensaje */
			msgsnd(interfaz1, &msg_c, sizeof(msg_C) - sizeof(long), 0);
			
			/*Ahora que es el ulitmo mensaje libera SEM0 */
			libera_sem(0);
			printf("\nFin del mensaje.");
			printf("\nLiberado SEM 0.");

			
		}

		printf("\nDatos enviados al usuario1.");
	}

}

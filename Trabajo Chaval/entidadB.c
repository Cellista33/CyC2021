/******************************************************************************/
/*																			  */
/*								entidadB.c:									  */
/*																			  */
/*	Se encarga de recibir por la memoria compartida las instrucciones del	  */
/*  usuario1 para enviarselas por la INTERFAZ2 al usuario2.					  */
/*																			  */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "msg.h"
#include "msgq.h"

/* Definimos la estructura del semaforo */
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
} semctl_arg;

/* Se crea estructura para la cola */
msg_C msg_c;     

/* Identificadores de memoria compartida y semaforos */
int shm, sem, sem2; 

/* Puntero a la estructura de memoria compartida */
msg_M  *msg_ptr;  

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

	/* Variable que almacenará el ID de la cola 2 */
	int interfaz2;
	/*Variable para comprobar que se cree bien la SHM */
	int id;

	/* Creamos la cola de mensajes */
	if( (interfaz2 = msgget(567L, 0666 | IPC_CREAT)) < 0 ){
		perror("Servidor: No puedo obtener id de la cola");	
		exit(1);
	}

	/* Se obtiene el identificador para la memoria compartida */
	if ((id=SHMKEY) == (key_t) -1)
	{
		printf(" Se ha producido un error al crear la llave de la memoria compartida");
		exit(1);
	}
	
	/* Conectamos a la memora compartida */
	if ( (shm = shmget(123L, sizeof(msg_M),0666|IPC_CREAT)) < 0)
		perror("shmget");

	/* Asociamos un puntero al segmento de memoria compartida */
	if ( (msg_ptr = (msg_M *) shmat(shm, (char *) 0, 0)) == (msg_M *) -1){
		printf("No se puede asociar el puntero al segmento de memoria compartida");
		perror("shmat");
	}

	
	/* Se crea el canal de semaforos*/
	if(( sem = semget(234L, 2, 0666|IPC_CREAT)) <0){
		perror("semget");
		exit(1);
	}

	/* Se inicializa con valor 1 el SEM 0 */
	semctl_arg.val = 1;
	if(semctl(sem, 0, SETVAL, semctl_arg) < 0){
		perror("semctl");
		exit(1);
	}

	/* Se inicializa con valor 0 el SEM 1 */
	semctl_arg.val = 0;
	if(semctl(sem, 1, SETVAL, semctl_arg) < 0){
		perror("semctl");
		exit(1);
	}

	while(1){
		/* Por defecto antes de saber lo que elige usuario1 se toma
		 * como modo el memoria compartida */
		msg_c.msgmodo = 'm';
			
		/* Si en modo hay una m usamos memoria compartida y si no usamos socket */
		printf("\nEntidad 2 OK.");

		/* Si el mecanismo de comunicacion entre los servidores es memoria compartida */
		if (msg_c.msgmodo == 'm') {
			
			/* Toma el semaforo1 */
			printf("\nVa a tomar sem 1.");
			toma_sem(1);
		
			/* Se lee en la memoria compartida los datos que escribió entidadA.*/
			msg_c.msgtype = msg_ptr->msgdestino;
			msg_c.msgorigen = msg_ptr->msgorigen; 
			msg_c.msgdestino = msg_ptr->msgdestino;
			msg_c.msgprotocolo = msg_ptr->msgprotocolo;
			msg_c.msgopcion = msg_ptr->msgopcion;
			strcpy(msg_c.msgdatos, msg_ptr->msgdatos);
			strcpy(msg_c.msgcadena, msg_ptr->msgcadena);

			printf("\nHa leido de mem comp.");

			/* Se lo envia al cliente 2 */
			printf("\nEnvia a Usuario B.");
			msgsnd(interfaz2, (msg_C *) &msg_c, sizeof(msg_C) - sizeof(long), 0);
			
			/* Si el comando introducido el "quit" se sale del programa, ya se ha 
			 * comunicado a los demás procesos por la memoria compartida */
			if(strcmp(msg_c.msgcadena, "quit") == 0) {
				
				/*Un tiempo de espera para dar tiempo a usuarioB a leer la cola*/			
				sleep(2);

				/* Liberamos la memoria compartida */
				if(shmdt((char *) msg_ptr) < 0){
					perror("No se puede eliminar la memoria");
					exit(1);
				}
				
				/* Se elimina la memoria compartida */
				if(shmctl(shm, IPC_RMID, NULL) < 0){
					perror("No se puede eliminar la memoria");
					exit(1);
				}

				/* Se elimina la cola 2 */
				if (msgctl(interfaz2, IPC_RMID, (struct msqid_ds *) NULL) < 0) {
					perror("Error");
					exit(1);
				}
				
				/* Se destruyen los semaforos */
				if(semctl(sem,2,IPC_RMID,semctl_arg)<0) {
					 printf("\nNo se ha podido borrar el semaforo\n");
					exit(1);
				}
				exit(0);
			}

			/* Se indica el sentido de la cola */
			msg_c.msgtype = 2L;

			/* Se lee de la cola 2 lo que envia usuario2 */
			msgrcv(interfaz2, (msg_C *) &msg_c, sizeof (msg_C) - sizeof(long), msg_c.msgtype, 0);	
			printf("\nHa leido de Usuario B");
			
			/* Escribe en la memoria compartida los nuevos datos para entidadA */
			printf("\nVa a escribir en mem comp");

			msg_ptr->msgorigen = msg_c.msgorigen;
			msg_ptr->msgdestino = msg_c.msgdestino;
			msg_ptr->msgprotocolo = msg_c.msgprotocolo;
			strcpy(msg_ptr->msgdatos, msg_c.msgdatos);
			
			/*Liberamos el semaforo 0 para que la entidadA pueda acceder a la memoria */
			libera_sem(0);
			printf("\nHa escrito en mem comp y libera SEM 0");

			/* Se indica el sentido de la cola y se resetea el flag de fin de mensaje */
			msg_c.msgtype = 2L;
			msg_c.msgfinal = 0;
			while(msg_c.msgfinal != 1){		
				
				/* Toma el semaforo1 para leer */
				toma_sem(1);
				
				/* Se lee de la cola 2 lo que envia usuario2 */
				msgrcv(interfaz2, (msg_C *) &msg_c, sizeof (msg_C) - sizeof(long), msg_c.msgtype, 0);
				printf("\nHa leido de Usuario B");
				printf("\nVa a escribir en mem comp");

				/* Se escribe en la memoria para entidadA */
				msg_ptr->msgorigen = msg_c.msgorigen;
				msg_ptr->msgdestino = msg_c.msgdestino;
				msg_ptr->msgprotocolo = msg_c.msgprotocolo;
				strcpy(msg_ptr->msgdatos, msg_c.msgdatos);

				/*Liberamos el semaforo 0 para que la entidadA pueda acceder a la memoria */
				libera_sem(0);

				printf("\nHa escrito en mem comp y libera SEM 0");
			}

			/* Se indica el fin del mensaje */
			msg_ptr->msgfinal=1;
			printf("\nEscrito en la memoria compartida para la entidad 1\n\n");	
		}
	}

}



#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "entidadA.h"


//Definimos las varibales asociadas a la comunicación

  //Cola de mensajes
  cola_msg  cola_A;
  int ID_colaA;

  //memoria compartida
  int ID_memo;
  m_compartida *mc_ptr;

  //Semaforos
  int sema;
  

  int ID_A;




void inicio_cola(){
  if(( ID_colaA = msgget(CLAVE1, PERMS|IPC_CREAT) ) < 0){
    perror("No se ha podido obtener la id de la cola");
    exit(1);
  } else printf ("\nSe ha iniciado la cola de mensajes");


}


void inicio_memoria(){
  if((ID_memo = shmget(SHMKEY,sizeof(m_compartida),0)) < 0){
    perror("No se ha podido conectar a la memoria compartida");
    exit(1);
  }else printf("\n Se ha conectado a la memoria compartida");

  if((mc_ptr =(m_compartida *) shmat(ID_memo, (char *)0, 0)) == (m_compartida *) -1){
    perror ("No se ha podido apuntar a la memoria compartida");
    exit(1);
  }else printf("\n  Se ha apuntado a la memoria compartida");
}

void inicio_semaforo(){
  if ((sema = semget(SEMKEY, 2,0)) < 0 ){ //Indicamos que inicializamos el array de semaforos que tendra dos dimensiones
    perror("No se ha podido crear el canal de semaforo");
    exit(1);
  }else printf("\n No Se ha conectado al canal del semaforo");
}

void bloq_sema (int id){
	struct sembuf op;
	printf ("\nSe va a bloquear el semaforo %i", id);
	
	op.sem_num = id;
	op.sem_num = -1;
	op.sem_num = 0;
	
	if (semop(sema, &op, 1) < 0){
		perror("No se ha podido bloquear el semaforo");
	} else printf("\n Se ha bloqueado el semaforo %i", id);
}

void libre_sema (int id){
	struct sembuf op;
	printf ("\nSe va a liberar el semaforo %i", id);
	
	op.sem_num = id;
	op.sem_num = 1;
	op.sem_num = 0;
	
	if (semop(sema, &op, 1) < 0){
		perror("No se ha podido liberar el semaforo");
	}else printf("\n Se ha liberado el semaforo %i", id);
}

void guarda_memoria(){
	
	printf("\nSe va a guardar en la memoria compartida");
	
	mc_ptr -> origen = cola_A.origen;
	mc_ptr -> destino = cola_A.destino;
	mc_ptr -> opcion = cola_A.opcion;
	strcpy( mc_ptr -> cadena, cola_A.cadena);
	strcpy( mc_ptr -> datos, cola_A.datos);	

	printf("\n Ya se ha guardado en la memoria compartida");
}

void lee_memoria(){

	printf ("\nSe va a leer de la memoria compartida y a escribir en la cola");
	
	cola_A.origen = mc_ptr -> origen;  
	cola_A.destino = mc_ptr -> destino;
	cola_A.opcion = mc_ptr -> opcion;
	strcpy( cola_A.datos,    mc_ptr -> datos);
	
	printf ("\n Se han pasado los datos de la memoria compartida a la cola");
}


void lectura_cola(){

	if ((msgrcv(ID_colaA, &cola_A, sizeof(cola_msg)-sizeof(long), 1L, 0)) < 0 ){
	
		printf("Error al recibir por la cola");
		fflush(stdout);
		exit(1);
	}

	printf("\nHa recibido de usuario A.");
    	printf("\tdatos: %s\n", cola_A.datos);
  
}

void cierre_memoria(){
  if (shmdt((char *) mc_ptr) < 0){
    perror("No se ha podido desapuntar la memoria");
    exit(1);
  } else{
  printf("\n Se ha desapuntado a la memoria");
  }
}

void cierre_cola(){
  if((msgctl(ID_colaA, IPC_RMID, (struct msqid_ds *)NULL)) < 0){
    perror("\nNo se ha podido cerrar la cola de mensajes");
    exit(1);
  } else{
  printf("\nSe ha cerrado la cola de mensajes");
  }
}

int main() { 


  inicio_cola();  
  inicio_memoria();  
  inicio_semaforo();  
  printf("\nEn espera peticion de usuario 1");
  lectura_cola();
  bloq_sema(0);
  guarda_memoria();
  libre_sema(1);
  
  bloq_sema(0);
  lee_memoria();
  
  
  

  
  
  
  cierre_memoria();
  cierre_cola();


}
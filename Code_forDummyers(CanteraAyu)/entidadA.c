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
  m_compartida *memo_puntero;

  //Semaforos
  int sema_1, sema_2;

  int ID_A;




void inicio_cola(){
  if(( ID_colaA = msgget(CLAVE1, PERMS|IPC_CREAT) ) < 0){
    perror("No se ha podido obtener la id de la cola");
    exit(1);
  }


}


void inicio_memoria(){
  if((ID_memo = shmget(SHMKEY,sizeof(m_compartida),0)) < 0){
    perror("No se ha podido conectar a la memoria compartida");
    exit(1);
  }

  if((memo_puntero =(m_compartida *) shmat(ID_memo, (char *)0, 0)) == (m_compartida *) -1){
    perror ("No se ha podido apuntar a la memoria compartida");
    exit(1);
  }
}

void inicio_semaforo(){
  if ((sema_1 = semget(SEMKEY, 2,0)) < 0 ){
    perror("No se ha podido crear el canal de semaforo");
    exit(1);
  }
}

void lectural_cola(){

	if ((msgrcv(ID_colaA, &cola_A, sizeof(cola_msg)-sizeof(long), 1L, 0)) < 0 ){
	
		printf("Error al recibir por la cola");
		fflush(stdout);
		exit(1);
	}

	printf("\nHa recibido de usuario A.");
    	printf("\tdatos: %s\n", cola_A.datos);
  
}

void cierre_memoria(){
  if (shmdt((char *) memo_puntero) < 0){
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
  printf ("\nSe ha iniciado la cola de mensajes");
  inicio_memoria();
  printf("\n No Se ha conectado a la memoria compartida");
  inicio_semaforo();
  printf("\n No Se ha conectado al canal del semaforo");
  printf("\nEn espera peticion de usuario 1");
  
  lectura_cola();

  
  
  
  cierre_memoria();
  cierre_cola();


}
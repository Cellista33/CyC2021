#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "entidadA.h"

//Definimos las varibales asociadas a la comunicación

  //Cola de mensajes
  cola_msg  cola_B;
  int ID_colaB;

  //memoria compartida
  int ID_memo;
  m_compartida *memo_puntero;

  //Semaforos
  int sema;

  int ID_A;




void inicio_cola(){
  if(( ID_colaB = msgget(CLAVE2, PERMS|IPC_CREAT) ) < 0){
    perror("No se ha podido obtener la id de la cola");
    exit(1);
  }


}


void inicio_memoria(){
  if((ID_memo = shmget(SHMKEY,sizeof(m_compartida),PERMS|IPC_CREAT)) < 0){
    perror("No se ha podido crear la memoria compartida");
    exit(1);
  }

  if((memo_puntero =(m_compartida *) shmat(ID_memo, (char *)0, 0)) == (m_compartida *) -1){
    perror ("No se ha podido apuntar a la memoria compartida");
    exit(1);
  }
}

void inicio_semaforo(){
  if ((sema = semget(SEMKEY, 2,PERMS|IPC_CREAT)) < 0 ){
    perror("No se ha podido crear el canal de semaforo");
    exit(1);
  } else printf("\nSe ha creado el canal de los semaforos");
  
  
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


void cierre_memoria(){
  if (shmdt((char *) memo_puntero) < 0){
    perror("No se ha podido desapuntar la memoria");
    exit(1);
  } else{
  printf("\nSe ha desapuntado de la memoria");
  }

  if(shmctl(ID_memo, IPC_RMID, (struct shmid_ds *)0) < 0 ){ //shmid_ds es una estructura de /sys/shm.h que define una región de memoria compartida 
  perror("No se puede eliminar la memoria compartida");
  exit(1);
  } else{
  printf("\nSe ha eliminado la memoria compartida");
  }
}

void cierre_cola(){
  if((msgctl(ID_colaB, IPC_RMID, (struct msqid_ds *)NULL)) < 0){
    perror("No se ha podido cerrar la cola de mensajes");
    exit(1);
  } else{
  printf("\nSe ha cerrado la cola de mensajes");
  }
}





int main() {

  


  inicio_cola();
  printf ("\nSe ha iniciado la cola de mensajes");
  inicio_memoria();
  printf("\nSe ha conectado a la memoria compartida");
  inicio_semaforo();
  printf("\nSe ha conectado al canal del semaforo");

  
  sleep(20);
  
  cierre_memoria();
  cierre_cola();


}
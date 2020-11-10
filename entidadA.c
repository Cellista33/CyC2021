#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "entidadA.h"

//Definimos las varibales asociadas a la comunicación

  //Cola de mensajes
  cola_msg  cola_A;

  //memoria compartida
  int ID_memo;
  m_compartida *memo_puntero;

  //Semaforos
  int sema_1, sema_2;

  int ID_A;




/*void inicio_cola(int *ID){
  if()


}
*/

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



int main() {

  


  //inicio_cola(&cola_A);
  printf ("\nSe ha iniciado la cola de mensajes");
  inicio_memoria();
  printf("\nSe ha conectado a la memoria compartida");
  inicio_semaforo();
  printf("\nSe ha conectado al canal del semaforo");


}


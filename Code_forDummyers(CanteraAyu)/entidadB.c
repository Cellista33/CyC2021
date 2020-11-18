#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "entidadA.h"

//Definimos las varibales asociadas a la comunicación

//Cola de mensajes
cola_msg  cola_B;
int ID_colaB;

//memoria compartida
int ID_memo;
m_compartida *mc_ptr;

//Semaforos
int sema;

int ID_A;

struct sembuf opb; 
struct sembuf opl;


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
  
  if((mc_ptr =(m_compartida *) shmat(ID_memo, (char *)0, 0)) == (m_compartida *) -1){
    perror ("No se ha podido apuntar a la memoria compartida");
    exit(1);
  }
}

void inicio_semaforo(){
  if ((sema = semget(SEMKEY, 2,PERMS|IPC_CREAT)) < 0 ){
    perror("No se ha podido crear el canal de semaforo");
    exit(1);
  } else printf("\nSe ha creado el canal de los semaforos");
  
  semaforo.val = 0;
  if(semctl(sema, 0, SETVAL, semaforo) < 0){
    perror("semctl");
    exit(1);
  }
  
  /* Se inicializa con valor 0 el SEM 1 */
  semaforo.val = 0;
  if(semctl(sema, 1, SETVAL, semaforo) < 0){
    perror("semctl");
    exit(1);
  }
  
  
  
}

void bloq_sema (int id){
  
  printf ("\nSe va a bloquear el semaforo %i", id);
  
  opb.sem_num = id;
  
  //Operacion de bloquear al semaforo
  opb.sem_op = -1;
  opb.sem_flg = 0;
  
  
  
  
  
  
  
  
  if (semop(sema, &opb, 1) < 0){
    perror("No se ha podido bloquear el semaforo");
  } else printf("\n Se ha bloqueado el semaforo %i", id);
}

void libre_sema (int id){
  
  printf ("\nSe va a liberar el semaforo %i", id);
  
  opl.sem_num = id;
  //Operacion de liberar al semaforo
  opl.sem_op = 1;
  opl.sem_flg = 0;
  
  
  if (semop(sema, &opl, 1) < 0){
    perror("No se ha podido liberar el semaforo");
  }else printf("\n Se ha liberado el semaforo %i", id);
}

void guarda_memoria(){
  
  printf("\nSe va a guardar en la memoria compartida");
  mc_ptr -> origen = cola_B.origen;
  mc_ptr -> destino = cola_B.destino;
  mc_ptr -> longitud = cola_B.longitud;
  mc_ptr -> datito = cola_B.dat_que;
  mc_ptr -> tipo = (unsigned char) 0;
  
  printf("\n Ya se ha guardado en la memoria compartida");
}

void lee_memoria(){
  
  printf ("\nSe va a leer de la memoria compartida y a escribir en la cola");
  cola_B.type = (long) mc_ptr -> destino;
  cola_B.origen = mc_ptr -> origen;  
  cola_B.destino = mc_ptr -> destino;
  cola_B.longitud = mc_ptr -> longitud;
  cola_B.dat_que = mc_ptr -> datito;
  
  
  printf ("\n Se han pasado los datos de la memoria compartida a la cola");
}

void envia_cola(){
  
  printf("\nEnviando mensaje por la cola");
  msgsnd(ID_colaB, &cola_B, sizeof(cola_B)-sizeof(cola_B.type), 0);
  printf("\nEl mensaje se ha enviado por la cola");
  
}

void recibe_cola(){
  
  cola_B.type = 2L;
  printf("\nRecibiendo mensaje por la cola");
  msgrcv(ID_colaB, &cola_B, sizeof(cola_msg)-sizeof(long),cola_B.type, 0);
  printf("\nEl mensaje se ha recibido por la cola");
  
}


void cierre_memoria(){
  if (shmdt((char *) mc_ptr) < 0){
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





//--------------------------------------------------------FUNCIONES PARA SACAR DATOS POR PANTALLA---------------------------------------------------------
void saca_memoria(){
  printf("\n\t\t\tValores almacenados en la memoria compartida");
  printf("\n\t Origen %ls", &(mc_ptr->origen));
  printf("\n\t Destino %ls", &(mc_ptr->destino));
  
}

void saca_cola(){
  printf("\n\t\t\tValores almacenados en la cola B");
  printf("\n\t Origen %i", cola_B.origen);
  printf("\n\t Destino %i", cola_B.destino);
  printf("\n\t Datos %s", cola_B.dat_que.data);
  
}



int main() {
  
  char aux;
  int uno = 1;
  int cero = 0;
  
  
  
  
  inicio_cola();
  printf ("\nSe ha iniciado la cola de mensajes");
  inicio_memoria();
  printf("\nSe ha conectado a la memoria compartida");
  inicio_semaforo();
  printf("\nSe ha conectado al canal del semaforo");
  //printf ("\nPuta vida loco");
  
  
  
  
  do{
    
    libre_sema(0);
    bloq_sema(1);
    printf("\nEsperando a leer la memoria");
    lee_memoria();
    saca_memoria();
    
    
    if (mc_ptr -> tipo == (unsigned char) 0){
      mc_ptr -> tipo = (unsigned char) 1;
    }else mc_ptr -> tipo = (unsigned char) 2;
    
    if(mc_ptr -> tipo == 1){		
      printf("\nSe ha leido la memoria y se ha comfirmado");
      saca_cola();
      //printf("dgsgsdfgsdgf");
      envia_cola();
    }	
    
    libre_sema(0);
    bloq_sema(1);
    
  } while( mc_ptr -> longitud == (MAXDATA_U + 2) || mc_ptr -> tipo == (unsigned char)2);
  
  
  
  
  
  
  do{
    
    recibe_cola();
    do{
      
      libre_sema(0);
      bloq_sema(1);
      guarda_memoria();
      saca_cola();
      saca_memoria();
      libre_sema(0);
      bloq_sema(1);
      
      
      
      //Ahora comprueba el mensaje de respuesta para ver si es ack o error 
      //bloq_sema(0);
      lee_memoria();
      aux = mc_ptr -> tipo; 
      
      libre_sema(0);
      //bloq_sema(1);
      
    } while( aux != (unsigned char)1);
    printf("\nMensaje confirmado");
    
  }while( cola_B.longitud == (MAXDATA_U + 2));
  
  
  
  
  
  cierre_memoria();
  cierre_cola();
  
  
}






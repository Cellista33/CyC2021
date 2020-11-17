#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <dirent.h>
#include <errno.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "usuario2.h"


//Definimos las varibales asociadas a la comunicación

  //Cola de mensajes
  cola_msg  cola_B;
  int ID_colaB;
  

//Variables asociadas a la gestion de ficheros
	DIR *dir_funct;
	struct dirent directorio, *directorio_ptr;
	
	
  
  
  
  void conecta_cola(){
  	
  	if ((ID_colaB = msgget (CLAVE2, 0))<0){
  		
  		perror("Usuario 2 no se ha podido conectar a la cola.");
  		exit(1);  		
  	} else{
  		printf("\nSe ha conectado a la cola de mensajes 2 creada por B");
  	}
  }
  
  void cli_eco(){
  
  	printf("\nEsta en el modo de eco");
  
  	saca_cola();	
  	invierte_cola();
  	envia_cola();
  
  }
  
  
  
  void cli_tf(){
  	printf ("\nEsta en el modo de TF");
  
  }
  
  void recibe_cola(){
  
  	printf("\nEsperando a la recepcion por la cola B");
  	msgrcv(ID_colaB, (cola_msg *) &cola_B, sizeof(cola_B) - sizeof(long), getpid(), 0); //Se utiliza como identificador de la cola el pid de usuario 2.
  }
  
  
  void invierte_cola(){
  
  	 //Aquí es donde invertimos los valores para que se envíen los mismos datos de vuelta a entidad B
  	 int var_aux;
  	 
  	 var_aux = cola_B.origen;
  	 cola_B.origen = cola_B.destino;
  	 cola_B.destino = var_aux;
  	 cola_B.type = 2L; 
  	 
  	 
  }
  
  void envia_cola(){
  	
  	if (msgsnd(ID_colaB, (cola_msg *) &cola_B, sizeof(cola_B) - sizeof(long), 0) < 0){
  		perror("No se ha podido enviar por la cola B");
  		exit(1);
  	}else {
  		printf("\nSe han mandado el mensaje por la cola B");
  	}
  }
  
  //--------------------------------------------------Saca datos por pantalla-------------------------------------------------------
  
  void saca_cola(){
	printf("\n\t\t\tValores almacenados en la cola B");
	printf("\n\t Type %ld", cola_B.type);
	printf("\n\t Origen %i", cola_B.origen);
	printf("\n\t Destino %i", cola_B.destino);
	printf("\n\t Datos %s", cola_B.datos);
	
}

  
  
  
  
  void main (){
  
  	printf("\n------------------------Usuario 2-----------------------");
  	printf("\n\n\t\tEl PID del usuario 2 es: %ld", cola_B.type = getpid());
  	conecta_cola();
  	
  	
  	while (1){ 
  		
  		recibe_cola();
	  	switch (cola_B.opcion + 1){ //Como la opcion 0 es la default, le sumamos uno para poder determinar los casos
	  		
	  		case 1:
	  			printf("\nEstamos dentro del bucle y se va a proceder eco");
	  			cli_eco();
	  			break;
	  		case 2: 
	  			cli_tf();
	  			break;
	  	}
	  	  	
	  }
  	  	
  }
  



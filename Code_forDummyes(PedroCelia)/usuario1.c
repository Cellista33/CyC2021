//--------------------------------------------------------------------------------//
//	Usuario 1: Controla la comunicación haciendo peticiones ECO o TF							//
//--------------------------------------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "msg.h"
#include "msgq.h"

// Creamos una colo de tipo de dato cola_msg
cola_msg colaMsg;

void main (int argc, char *argv[]){

	// Variable para almacenar ID de cola 1
	int IDcola1;

	printf("\nIntruduzca instruccion: ");

	// Comprobamos la cantidad de argumentos introducidos es correcto
	if(argc != 3 || argc != 4) {
		perror("\nERROR: Formato de instruccion incorrecto");
		// Salida del programa por fallo de entorno
		exit(EXIT_FAILURE);
	}

  // Recorremos los datos introducidos por tecado y los asignamos 
  // a sus campos en cola de mensajes

	// Pid del usuario destino (usuario2 en este caso)
	colaMsg.destino = atoi(argv[1]);

	// Mecanismo de IPC empleado (Mem compardida (m) o sockets (s))
	colaMsg.modo = *argv[2];

  // Pid del usuario origen (usuario1 aquí)
	colaMsg.origen = getpid();

	// Comprobación de modo: ECO/TF
	// Se comprueba si las cadenas son iguales solo si el numero de argumentos es mayor
	// que 4, en caso contrario, da error
	if(argc == 4)
  {
		if (!(strcmp(argv[3], "TF")) || !(strcmp(argv[3], "tf")))
      {
			  // Estamos en modo TF porque tenemos 4 argumentos
			  colaMsg.opcion = 1;
			  printf("\nModo TF.");
		  }

		else
      {
        printf("\nERROR: Ultimo argumento no reconocido");
        exit(EXIT_FAILURE);
      }
	}

	else
  {
		// Estamos en modo eco porque tenemos 3 argumentos 
		colaMsg.opcion = 0;
		printf("\nModo eco.");
	}

	// Nos intentamos conectar a la cola 1 
	if ( (IDcola1 = msgget(MKEYQ1, 0)) < 0)
  {
		printf("\nEl Usuario 1 no se ha podido conectar a la cola 1");
		exit(EXIT_FAILURE);
	}

	// Bucle general de ejecución del programa
	while(1){
	// Se inicia el flag de final antes de cada bucle
	colaMsg.fin = 0;
	// Sentido fijado en dirección Usuario -> Servidor
	colaMsg.sentido = 1L;

	if (colaMsg.opcion){
		// Modo TF
		printf("\nIntroduzca que funcion a realizar:\n\t=>dir\n\t=>cd\n\t=>get\n\t=>quit");
		printf("\nSu comando es: ");
		// Entrada por teclado
		scanf("%s",colaMsg.comando);

		// Bucle para asegurar que se introduce un comando válido
		while(strcmp(colaMsg.comando, "cd") && strcmp(colaMsg.comando, "dir") && strcmp(colaMsg.comando,"get") && strcmp(colaMsg.comando, "quit"))
    {
			printf("\nLa funcion introducidad no es correcta");
			printf("\nIntroduzca que funcion a realizar:\n\t=>dir\n\t=>cd\n\t=>get\n\t=>quit");
		  printf("\nSu comando es: ");
			scanf("%s",colaMsg.comando);
		}

		// Se ejecuta acción según el comando introducido

    // Para comando "dir", se ejecuta exclusivamente en usuario 2
		if(strcmp(colaMsg.comando,"dir") == 0) 
    {
			printf("\nInformacion sobre directorio usuario 2: ");
		}

		// Para comando "cd", se pide directorio:
		if(strcmp(colaMsg.comando,"cd") == 0) 
    {
			printf("\nIntroduzca el directorio que desee abrir: ");
			scanf("%s",colaMsg.datos);
		}

		// Para comando "get", se pide fichero:
		if(strcmp(colaMsg.comando,"get") == 0 )
    {
			printf("\nIntroduzca el directorio que contiene los archivos que desee recibir: ");
			scanf("%s",colaMsg.datos);
		}

		// Para comando "quit", se ejecuta el cierre:
		if(strcmp(colaMsg.comando,"quit") == 0 )
    {
			printf("\nCerrando el programa...");
			msgsnd(IDcola1, &colaMsg, sizeof(cola_msg)-sizeof(long), 0);
			sleep(2);
      printf("\nPrograma finalizado");
			exit(EXIT_SUCCESS);
		}
	}

	else
  {
		// Modo ECO
		puts("\nIntroduzca texto para enviar: ");

		// Comprobamos si tamaño introducido es mayor que el admitido
		if (fgets(colaMsg.datos, TAM_MAX, stdin) == NULL)
		{
			printf("\nError en el texto introducido");
			exit(EXIT_FAILURE);
		}
	}

	// Mostramos por pantalla parametros interesantes
	printf("\nEnviado desde el Usuario 1:\n");
	printf("\tOrigen: %i\n",colaMsg.origen);
	printf("\tDestino: %i\n", colaMsg.destino);
	printf("\tDatos: %s\n", colaMsg.datos);

	printf("\n\n");

	// Enviamos la trama por la cola
	printf("\nEnviando mensaje por cola...");
	msgsnd(IDcola1, &colaMsg, sizeof(cola_msg)-sizeof(long), 0);
	printf("\nMensaje enviado por cola");

	// Espera hasta que en el campo colaMsg.msgopcion aparezca un 1
	if (colaMsg.opcion)
  {
		// Modo TF
		printf("\nEsperando mensaje en modo TF");

		// Se recibe mensaje por la cola
		printf("\nEsperando mensaje por cola");
		msgrcv(IDcola1, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), (long)getpid(), 0);

		// Se muestra el mensaje recibido de usuario2
		printf("\nRecibido del Usuario 2:\n");
		printf("\torigen: %i\n",colaMsg.origen);
		printf("\tdestino: %i\n", colaMsg.destino);
		printf("\tdatos: %s\n", colaMsg.datos);

		// Se reciben mensajes por la cola hasta que llegue el ultimo 
		while (colaMsg.fin != 1)
    {
			msgrcv(IDcola1, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), (long)getpid(), 0);
			printf("\t       %s\n", colaMsg.datos);
		}
	}

	else{
		// Modo ECO
		// Se recibe mensaje por la cola
		printf("\nEsperando mensaje en modo ECO");

		msgrcv(IDcola1, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), (long)getpid(), 0);

   // int msgrcv(IDcola1, struct msgbuf *msgp, int msgsz, long mtype, int msgflg);


		// Se muestra el mensaje recibido de usuario2 
		printf("\nRecibido desde el Usuario 2:\n");
		printf("\torigen: %i\n",colaMsg.origen);
		printf("\tdestino: %i\n", colaMsg.destino);
		printf("\tdatos: %s\n", colaMsg.datos);

		// Salimos del programa
		exit(EXIT_SUCCESS);
	}

	// Se invierten PID de origen y destino
	colaMsg.sentido = colaMsg.destino;
	colaMsg.destino = colaMsg.origen;
	colaMsg.origen = colaMsg.sentido;
	}
}
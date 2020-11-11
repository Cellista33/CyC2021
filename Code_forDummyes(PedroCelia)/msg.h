//----------------------------------------------------------------------------//
//	msg.h: Declaración de las estructuras usadas en mecanismos IPC			      //
//----------------------------------------------------------------------------//
#define TAM_MAX (1500) // Tamaño maximo del mensaje de cola.
#define TAM_CAD (10) // Tamaño maximo del mensaje de memoria compartida.


//Estructura para las colas de mensajes
typedef struct {
	long sentido;          // Indica el sentido de envio
	int origen;            // Pid del proceso origen
	int destino;           // Pid del proceso destino
  char modo;             // Memoria compartida o socket
	int opcion;            // Indica si es opcion ECO o TF
	int fin;               // Fin de envio de archivos (para fragmentaciones)
	char datos[TAM_MAX];   // Datos del mensaje
  char comando[TAM_CAD]; // Si es opcion TF, comando a introducir
} cola_msg;


//Estructura de la memoria compartida, el mensaje enviado
typedef struct {
  // int patron;
	int origen;            // Pid del proceso origen  
	int destino;           // Pid del proceso destino
	int longitud;          // Longitud del mensaje
	int error;             // 1 si llega correctamente, 0 si no
	int opcion;            // 0 en eco, 1 en ftp
	// int tipo;            // 0: data; 1: ACK; 2: ERROR
	int fin;               // Indica fin de envio de archivos
	char datos[TAM_MAX];   // Datos del mensaje
	char comando[TAM_CAD]; // Comandos: cd, dir, get, quit
} memo_msg;

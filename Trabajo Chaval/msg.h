/******************************************************************************/
/*																			  */
/*								msg.h:										  */
/*	Aqui se declaran las estructuras que despues se usaran en los 			  */
/*	diferentes mecanismos IPC												  */
/*																	          */
/*																			  */
/******************************************************************************/
#define MAXDATA (1500) // Se define el tamanio maximo del mensaje.

/*Estructura para las colas de mensajes*/

typedef struct 
{
	long msgtype; //Indica el sentido de envio de mensajes en la cola
	char msgmodo;     // Para decir si es memoria compartida o socket
	char msgcadena[10]; // Para decir si es cd, dir, get o quit
	int msgprotocolo; // 1 unidireccional y 2 bidireccional
	int msgorigen;    // Ponemos el pid del proceso
	int msgdestino;   // El pid del proceso destino
	int msgopcion; // si es 0 es eco, si es 1 es ftp
	int msgfinal; // Marca de fin de envio de archivos
	char msgdatos[MAXDATA]; // Datos del mensaje
} msg_C;

//Estructura de la memoria compartida, el mensaje que pasamos

typedef struct
{
	int msgprotocolo; /* 1 unidireccional y 2 bidireccional */
	int msgorigen;    
	int msgdestino;
	int msglongitud;
	int msgerror; // pone un 1 si le llega y si no llega pone un 0
	int msgopcion; //Si es 0 es eco, si es 1 es ftp
	int msgtipo;     /* 0: data, 1: ACK y 2: ERROR */
	int msgfinal; // Marca de fin de envio de archivos
	char msgdatos[MAXDATA];
	char msgcadena[10]; //Para decir si es cd, dir, get o quit

} msg_M;

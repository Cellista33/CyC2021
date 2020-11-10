#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
extern int errno;

#define MAXDATA (1500) //Definimos el tamaño máximo posible
#define PERMS 0666


typedef struct {
	long msgtype; //Indica el sentido de envio de mensajes en la cola
	char msgmodo;     // Para decir si es memoria compartida o socket
	char msgcadena[10]; // Para decir si es cd, dir, get o quit
	int msgprotocolo; // 1 unidireccional y 2 bidireccional
	int msgorigen;    // Ponemos el pid del proceso
	int msgdestino;   // El pid del proceso destino
	int msgopcion; // si es 0 es eco, si es 1 es ftp
	int msgfinal; // Marca de fin de envio de archivos
	char msgdatos[MAXDATA]; // Datos del mensaje
} cola_msg;

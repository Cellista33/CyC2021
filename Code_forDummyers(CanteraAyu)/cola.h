#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
extern int errno;

#define MAXDATA_COLA (1500) //Definimos el tamaño máximo posible
#define PERMS 0666

#define CLAVE1 ftok("./cola.h", 'a')
#define CLAVE2 ftok("./cola.h", 'b')
#define 

typedef struct {
	long type; //Indica el sentido de envio de mensajes en la cola
	char modo;     // Para decir si es memoria compartida o socket
	char cadena[10]; // Para decir si es cd, dir, get o quit
	int protocolo; // 1 unidireccional y 2 bidireccional
	int origen;    // Ponemos el pid del proceso
	int destino;   // El pid del proceso destino
	int opcion; // si es 0 es eco, si es 1 es ftp
	int final; // Marca de fin de envio de archivos
	char datos[MAXDATA_COLA]; // Datos del mensaje
} cola_msg;
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
extern int errno;

#define PERMS 0666  // Permisos 
#define SHMKEY ftok("./memo_comp.h", 'c') // Clave para la memoria compartida 

typedef struct {

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

} m_compartida;

#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
extern int errno;
#define MAXDATA_MEMO (1024)
#define PERMS 0666  // Permisos 
#define SHMKEY ftok("./memo_comp.h", 'c') // Clave para la memoria compartida 

typedef struct {

  char patron;
	unsigned int protocolo; /* 1 unidireccional y 2 bidireccional */
	unsigned int origen;    
	unsigned int destino;
	unsigned int longitud;
	bool opcion; //Si es 0 es eco, si es 1 es ftp
	unsigned int tipo;     /* 0: data, 1: ACK y 2: ERROR */
	int final; // Marca de fin de envio de archivos
	char datos[MAXDATA];
	char cadena[10]; //Para decir si es cd, dir, get o quit

} m_compartida;
// Tamano int: 4bytes
/* Quitamos el campo de erro, porque en el enunciado emplea el parámetro tipo para esa misma función.
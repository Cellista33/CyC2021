#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "usu.h"

extern int errno;





#define PERMS 0666

#define CLAVE1 ftok("./cola.h", 'a')
#define CLAVE2 ftok("./cola.h", 'b')




/**
*	\brief Esctructura de de la comunicación por cola de mensajes. 
*
*	Estructura diseñada para el intercambio de PDU entre la entidadA y entidadB	
*
*
*/






typedef struct {

	/** Campo necesario en el mecanismo de cola de mensajes*/
	long type; 
	
	/** Indicamos si utilizaremos memoria compartida(m) o sockets(s) */
	char modo;     
	
	unsigned int origen;
	/** PID del proceso donde se entrega el mensaje */    
	unsigned int destino;
	
	/** Tamaño de la estructura asociada al intercambio de datos */
	unsigned int longitud;
	
	/** Estrcutra de datos donde se manda el mensaje y datos asociados*/ 
	struct datos_usuario dat_que; ///  

} cola_msg;

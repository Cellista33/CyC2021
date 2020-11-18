#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
//#include "usu.h"


extern int errno;

#define PERMS 0666  // Permisos 
#define SHMKEY ftok("./memo_comp.h", 'c') // Clave para la memoria compartida 






/**
*	\brief Esctructura del protocolo de memoria compartida
*
*	Estructura diseñada para el intercambio de PDU entre la entidadA y entidadB	
*
*
*/

typedef struct {
	/** Patron fijo que permite identificar el mensaje "/" */
  	char patron;
  	
  	/** PID del proceso donde se origina el mensaje */
	unsigned int origen;
	
	/** PID del proceso donde se entrega el mensaje */    
	unsigned int destino;
	
	/** Tamaño de la estructura asociada al intercambio de datos */
	unsigned int longitud;
	
	/** 
	*	\brief Tipo de mensaje
	*
	*	0: Si se están transmitiendo datos.
	*	1: Si se quiere comunicar un ACK.
	*	2: si se quiere comunicar un NAK.
	*/
	unsigned char tipo;    
	
	
	/** Estrcutra de datos donde se manda el mensaje y datos asociados*/ 
	struct datos_usuario datito; ///  

} m_compartida;


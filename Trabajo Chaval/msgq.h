/******************************************************************************/
/*																			  */
/*								msgq.h:								  */
/*	Aqui se declaran las llaves que despues se usaran para crear los 		  */
/*	diferentes mecanismos IPC												  */
/*																	          */
/*																			  */
/******************************************************************************/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>
extern int	errno;

#define PERMS 0666  /* Permisos */

#define MKEYQ1 ftok("./msgq.h", 'a')  /*Clave para la cola 1 */

#define MKEYQ2 ftok("./msgq.h", 'b')  /* clave para la cola 2 */

#define SHMKEY ftok("./msgq.h", 'c') /* Calve para la memoria compartida */

#define SEMKEY ftok("./msq.h", 'd') /* Clave para el semaforo */



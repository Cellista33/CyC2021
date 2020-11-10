#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>
extern int	errno;

#define PERMS 0666  /* Permisos */



#define SEMKEY ftok("./msq.h", 'd') /* Clave para el semaforo */

/*Estructura del semaforo*/
union semun {
  int val; 
  struct semid_ds *buf;
  ushort *array;
}semctl_arg;


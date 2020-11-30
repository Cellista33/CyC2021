//----------------------------------------------------------------------------//
//	msgq.h: Declaración de las llaves para crear mecanismos IPC								//
//----------------------------------------------------------------------------//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>
extern int	errno;

#define PERMS 0666                        // Permisos

#define MKEYQ1 ftok("./msgClave.h", 'a')  // Clave para cola 1

#define MKEYQ2 ftok("./msgClave.h", 'b')  // Clave para cola 2

#define SHMKEY ftok("./msgClave.h", 'c')  // Clave para memoria compartida

#define SEMKEY ftok("./msgClave.h", 'd')  // Clave para semaforo
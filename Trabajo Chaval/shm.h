#include	"mesg.h"

#define	NBUFF	4	/* number of buffers in shared memory */
			/* (for multiple buffer version) */

#define	SHMKEY	ftok("./shm.h", 'a') /* base value for shmem key */

#define	SEMKEY1	ftok("./shm.h", 'b') /* client semaphore key */
#define	SEMKEY2	ftok("./shm.h", 'c') /* server semaphore key */

#define	PERMS	0666


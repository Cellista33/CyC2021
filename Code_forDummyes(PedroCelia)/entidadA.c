//----------------------------------------------------------------------------//
//	entidadA.c: Recibe intrucciones de suario 1 y las envía a Entidad B		  	//
//----------------------------------------------------------------------------//

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "msg.h"
#include "msgClaves.h"

// Creacion estructura para cola de mensajes
cola_msg colaMsg;
// Declaracion de identificadores para memoria compartida y semaforos
int shmid, sem;
// Creacion de puntero a la estructura de memoria compartida
memo_msg  *memoPtr;


// Definicion de la estructura del semaforo
union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
} semctl_arg;


// Definición de funciones para manejo de semaforos
// sem 0 regula acceso de la entidad A, sem 1 lo hace para entidad B
void abre_sem(int num)
{
    struct sembuf operador;
    // Semaforo seleccionado
    operador.sem_num = num;
    // Incrementa 1, abre
    operador.sem_op = 1;
    // Numero de bloqueo
    operador.sem_flg = 0;
    if(semop(sem, &operador, 1) < 0)
    {
        perror("\nERROR: Fallo en las operaciones del semaforo");
    }
}

void cierra_sem(int num)
{
    struct sembuf operador;
    // Semaforo seleccionado
    operador.sem_num = num;
    // Decrementa 1, cierra
    operador.sem_op = -1;
    // Numero de bloqueo
    operador.sem_flg = 0;
    if(semop(sem, &operador, 1) < 0)
    {
        perror("\nERROR: Fallo en las operaciones del semaforo");
    }
}


main ()
{
    // Variable para almacenar ID de cola 1
    int IDcola1;

    // Creación de cola de mensajes
    if( (IDcola1 = msgget(MKEYQ1, PERMS|IPC_CREAT)) < 0 )
    {
        perror("\nERROR: No se pudo obtner ID de la cola");
        exit(EXIT_FAILURE);
    }

    // Comexión a la memoria compartida
    if ( (shmid = shmget(SHMKEY, sizeof(memo_msg),0)) < 0)
    {
        perror("\nERROR: Fallo de conexión a la memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Asociación de puntero a memoria compartida
    if ( (memoPtr = (memo_msg *) shmat(shmid, (char *)0, 0)) == (memo_msg *) -1)
    {
        perror("\nERROR: Fallo al asociar puntero a memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Creación de semaforos
    if(( sem = semget(SEMKEY, 2, 0)) < 0)
    {
        perror("\nERROR: Fallo en la creación de los semáforos");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        // El modo por defecto es memoria compartida
        colaMsg.modo = 'm';
        // Reinicio del flag de final en cada bucle
        memoPtr->fin = 0;

        // Recepción de usuario A por cola de mensajes
        printf("\nEsperando usuario 1");
        if ((msgrcv(IDcola1, &colaMsg, sizeof(cola_msg)-sizeof(long), 1L, 0)) < 0 )
        {
            perror("\nERROR: Fallo en la recepción por cola");
            fflush(stdout);
            exit(EXIT_FAILURE);
        }
        printf("\nRecibido de usuario 1");

        // En esta práctica es siempre memoria compartida. Dejamos praparado para añadir sockets
        if(colaMsg.modo == 'm')
        {

            // Toma el semaforo 0
            printf("\nControl sobre sem0");
            cierra_sem(0);

            //Introducción en memoria compartida los datos obtenidos por la cola de mensajes
            printf("\nEscritura en memoria compartida");

            // Inclusion del patron de encabezado en la comunicacion entre entidades

            memoPtr->patron = 92;

            // Copia de campos de cola de mensajes a memoria compartida
            memoPtr->origen = colaMsg.origen;
            memoPtr->destino = colaMsg.destino;
            memoPtr->opcion = colaMsg.opcion;
            strcpy(memoPtr->comando, colaMsg.comando);
            strcpy(memoPtr->datos, colaMsg.datos);

            printf("\nEscritura finalizada en memoria compartida");

            // Abrimos semaforo 1 permitiendo acceder a la memoria compartida a Entidad 2
            abre_sem(1);
            printf("\nsem1 abierto");

            // Si el comando es "quit" el programa finaliza. El resto de procesos ya fueron informados por memoria compartida.
            if(strcmp(colaMsg.comando,"quit") == 0)
            {
                if ((msgctl(IDcola1, IPC_RMID, (struct msqid_ds *) NULL)) < 0)
                {
                  perror("\nERROR: Fallo al borrar la cola");
                  exit(EXIT_FAILURE);
                }
                printf("\nCierre de programa");
                exit(EXIT_SUCCESS);
            }

            // Activo hasta que se reciba el flag de fin de mensaje
            while(memoPtr->fin != 1)
            {
                printf("\nBucle de lectura");
                printf("\nCierre sem0 para leer mem");
                cierra_sem(0);

                printf("\nEscritura en cola");

                // Lectura de memoria compartida
                colaMsg.origen = memoPtr->origen;
                colaMsg.destino = memoPtr->destino;

                colaMsg.sentido = memoPtr->destino;

                strcpy(colaMsg.datos, memoPtr->datos);


                // Envío por cola de mensajes a Usuario 1 (interfaz)
                msgsnd(IDcola1, &colaMsg, sizeof(cola_msg)-sizeof(long), 0);
                printf("\nEnvio a Usuario 1 por cola");

                // Apertura de sem1 si se llega al final del mensaje
                if(memoPtr->fin != 1)
                {
                    printf("\nLectura completada");
                    abre_sem(1);
                    printf("\nsem1 abierto");
                }
            }

            // Limpieza de variables antes del ultimo mensaje
            strcpy(colaMsg.datos,"");
            colaMsg.fin = 1;

            // Envío del ultimo mensaje
            msgsnd(IDcola1, &colaMsg, sizeof(cola_msg) - sizeof(long), 0);

            // Apertura de sem0
            abre_sem(0);
            printf("\nMensaje finalizado");
            printf("\nApertura de sem0");
        }
        printf("\nEnvio de datos a Usuario 1 completado");
    }
}

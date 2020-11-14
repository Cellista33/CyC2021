//-----------------------------------------------------------------------------//
//	entidad B.c: Recibe de entidad A por memoria compara y envía a Usuario 2 //
//-----------------------------------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "msg.h"
#include "msgq.h"

// Creacion estructura para cola de mensajes
cola_msg colaMsg;
// Declaracion deentificadores para memoria compara y semaforos
int shmid, sem;
// Creacion de puntero a la estructura de memoria compara
memo_msg  *memoPtr;


// Definicion de la estructura del semaforo
union semun
{
    int val;
    struct se_ds *buf;
    ushort *array;
} semctl_arg;


// Definición de funciones para manejo de semaforos
// sem 1 regula acceso de la enad B, sem 0 lo hace para enad A
void abre_sem(int num)
{
    struct sembuf operador;
    // Semaforo seleccionado
    operador.sem_num = num;
    // Incrementa 1, abre
    operador.sem_op = 1;
    // Numero de bloqueo
    operador.sem_flg = 0;
    if(semop(sem,  &operador, 1) < 0)
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
    if(semop(sem,  &operador, 1) < 0)
    {
        perror("\nERROR: Fallo en las operaciones del semaforo");
    }
}


main ()
{
    // Variable para almacenar ID de cola 2
    int IDcola2;
    // Variable para comprobar que se cree bien la SHM
    int memo_compr;

    // Creacion de la cola de mensajes
    if((IDcola2 = msgget(MKEYQ2, PERMS | IPC_CREAT)) < 0 )
    {
        perror("\nERROR: Fallo al crear la cola");
        exit(EXIT_FAILURE);
    }

    // Obtención del identificador para la memoria compartida
    if ((memo_compr = SHMKEY) == (key_t) -1)
    {
        printf("\nERROR: Fallo al crear la clave de la memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Conexion a la memoria compartida
    if ( (shmid = shmget(SHMKEY, sizeof(memo_msg),PERMS|IPC_CREAT)) < 0)
    {
        perror("\nERROR: Fallo al conectarse a memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Asociación de puntero a la memoria compartida
    if ( (memoPtr = (memo_msg *) shmat(shmid, (char *) 0, 0)) == (memo_msg *) -1)
    {
        perror("\nERROR: Fallo al asociar puntero a memoria compartida");
        exit(EXIT_FAILURE);
    }


    // Creación de semaforos
    if(( sem = semget(SEMKEY, 2, PERMS|IPC_CREAT)) <0)
    {
        perror("\nERROR: Fallo en la creación de los semáforos");
        exit(EXIT_FAILURE);
    }

    // Inicializacion de sem0 a 1
    semctl_arg.val = 1;
    if(semctl(sem, 0, SETVAL, semctl_arg) < 0)
    {
        perror("\nERROR: Fallo al inicializar sem0");
        exit(EXIT_FAILURE);
    }

    // Inicializacion de sem1 a 0
    semctl_arg.val = 0;
    if(semctl(sem, 1, SETVAL, semctl_arg) < 0)
    {
        perror("\nERROR: Fallo al inicializar sem1");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        // El modo por defecto es memoria compartida
        colaMsg.modo = 'm';

        // Comunicacion por memoria compartida. Dejamos margen para añadir sockets
        if (colaMsg.modo == 'm')
        {
            // Cierra sem1
            printf("\nControl sobre sem1");
            cierra_sem(1);

            // Reconocimiento de patron de encabezado
            if(memoPtr->patron == 92) {
              printf("\nMesaje recibido de entidad A");
            }
            else {
              perror("\nERROR: PAtron de encabezado no reconocido");
              exit(EXIT_FAILURE);
            }

            // Lectura de la memoria compartida. Introducción en la cola de mensajes 2
            colaMsg.sentido = memoPtr->destino; //Nos lo cargamos??
            colaMsg.origen = memoPtr->destino;
            colaMsg.destino = memoPtr->destino;
            //colaMsg.msgprotocolo = memoPtr->msgprotocolo;
            colaMsg.opcion = memoPtr->opcion;
            strcpy(colaMsg.datos, memoPtr->datos);
            strcpy(colaMsg.comando, memoPtr->comando);

            printf("\nLectura de memoria compartida finalizada");

            // Envio por cola a Usuario 2
            printf("\nEnvia a Usuario 2");

            //msgsndcola2, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), 0);

            msgsnd(IDcola2, (char *) &colaMsg, sizeof(cola_msg)-sizeof(long), 0);

            // Si el comando es "quit" el programa finaliza. El resto de procesos ya fueron informados por memoria compartida.
            if(strcmp(colaMsg.comando, "quit") == 0)
            {

                // Espera a que Usuario 2 lea la cola
                sleep(2);

                // Liberacion de memoria compartida
                if(shmdt((char *) memoPtr) < 0)
                {
                    perror("\nERROR: Fallo al liberar la memoria");
                    exit(EXIT_FAILURE);
                }

                // Borrado de memoria compartida
                if(shmctl(shmid, IPC_RMID, NULL) < 0)
                {
                    perror("\nERROR: Fallo al eliminar la memoria");
                    exit(EXIT_FAILURE);
                }

                // Borrado de cola 2
                if((msgctl(IDcola2, IPC_RMID, (struct msqid_ds *)NULL)) < 0)
                {
                    perror("\nERROR: Fallo al borrar la cola");
                    exit(EXIT_FAILURE);
                }

                // Borrado de semaforos
                if(semctl(sem,2,IPC_RMID,semctl_arg) < 0)
                {
                    printf("\nERROR: Fallo al eliminar semaforos");
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);
            }

            // Sentido de la cola
            colaMsg.sentido = 2L;

            // Lectura de contenido de la cola 2, enviado por Usuario 2
            // msgrcvcola2, (cola_msg *) &colaMsg, sizeof (cola_msg) - sizeof(long), colaMsg.sentido, 0);

            msgrcv(IDcola2, &colaMsg, sizeof(cola_msg)-sizeof(long), 1L, 0);
            printf("\nMensaje de Usuario 2 leido");

            // Escritura en memoria compartida
            printf("\nEscribiendo en memoria compartida...");

            memoPtr->origen = colaMsg.origen;
            memoPtr->destino = colaMsg.destino;
            // memoPtr->msgprotocolo = colaMsg.msgprotocolo;
            strcpy(memoPtr->datos, colaMsg.datos);

            // Abrimos sem0 para permitir acceso a memoria de Entidad A
            abre_sem(0);
            printf("\nEscrito en memoria, sem0 abierto");

            // Indicacion del sentido y reseteo del flag de fin
            colaMsg.sentido = 2L;
            colaMsg.fin = 0;
            while (colaMsg.fin != 1)
            {
                // Cierra sem1 para leer
                cierra_sem(1);

                // Lectura de cola 2

                (msgrcv(IDcola2, &colaMsg, sizeof(cola_msg)-sizeof(long), 1L, 0));

                // msgrcvcola2, (cola_msg *) &colaMsg, sizeof (cola_msg) - sizeof(long), colaMsg.sentido, 0);

                printf("\nLectura de ususario 2");
                printf("\nSe procede a la escirtura en memoria compartida");

                //Introducción en memoria compartida los datos obtenidos por la cola de mensajes
                memoPtr->origen = colaMsg.origen;
                memoPtr->destino = colaMsg.destino;
                // memoPtr->msgprotocolo = colaMsg.msgprotocolo;
                strcpy(memoPtr->datos, colaMsg.datos);

                // Abrimos sem0 para permitir acceso de la Entidad A a memoria
                abre_sem(0);

                printf("\nMemoria escrita, sem0 abierto");
            }

            memoPtr->fin = 1;
            printf("\nMemoria compartida escrita para Entidad A");
        }
    }
}

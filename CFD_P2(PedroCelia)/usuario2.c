//---------------------------------------------------------------------------------//
//	Usuario 2: Recibe por cola de mensajes de la entidad 2. Según modo:						 //
//	  -Eco: Invierte origen y destino en cabecera y devuelve a origen.		         //
//    -TF: Ejecuta el comando. Si debe enviar algo a usuario1, 			               //
//          lo hace a través de cola por la entidad B.		                         //
//---------------------------------------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include "msg.h"
#include "msgClaves.h"

// Creamos una colo de tipo de dato cola_msg
cola_msg colaMsg;

void main ()
{
    // Variable para almacenar ID de cola 2
    int IDcola2;

    // Mostramos el pid del usuario2
    printf("\nPID de usuario 2: %d", getpid());

    // Se intenta conectar la cola 2 con la entidad 2
    if( (IDcola2 = msgget(MKEYQ2, 0)) < 0 )
    {
        perror("\nERROR: No se ha podido conectar a la cola");
        exit(EXIT_FAILURE);
    }

    // Se fija el campo tipo en el sentido Entidad 2 -> usuario B
    colaMsg.sentido = getpid();

    while(1)
    {
        // Declaramos un puntero y una estructura para poder usar funciones dir
        DIR *fdir;
        struct dirent directorio, *p_dir;
        p_dir = &directorio;

        // Recepción de mensajes por la cola
        puts("\nEsperando envio de la entidad B...");

        //msgrcv(IDcola2, (cola_msg *) &colaMsg, sizeof(colaMsg) - sizeof(long), colaMsg.sentido, 0);

        if ((msgrcv(IDcola2, &colaMsg, sizeof(cola_msg)-sizeof(long), colaMsg.sentido, 0)) < 0 )
        {
            perror("\nERROR: Fallo en la recepción por cola");
            fflush(stdout);
            exit(EXIT_FAILURE);
        }
        puts("\nRecibido de usuario A");

        // Invierte el sentido de envío de la cola
        colaMsg.sentido = colaMsg.origen;
        colaMsg.origen = colaMsg.destino;
        colaMsg.destino = colaMsg.sentido;
        colaMsg.sentido = 2L;

        // Modo ECO
        if(colaMsg.opcion == 0)
        {
            // Mostramos resultados por pantalla
            puts("\nModo ECO");

            // Envío de datos
            msgsnd(IDcola2, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), 0);
            puts("\nEnvio de datos a entidad B");

            // Envío del ultimo mensaje
            colaMsg.fin = 1;
            strcpy(colaMsg.datos,"");
            msgsnd(IDcola2, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), 0);
        }

        // Modo TF
        else
        {
            // Reseteo del flag del mensaje
            colaMsg.fin = 0;

            // Se recibe comando "dir"
            if(strcmp(colaMsg.comando,"dir") == 0 )
            {
                fdir = opendir(".");

                // Extraemos toda la información del directorio actual
                while ((p_dir = readdir(fdir)) != NULL)
                {
                    // Se muestran los elementos por pantalla
                    printf("%s\n", p_dir->d_name);
                    strcpy(colaMsg.datos,p_dir->d_name);
                    fflush(stdout);
                    // Envío por la cola hacia usuario 1
                    msgsnd(IDcola2, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), 0);
                }

                if((p_dir = readdir(fdir)) == NULL)
                {
                    // Envío del mensaje de fin
                    strcpy(colaMsg.datos,"");
                    colaMsg.fin=1;
                    msgsnd(IDcola2, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), 0);
                }

                printf("\nDatos enviados al usuario 1");
                // cierre del directorio
                closedir(fdir);
            }

            // Se recibe comando "cd"
            else if(strcmp(colaMsg.comando,"cd") == 0)
            {
                // Cambio al directorio cuyo nombre ha indicado usuario 1
                printf("\nCambiando al directorio indicado por el usuario: %s",colaMsg.datos);
                // chdir es una llamada a sistema que cambia de directorio
                chdir(colaMsg.datos);
                fdir = opendir(".");

                // Se muestra el contenido del directorio
                while ((p_dir = readdir(fdir)) != NULL)
                {
                    printf("%s\n", p_dir->d_name);
                    // Limpia buffer y lo presenta en consola
                    fflush(stdout);
                }

                // Comprobación de que el directorio se ha abierto correctamente
                if(chdir(colaMsg.datos) == 0)
                {
                    // Envío por cola de confirmación de apertura
                    strcpy(colaMsg.datos,"Directorio correctamente abierto");
                    msgsnd(IDcola2, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), 0);

                    // Envío de mensaje final
                    colaMsg.fin = 1;
                    strcpy(colaMsg.datos,"");
                    msgsnd(IDcola2, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), 0);
                    printf("\nDirectorio correctamente abierto");
                }

                else
                {
                    // Envío de aviso de fallo en la apertura y lanzamiento de error
                    strcpy(colaMsg.datos,"Error al abrir el directorio");
                    msgsnd(IDcola2, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), 0);

                    perror("\nERROR: Fallo en la apertura del directorio");
                    exit(EXIT_FAILURE);
                }
            }

            else if(strcmp(colaMsg.comando,"get") == 0 )
            {
                // Se recibe comando "get"
                chdir(colaMsg.datos);
                fdir = opendir(".");

                // Comprobación del fichero
                if(chdir(colaMsg.datos)==0)
                {
                    // Envío de los datos del fichero
                    while ((p_dir = readdir(fdir)) != NULL)
                    {
                        strcpy(colaMsg.datos,p_dir->d_name);
                        fflush(stdout);
                        msgsnd(IDcola2, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), 0);
                    }

                    // Envío del mensaje de fin
                    if((p_dir = readdir(fdir))== NULL)
                    {
                        strcpy(colaMsg.datos,"");
                        colaMsg.fin = 1;
                        msgsnd(IDcola2, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), 0);
                        printf("\nDatos enviados al usuario A");
                    }
                }
                // Si no se encuentra el fichero
                else
                {
                    // Informamos a usuario de que no se encuentra el archivo
                    strcpy(colaMsg.datos,"\nNo se encuntra el archivo");
                    msgsnd(IDcola2, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), 0);

                    // Envío del mensaje de fin
                    colaMsg.fin = 1;
                    strcpy(colaMsg.datos,"");
                    msgsnd(IDcola2, (cola_msg *) &colaMsg, sizeof(cola_msg) - sizeof(long), 0);
                    printf("\nNo se encuntra el archivo");
                }
                closedir(fdir);
            }

            // Se recibe comando "quit", cierre de programa
            else if(strcmp(colaMsg.comando,"quit") == 0)
            {
                printf("\nCierre de programa");
                exit(EXIT_SUCCESS);
            }

            // Se recibe un comando desconocido, cierre por error
            else
            {
                perror("\nERROR: Se ha recibido un comando desconocido");
                exit(EXIT_FAILURE);
            }
        }
    }
}


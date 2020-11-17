//----------------------------------------------------------------------------//
//	msg.h: Declaración de las estructuras usadas en mecanismos IPC			      //
//----------------------------------------------------------------------------//
#define TAM_MAX (1500) // Tamaño maximo del mensaje de cola.
#define TAM_CAD (10) // Tamaño maximo del mensaje de memoria compartida.


//Estructura para las colas de mensajes
typedef struct
{
    long sentido;          // Indica el sentido de envio
    int origen;            // Pid del proceso origen
    int destino;           // Pid del proceso destino
    char modo;             // Memoria compartida o socket
    int opcion;            // Indica si es opcion ECO o TF
    char comando[TAM_CAD]; // Si es opcion TF, comando a introducir
    int fin;               // Fin de envio de archivos (para fragmentaciones)
    char datos[TAM_MAX];   // Datos del mensaje
} cola_msg;


//Estructura de la memoria compartida, el mensaje enviado
typedef struct
{
    int patron;            // Patron que marca el inicio del mensaje
    int destino;           // Pid del proceso destino
    int origen;            // Pid del proceso origen
    int longitud;          // Longitud del mensaje

    //Campo tipo:
    int tipo;           // 0: data; 1: ACK; 2: ERROR
    //int opcion;            // 0 en eco, 1 en ftp Lo ponemos??
    char comando[TAM_CAD]; // Comandos: cd, dir, get, quit
    int fin;               // Indica fin de envio de archivos

    //int error;             // 1 si llega correctamente, 0 si no

    char datos[TAM_MAX];   // Datos del mensaje

} memo_msg;

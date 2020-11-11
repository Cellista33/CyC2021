


/*Definimos el semáforo */
union sema{
  int val;
  struct semid_ds *buf;
  unsigned short int *array;  
} sema_def;
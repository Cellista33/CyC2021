#include <sys/types.h>

#define MAXDATA_U (1014)
typedef struct datos_usuario{

	unsigned char cli;
	unsigned char frag;
	char data[MAXDATA_U];
	

};


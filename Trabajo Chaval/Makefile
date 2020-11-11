
all: entidadA entidadB usuario1 usuario2


entidadA: entidadA.c msg.h msgq.h
	cc -o entidadA entidadA.c

entidadB: entidadB.c msg.h msgq.h
	cc -o entidadB entidadB.c
usuario1: usuario1.c msg.h msgq.h
	cc -o usuario1 usuario1.c
usuario2: usuario2.c msg.h msgq.h
	cc -o usuario2 usuario2.c

clean:
	rm entidad1 entidadB usuario1 usuario2 *.o

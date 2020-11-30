
all: entidadA entidadB usuario1 usuario2


entidadA: entidadA.c entidadA.h sema.h memo_comp.h cola.h  usu.h
	cc -o entidadA entidadA.c

entidadB: entidadB.c entidadB.h sema.h memo_comp.h cola.h usu.h
	cc -o entidadB entidadB.c
usuario1: usuario1.c usuario1.h cola.h sema.h usu.h
	cc -o usuario1 usuario1.c
usuario2: usuario2.c usuario2.h cola.h sema.h usu.h
	cc -o usuario2 usuario2.c

clean:
	rm entidad1 entidadB usuario1 usuario2 *.o

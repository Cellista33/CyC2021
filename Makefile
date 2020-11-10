all: entidadA entidadB

entidadA: entidadA.c entidadA.h sema.h memo_comp.h cola.h
	cc -o entidadA entidadA.c
	
	
entidadB: entidadB.c entidadB.h sema.h memo_comp.h cola.h
	cc -o entidadB entidadB.c
	
clean:
	rm entidadA entidadB *.o
	
	

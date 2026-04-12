LIBDIR = C:\Users\gabri\Documents\PROJETOS\C\MAC 2\EP 3
CFLAGS = -g -Wall -std=c99 -pedantic -Wno-unused-result
###########################################################################

EP3: EP3.o stack.o polinomios.o
	gcc $(CFLAGS) EP3.o stack.o polinomios.o -o EP3

EP3.o: EP3.c stack.h item.h polinomios.h
	gcc $(CFLAGS) -c EP3.c

stack.o: stack.c stack.h item.h
	gcc $(CFLAGS) -c stack.c

polinomios.o: polinomios.c polinomios.h item.h
	gcc $(CFLAGS) -c polinomios.c
# Ajuste os diretorios abaixo conforme o seu sistema 
LIBDIR = "C:\Users\gabri\Documents\PROJETOS\C\MAC 2\EP 5"
CFLAGS = -g -Wall -std=c99 -pedantic -Wno-unused-result
###########################################################################

all: ep5

ep5: ep5.o 
	gcc -o ep5 ep5.o

ep5.o: ep5.c
	gcc $(CFLAGS) -I$(LIBDIR) ep5.c -c 

clean:
	del /Q *.o ep5.exe
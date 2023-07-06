CFLAGS = -Wall -pedantic -std=c99 -c -g -DTTF
LFLAGS = -lSDL2 -lSDL2_ttf -lm

programa: lista.o masas.o resortes.o malla.o dibujo.o simulacion.o main.o 
	gcc lista.o masas.o resortes.o malla.o dibujo.o simulacion.o main.o -o programa $(LFLAGS)

lista.o: lista.c lista.h
	gcc $(CFLAGS) lista.c

masas.o: masas.c masas.h
	gcc $(CFLAGS) masas.c

resortes.o: resortes.c resortes.h
	gcc $(CFLAGS) resortes.c

malla.o: malla.c malla.h
	gcc $(CFLAGS) malla.c

dibujo.o: dibujo.c dibujo.h
	gcc $(CFLAGS) dibujo.c 

simulacion.o: simulacion.c simulacion.h
	gcc $(CFLAGS) simulacion.c 

main.o: main.c config.h dibujo.h lista.h resortes.h masas.h malla.h simulacion.h
	gcc $(CFLAGS) main.c

clean:
	rm *.o
	rm programa

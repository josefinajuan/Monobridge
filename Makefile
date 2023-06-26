CFLAGS = -Wall -pedantic -std=c99 -c -g 
LFLAGS = -lm -lSDL2

programa: lista.o masas.o malla.o resortes.o dibujo.o main.o 
	gcc lista.o masas.o malla.o resortes.o dibujo.o main.o -o programa $(LFLAGS)

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

main.o: main.c dibujo.h lista.h malla.h masas.h resortes.h
	gcc $(CFLAGS) main.c

clean:
	rm *.o
	rm programa

CFLAGS = -Wall -pedantic -std=c99 -c -g
LFLAGS = -lm

programa: lista.o malla.o masas.o resortes.o main.o
	gcc lista.o malla.o masas.o resortes.o main.o -o programa $(LFLAGS)

lista.o: lista.c lista.h
	gcc $(CFLAGS) lista.c

malla.o: malla.c malla.h
	gcc $(CFLAGS) malla.c

masas.o: masas.c masas.h
	gcc $(CFLAGS) masas.c

resortes.o: resortes.c resortes.h
	gcc $(CFLAGS) resortes.c

main.o: main.c lista.h malla.h masas.h resortes.h
	gcc $(CFLAGS) main.c

clean:
	rm *.o
	rm programa

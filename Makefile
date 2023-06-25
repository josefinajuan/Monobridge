CFLAGS = -Wall -pedantic -std=c99 -c -g
LFLAGS = -lm -lSDL2

programa: lista.o masas.o main.o
	gcc lista.o masas.o main.o -o programa $(LFLAGS)

lista.o: lista.c lista.h
	gcc $(CFLAGS) lista.c

masas.o: masas.c masas.h
	gcc $(CFLAGS) masas.c

main.o: main.c lista.h malla.h masas.h resortes.h
	gcc $(CFLAGS) main.c

clean:
	rm *.o
	rm programa

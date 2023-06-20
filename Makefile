CFLAGS = -Wall -pedantic -std=c99 -c -g
LFLAGS = -lm

programa: pila.o complejos.o main_pila_generica.o
	gcc pila.o complejos.o main_pila_generica.o -o programa $(LFLAGS)

pila.o: pila.c pila.h
	gcc $(CFLAGS) pila.c

complejos.o: complejos.c complejos.h
	gcc $(CFLAGS) complejos.c

main_pila_generica.o: main_pila_generica.c complejos.h pila.h
	gcc $(CFLAGS) main_pila_generica.c

clean:
	rm *.o
	rm programa

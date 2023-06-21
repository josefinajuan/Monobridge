#ifndef MASAS_H
#define MASAS_H

#include <stdbool.h>

typedef struct masas masas_t;

//Libero desde adentro hacia afuera
//Pre: existe ya una masa
void masas_destruir(masas_t *masas);

//Creo primer masa
masas_t *_masas_crear(size_t f, size_t c);

//Doy cantidad de filas
size_t masas_filas(const masas_t *masas);

//Doy cantidad de filas y de columnas
void masas_dimensiones(const masas_t *masas, size_t *filas, size_t *columnas);

//Agrego masa 
double** masa_agregar(double** masas, size_t* f, size_t c);

//falta determinar como saber si es fijar o no 
bool masa_es_fija(double **m, size_t f, size_t c);

#endif
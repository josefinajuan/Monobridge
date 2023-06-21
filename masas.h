#ifndef MASAS_H
#define MASAS_H

#include <stdbool.h>

typedef struct masas masas_t;

//libero desde adentro hacia afuera
void masas_destruir(masas_t *matriz);
//agrego masa 
double** masa_agregar(double** masas, size_t* f, size_t c);

//doy cantidad de filas
size_t masas_filas(const masas_t *masas);

//doy cantidad de filas y de columnas
void masas_dimensiones(const masas_t *masas, size_t *filas, size_t *columnas);

//agregar una masa nueva 
double** masa_agregar(double** masas, size_t* f, size_t c);

//falta determinar como saber si es fijar o no 
bool masa_es_fija(double **m, size_t f, size_t c);

#endif
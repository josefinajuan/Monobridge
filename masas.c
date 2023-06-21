#include "masas.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double **m;
    size_t filas, columnas;
} masas_t;


void masas_destruir(masas_t *masas){
    for(size_t i = 0; i < masas->filas; i++){ //libero desde adentro hacia afuera
        free(masas->m[i]); 
    }
    free(masas->m); 
    free(masas); 
}

masas_t *_masas_crear(size_t f, size_t c){
    masas_t *masas = malloc(sizeof(masas_t)); //pido memoria para variable llamada "matriz" que apunta a estructura tipo matriz_t
    if(masas == NULL){ //convalido que no apunte a NULL 
        return NULL; 
    }

    masas->filas = f; //doy valor a variable filas de la estructura
    masas->columnas = c; // " " " "columnas de la estructra

    masas->c = malloc(f * sizeof(double*)); //pido memoria para variable dentro de estructura llamada "m" que apunta a primera posicion de un arreglo de longitud de filas
    if (masas->c == NULL){ //convalido que no apunte a NULL
        free(masas);// libero de dentro hacia afuera
        return NULL;
    }

    for(size_t i = 0; i < f; i++){
        masas->m[i] = malloc(c * sizeof(double)); //pido memoria para que cada posicion del arreglo de longitud de filas apunte a un arreglo de longitud de columnas  
        if(masas->m[i] == NULL){ //convalido que no apunte a NULL
            masas->filas = i;
            masas_destruir(masas);
            return NULL;
        }
    }
    return masas;
}

size_t masas_filas(const masas_t *masas){
    return masas->filas;
}

void masas_dimensiones(const masas_t *masas, size_t *filas, size_t *columnas){
    *columnas = (masas->columnas);
    *filas = (masas->filas);
}

double** masa_agregar(double** masas, size_t* f, size_t c) {

    double** masas_totales = (double**) realloc (masas, (*f + 1) * sizeof(size_t*));
    masas_totales[*f] = (size_t*) malloc(c * sizeof(size_t));
    (*f)++; 
    return masas_totales;
}

//falta determinar como saber si es fijar o no 
bool masa_es_fija(double **m, size_t f, size_t c){
    if(m[f][c]= 0){
        return false;
    }
    return true;
}
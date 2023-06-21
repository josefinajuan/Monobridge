#include "masas.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    double **m;
    size_t filas, columnas;
} masas_t;


masas_t *_masas_crear(size_t f, size_t c){
    masas_t *masas = malloc(sizeof(masas_t));
    if (masas == NULL){
        return NULL;
    }

    masas->filas = f;
    masas->columnas = c;

    masas->c = malloc(f * sizeof(double *));
    if (masas->c == NULL){
        free(masas);
        return NULL;
    }

    for (size_t i = 0; i < f; i++){
        masas->m[i] = malloc(c * sizeof(double));
        if (masas->m[i] == NULL){
            masas->filas = i;
            masas_destruir(masas);
            return NULL;
        }
    }
    return masas;
}

void masas_destruir(masas_t *masas){
    for (size_t i = 0; i < masas->filas; i++)
    { // libero desde adentro hacia afuera
        free(masas->m[i]);
    }
    free(masas->m);
    free(masas);
}

size_t masas_filas(const masas_t *masas){
    return masas->filas;
}

void masas_dimensiones(const masas_t *masas, size_t *filas, size_t *columnas){
    *columnas = (masas->columnas);
    *filas = (masas->filas);
}

double **masa_agregar(double **masas, size_t *f, size_t c){

    double **masas_totales = (double **)realloc(masas, (*f + 1) * sizeof(size_t *));
    masas_totales[*f] = (size_t *)malloc(c * sizeof(size_t));
    (*f)++;
    return masas_totales;
}

// falta determinar como saber si es fijar o no
bool masa_es_fija(double **m, size_t f, size_t c){
    if (m[f][c] = 0)
    {
        return false;
    }
    return true;
}

masas_t *masas_rellenar(size_t n, size_t m, double a[n][m]){
    masas_t *masas = _masas_crear(n, m);
    if (masas == NULL)
    {
        masas_destruir(masas);
        return NULL;
    }
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            masas->m[i][j] = a[i][j];
        }
    }
    return masas;
}
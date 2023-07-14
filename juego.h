#ifndef JUEGO_H
#define JUEGO_H

#include "malla.h"

void nivel_inicio(malla_t *malla, int nivel);

bool leer_n_datos(FILE *f, size_t *datos);

bool leer_masa(FILE *f, size_t *id, float *x, float *y, float *tam, bool *es_fijo, float *masa);

bool leer_resorte(FILE *f, size_t *id, size_t *id_masa_1, size_t *id_masa_2, float *longitud);

bool escribir_n_datos(FILE *f, size_t datos);

bool escribir_masa(FILE *f, size_t id, float x, float y, float tam, bool es_fijo, float masa);

bool escribir_resorte(FILE *f, size_t id, size_t id_masa_1, size_t id_masa_2, float longitud);

bool nivel_guardar(FILE *f, malla_t *malla);

bool nivel_abrir(FILE *f, malla_t *malla);

int puntos_ganar(malla_t *malla, int nivel);

#endif
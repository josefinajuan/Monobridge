#ifndef JUEGO_H
#define JUEGO_H

#include "malla.h"

void inicializar_nivel(malla_t *malla, int nivel);

bool leer_numero_de_datos(FILE *f, size_t *datos);

bool leer_masa(FILE *f, size_t *id, float *x, float *y, float *tam, bool *es_fijo, float *masa);

bool leer_resorte(FILE *f, size_t *id, size_t *id_masa_1, size_t *id_masa_2, float *longitud);

bool escribir_numero_de_datos(FILE *f, size_t datos);

bool escribir_masa(FILE *f, size_t id, float x, float y, float tam, bool es_fijo, float masa);

bool escribir_resorte(FILE *f, size_t id, size_t id_masa_1, size_t id_masa_2, float longitud);

bool guardar_nivel(FILE *f, malla_t *malla);

bool abrir_nivel(FILE *f, malla_t *malla);

int cantidad_de_puntos(malla_t *malla, int nivel);

#endif
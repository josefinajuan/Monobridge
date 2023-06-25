#ifndef MASAS_H
#define MASAS_H

#include <stdbool.h>
#include "lista.h"

typedef struct masas masas_t;

struct masa* masas_crear(int id, int x, int y);

bool estoy_sobre_masa(int x, int y, lista_t* l);

void masas_borrar(int x, int y, lista_t *lista_masas); 

#endif
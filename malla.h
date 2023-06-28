#ifndef MALLA_H
#define MALLA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lista.h"
#include "resortes.h"
#include "masas.h"

typedef struct malla malla_t;

malla_t *malla_crear();

void malla_destruir(malla_t *m);

size_t malla_cantidad_masas(const malla_t* malla);

size_t malla_cantidad_resortes(const malla_t* malla);

bool estoy_sobre_masa(float coordx, float coordy, const malla_t *malla);

bool malla_eliminar_masa_por_coordenadas(malla_t *malla, float coordx, float coordy);

bool malla_agregar_masa(malla_t* malla, masa_t* masa);

bool malla_agregar_resorte(malla_t* malla, resorte_t* resorte);

void malla_actualizar_coord(masa_t* masa, float coordx, float coordy);

masa_t* obtener_masa(const malla_t* malla, float coordx, float coordy);

bool son_iguales(int x1, int y1, int x2, int y2);

#endif
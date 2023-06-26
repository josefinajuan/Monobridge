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

void malla_actualizar_coordx(malla_t *malla, size_t* id_masa, float coordx);

void malla_actualizar_coordy(malla_t *malla, size_t* id_masa, float coordy);

size_t obtener_id_masa_en_coordenadas(float x, float y, const malla_t* malla);

#endif
#ifndef RESORTES_H
#define RESORTES_H

#include "lista.h"
#include "masas.h"
#include "malla.h"

#define L0_MAX 3
#define K_BASE 200

typedef struct resorte resorte_t;

size_t resorte_obtener_id(const resorte_t *resorte);

float resorte_obtener_k(const resorte_t *resorte);

float resorte_obtener_lo(const resorte_t *resorte);

masa_t *resorte_obtener_m1(const resorte_t *resorte);

masa_t *resorte_obtener_m2(const resorte_t *resorte);

resorte_t* resorte_crear(size_t id, const masa_t* masa_inicial, const masa_t* masa_final);

bool en_radio(float etiqueta, float longitud); 

#endif
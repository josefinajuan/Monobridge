#ifndef RESORTES_H
#define RESORTES_H

#include "lista.h"
#include "masas.h"

typedef struct resorte resorte_t;

size_t resorte_obtener_id(const resorte_t *resorte);
float resorte_obtener_k(const resorte_t *resorte);
float resorte_obtener_lo(const resorte_t *resorte);
masa_t *resorte_obtener_m1(const resorte_t *resorte);
masa_t *resorte_obtener_m2(const resorte_t *resorte);

#endif
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

void resorte_actualizar_l(resorte_t *resorte, float l_inicial);

bool en_radio(float etiqueta, float longitud);

resorte_t* resorte_crear(size_t id,masa_t* masa_inicial,masa_t* masa_final);

void resorte_destruir(resorte_t* resorte);

void resorte_actualizar_id(resorte_t* resorte, int nuevo_id);

void resorte_actualizar_ids_resortes(lista_iter_t *iter, lista_t *lista_resortes);
#endif
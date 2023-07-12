#include "resortes.h"
#include "config.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct resorte
{
    size_t id;
    float k;
    float lo;
    struct masa *m1;
    struct masa *m2;
};

size_t resorte_obtener_id(const resorte_t *resorte)
{
    return resorte->id;
}

float resorte_obtener_k(const resorte_t *resorte)
{
    return resorte->k;
}

float resorte_obtener_lo(const resorte_t *resorte)
{
    return resorte->lo;
}

masa_t *resorte_obtener_m1(const resorte_t *resorte)
{
    return resorte->m1;
}

masa_t *resorte_obtener_m2(const resorte_t *resorte)
{
    return resorte->m2;
}

void resorte_actualizar_l(resorte_t *resorte, float l)
{
    resorte->lo = l;
}

resorte_t *resorte_crear(size_t id, masa_t *masa_inicial, masa_t *masa_final)
{
    if (masa_inicial == NULL || masa_final == NULL)
    {
        return NULL;
    }

    resorte_t *resorte = malloc(sizeof(resorte_t));
    if (resorte == NULL)
    {
        return NULL;
    }

    resorte->id = id;
    resorte->k = 0.0;
    resorte->lo = norma(masa_final, masa_inicial);
    resorte->m1 = masa_inicial;
    resorte->m2 = masa_final;

    return resorte;
}

void resorte_destruir(resorte_t *resorte){
    if (resorte != NULL)
    {
        free(resorte);
    }
}

void resorte_actualizar_id(resorte_t *resorte, int nuevo_id){
    if (resorte != NULL)
    {
        resorte->id = nuevo_id;
    }
}

void resorte_actualizar_ids_resortes(lista_iter_t *iter, lista_t *lista_resortes)
{
    size_t id_actual = 0;
    while (!lista_iter_al_final(iter))
    {
        resorte_t *resorte = lista_iter_ver_actual(iter);
        resorte->id = id_actual++;
        lista_iter_avanzar(iter);
    }
}
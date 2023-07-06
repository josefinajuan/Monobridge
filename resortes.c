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

void resorte_actualizar_l(resorte_t *resorte, float l_inicial)
{
    resorte->lo = l_inicial;
}

bool en_radio(float etiqueta, float longitud)
{
    return longitud < etiqueta;
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
    resorte->k = K_BASE;
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
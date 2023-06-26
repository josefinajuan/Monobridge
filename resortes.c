#include "resortes.h"


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct resorte{
    size_t id;
    float k;
    float lo;
    struct masa *m1;
    struct masa *m2;
};

size_t resorte_obtener_id(const resorte_t *resorte) {
    return resorte->id;
}

float resorte_obtener_k(const resorte_t *resorte) {
    return resorte->k;
}

float resorte_obtener_lo(const resorte_t *resorte) {
    return resorte->lo;
}

masa_t *resorte_obtener_m1(const resorte_t *resorte) {
    return resorte->m1;
}

masa_t *resorte_obtener_m2(const resorte_t *resorte) {
    return resorte->m2;
}

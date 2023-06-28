#include "masas.h"
#include "lista.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct masa{
    size_t id;
    float coordx, coordy, tam;
    bool es_fija;
};

struct masa* masa_crear(size_t id, float x, float y, float tam){
    struct masa* m = malloc(sizeof(struct masa));
    if (m == NULL) return NULL;

    m->id = id;
    m->coordx= x;
    m->coordy = y;
    m->tam = tam;
    m-> es_fija = false;

    return m;
}

float masa_obtener_coordy(const masa_t *masa) {
    return masa->coordy;
}

float masa_obtener_coordx(const masa_t *masa) {
    return masa->coordx;
}

size_t masa_obtener_id(const masa_t *masa) {
    return masa->id;
}

float masa_obtener_tam(const masa_t *masa) {
    return masa->tam;
}

void masa_actualizar_ids_masa(lista_iter_t *iter, lista_t *lista_masas) {
    size_t id_actual = 0;
    while (!lista_iter_al_final(iter)) {
        masa_t *masa = lista_iter_ver_actual(iter);
        masa->id = id_actual++;
        lista_iter_avanzar(iter);
    }
}

void masa_borrar(masa_t* masa) {
    free(masa);
}

bool coincidir_masas(const masa_t *masa, int coordenadas[2]) {
    return (masa->coordx == coordenadas[0] && masa->coordy == coordenadas[1]);
}

bool eliminar_masa_de_lista(masa_t* masa, lista_t* lista_masas) {
    lista_iter_t* iter = lista_iter_crear(lista_masas);
    if (iter == NULL) {
        return false;
    }

    while (!lista_iter_al_final(iter)) {
        masa_t* masa_actual = lista_iter_ver_actual(iter);
        if (masa_actual == masa) {
            lista_iter_borrar(iter);
            lista_iter_destruir(iter);
            return true;
        }
        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return false;
}

void masa_actualizar_coordx(masa_t* masa, float coordx) {
    masa->coordx = coordx;
}

void masa_actualizar_coordy(masa_t* masa, float coordy) {
    masa->coordy = coordy;
}

float norma(const masa_t* masa_final, const masa_t* masa_inicial) {
    float distancia_x = masa_final->coordx - masa_inicial->coordx;
    float distancia_y = masa_final->coordy - masa_inicial->coordy;
    return sqrtf(distancia_x * distancia_x + distancia_y * distancia_y);
}

bool es_fija(const masa_t * masa) {
    return masa -> es_fija;
}
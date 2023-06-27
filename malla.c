#include "malla.h"
#include "masas.h"
#include "resortes.h"
#include "lista.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MARGEN_ERROR 10

struct malla{
    lista_t* lista_resortes;
    lista_t* lista_masas;
};

malla_t *malla_crear() {
    malla_t *m = malloc(sizeof(malla_t));
    if (m == NULL) return NULL;

    m->lista_resortes = lista_crear();
    if (m->lista_resortes == NULL) {
        free(m);
        return NULL;
    }

    m->lista_masas = lista_crear();
    if (m->lista_masas == NULL) {
        lista_destruir(m->lista_resortes, NULL);
        free(m);
        return NULL;
    }

    return m;
}

void malla_destruir(malla_t *m) {
    if (m == NULL) return;

    lista_destruir(m->lista_resortes, free);
    lista_destruir(m->lista_masas, free);

    free(m);
}

size_t malla_cantidad_masas(const malla_t* malla) {
    return lista_largo(malla->lista_masas);
}

size_t malla_cantidad_resortes(const malla_t* malla) {
    return lista_largo(malla->lista_resortes);
}

bool estoy_sobre_masa(float coordx, float coordy, const malla_t *malla) {
    lista_iter_t *iter = lista_iter_crear(malla->lista_masas);
    if (iter == NULL) {
        // Manejar error de creación del iterador
        return false;
    }

    while (!lista_iter_al_final(iter)) {
        masa_t *masa = lista_iter_ver_actual(iter);
        float masa_coordx = masa_obtener_coordx(masa);
        float masa_coordy = masa_obtener_coordy(masa);

        if (coordx >= masa_coordx - MARGEN_ERROR && coordx <= masa_coordx + MARGEN_ERROR &&
            coordy >= masa_coordy - MARGEN_ERROR && coordy <= masa_coordy + MARGEN_ERROR) {
            lista_iter_destruir(iter);
            return true;
        }

        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return false;
}

bool malla_eliminar_masa_por_coordenadas(malla_t *malla, float coordx, float coordy) {
    lista_iter_t *iter = lista_iter_crear(malla->lista_masas);
    if (iter == NULL) {
        return false;
    }

    while (!lista_iter_al_final(iter)) {
        masa_t *masa = lista_iter_ver_actual(iter);
        float masa_coordx = masa_obtener_coordx(masa);
        float masa_coordy = masa_obtener_coordy(masa);

        if (coordx >= masa_coordx - MARGEN_ERROR && coordx <= masa_coordx + MARGEN_ERROR &&
            coordy >= masa_coordy - MARGEN_ERROR && coordy <= masa_coordy + MARGEN_ERROR) {
            lista_iter_borrar(iter);
            masa_actualizar_ids_masa(iter, malla->lista_masas);
            lista_iter_destruir(iter);
            return true;
        }

        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return false;
}

bool malla_agregar_masa(malla_t* malla, masa_t* masa) {
    return lista_insertar_ultimo(malla->lista_masas, masa);
}

bool malla_agregar_resorte(malla_t* malla, resorte_t* resorte) {
    return lista_insertar_ultimo(malla->lista_resortes, resorte);
}

void malla_actualizar_coord(masa_t* masa, float coordx, float coordy) {
    if (masa == NULL)
        return;

    masa_actualizar_coordx(masa, coordx);
    masa_actualizar_coordy(masa, coordy);
}

masa_t* obtener_masa(const malla_t* malla, float coordx, float coordy) {
    if (malla == NULL)
        return NULL;

    lista_t* lista_masas = malla->lista_masas;
    lista_iter_t* iter = lista_iter_crear(lista_masas);
    if (iter == NULL)
        return NULL;

    while (!lista_iter_al_final(iter)) {
        masa_t* masa = lista_iter_ver_actual(iter);
        if (masa_obtener_coordx(masa) == coordx && masa_obtener_coordy(masa) == coordy) {
            lista_iter_destruir(iter);
            return masa;
        }
        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return NULL;
}

size_t obtener_id_masa_en_coordenadas(float x, float y, const malla_t* malla) {
    lista_iter_t* iter = lista_iter_crear(malla->lista_masas);
    if (iter == NULL) {
        return -1; // Valor inválido para indicar que no se encontró la masa
    }

    size_t id_masa = 0;
    while (!lista_iter_al_final(iter)) {
        masa_t* masa = lista_iter_ver_actual(iter);
        float coordx = masa_obtener_coordx(masa);
        float coordy = masa_obtener_coordy(masa);

        if (x >= coordx - MARGEN_ERROR && x <= coordx + MARGEN_ERROR &&
            y >= coordy - MARGEN_ERROR && y <= coordy + MARGEN_ERROR) {
            lista_iter_destruir(iter);
            return id_masa;
        }

        lista_iter_avanzar(iter);
        id_masa++;
    }

    lista_iter_destruir(iter);
    return -1; // Valor inválido para indicar que no se encontró la masa
}
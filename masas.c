#include "masas.h"
#include "lista.h"
#include "config.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct masa
{
    size_t id;
    float coordx;
    float coordy;
    float tam;
    float masa;
    bool es_fija;
};

struct masa *masa_crear(size_t id, float x, float y, float tam)
{
    struct masa *m = malloc(sizeof(struct masa));
    if (m == NULL)
        return NULL;

    m->id = id;
    m->coordx = x;
    m->coordy = y;
    m->tam = tam;
    m->masa = 0.0;
    m->es_fija = false;

    return m;
}

struct masa* masa_crear_fija(size_t id, float x, float y, float tam){
    
    struct masa *m = malloc(sizeof(struct masa));
    if (m == NULL)
        return NULL;

    m->id = id;
    m->coordx = x;
    m->coordy = y;
    m->tam = tam;
    m->masa = 0.0;
    m->es_fija = true;

    return m;
}

float masa_obtener_coordy(const masa_t *masa)
{
    return masa->coordy;
}

float masa_obtener_coordx(const masa_t *masa)
{
    return masa->coordx;
}

size_t masa_obtener_id(const masa_t *masa)
{
    return masa->id;
}

float masa_obtener_tam(const masa_t *masa)
{
    return masa->tam;
}

void masa_actualizar_ids_masa(lista_iter_t *iter, lista_t *lista_masas)
{
    size_t id_actual = 0;
    while (!lista_iter_al_final(iter))
    {
        masa_t *masa = lista_iter_ver_actual(iter);
        masa->id = id_actual++;
        lista_iter_avanzar(iter);
    }
}

void masa_borrar(masa_t *masa)
{
    free(masa);
}

bool coincidir_masas(const masa_t *masa, float coordx, float coordy)
{
    return (coordx >= masa->coordx - MARGEN_ERROR && coordx <= masa->coordx + MARGEN_ERROR &&
            coordy >= masa->coordy - MARGEN_ERROR && coordy <= masa->coordy + MARGEN_ERROR);
}

bool eliminar_masa_de_lista(masa_t *masa, lista_t *lista_masas)
{
    lista_iter_t *iter = lista_iter_crear(lista_masas);
    if (iter == NULL)
    {
        return false;
    }

    while (!lista_iter_al_final(iter))
    {
        masa_t *masa_actual = lista_iter_ver_actual(iter);
        if (masa_actual == masa)
        {
            lista_iter_borrar(iter);
            lista_iter_destruir(iter);
            return true;
        }
        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return false;
}

void masa_actualizar_coordx(masa_t *masa, float coordx)
{
    masa->coordx = coordx;
}

void masa_actualizar_coordy(masa_t *masa, float coordy)
{
    masa->coordy = coordy;
}

float norma_puntos(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

float norma(const masa_t *masa_final, const masa_t *masa_inicial)
{
    float x1 = masa_inicial->coordx;
    float x2 = masa_final->coordx;
    float y1 = masa_inicial->coordy;
    float y2 = masa_final->coordy;
    return norma_puntos(x1, y1, x2, y2);
}

bool es_fija(const masa_t *masa)
{
    return masa->es_fija;
}
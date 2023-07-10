#ifndef MASAS_H
#define MASAS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

typedef struct masa masa_t;

struct masa* masa_crear(size_t id, float x, float y, float tam);

struct masa* masa_crear_fija(size_t id, float x, float y, float tam);

float masa_obtener_coordy(const masa_t *masa);

float masa_obtener_coordx(const masa_t *masa);

size_t masa_obtener_id(const masa_t *masa);

float masa_obtener_tam(const masa_t *masa);

bool coincidir_masas(const masa_t *masa, float coordx, float coordy);

void masa_actualizar_ids_masa(lista_iter_t *iter, lista_t *lista_masas);

void masa_borrar(masa_t* masa);

bool eliminar_masa_de_lista(masa_t* masa, lista_t* lista_masas);

void masa_actualizar_coordx(masa_t* masa, float coordx);

void masa_actualizar_coordy(masa_t* masa, float coordy);

float norma_puntos(float x1, float y1, float x2, float y2);

float norma(const masa_t* masa_final, const masa_t* masa_inicial);

bool es_fija(const masa_t * masa);


#endif
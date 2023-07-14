#ifndef MALLA_H
#define MALLA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lista.h"
#include "masas.h"
#include "resortes.h"

typedef struct malla malla_t;

float distancia_puntos(float x1, float y1, float x2, float y2);

float distancia_punto_segmento(float xp, float yp, float xj, float yj, float xk, float yk);

malla_t *malla_crear();

void malla_destruir(malla_t *m);

size_t malla_cantidad_masas(const malla_t *malla);

size_t malla_cantidad_resortes(const malla_t *malla);

bool estoy_sobre_masa(float coordx, float coordy, const malla_t *malla);

bool estoy_sobre_resorte(float x, float y, const malla_t* malla);

bool malla_eliminar_masa_por_coordenadas(malla_t *malla, float coordx, float coordy);

void malla_eliminar_resorte_por_coordenadas(malla_t* malla, float x, float y);

masa_t* malla_masa_nueva(malla_t* malla, size_t id_masa, float coordx, float coordy, float tam);

masa_t* malla_masa_nueva_fija(malla_t* malla, size_t id_masa, float coordx, float coordy, float tam);

resorte_t* malla_resorte_nuevo(malla_t* malla, size_t id_resorte, struct masa* masa1, struct masa* masa2) ;

void malla_resorte_actualizar_l0(resorte_t* resorte, float l_inicial);

bool malla_masas_en_radio(malla_t* malla, masa_t* masa, float l0);

bool malla_mover_masas(malla_t* malla, masa_t* masa, float coordx, float coordy, float l0_max);

masa_t *obtener_masa(const malla_t *malla, float coordx, float coordy);

bool son_iguales(int x1, int y1, int x2, int y2);

void malla_actualizar_coord(masa_t* masa, float coordx, float coordy);

lista_t* malla_obtener_lista_masas(malla_t* malla);

lista_t* malla_obtener_lista_resortes( malla_t* malla);

masa_t* malla_obtener_masa_por_id(malla_t* malla, size_t indice);

resorte_t* malla_obtener_resorte_por_id(malla_t* malla, size_t indice);

masa_t* malla_id_resorte_masa1(malla_t* malla, size_t id);

masa_t* malla_id_resorte_masa2(malla_t* malla, size_t id);

float malla_resorte_longitud_actual(malla_t* malla, size_t id);

float malla_masa_obtener_coordx(masa_t* masa);

float malla_masa_obtener_coordy(masa_t* masa);

float malla_masa_obtener_tam(masa_t* masa);

float malla_masa_obtener_masa(masa_t* masa);

void copiar_malla(malla_t* malla_origen, malla_t* malla_destino);

size_t malla_resorte_obtener_id_m1(const resorte_t* resorte);

size_t malla_resorte_obtener_id_m2(const resorte_t* resorte);

size_t malla_obtener_id_masa(const masa_t* masa);

size_t malla_resorte_obtener_id(const resorte_t* resorte);

float malla_resorte_obtener_lo(const resorte_t* resorte);

void reacomodar_id(struct malla *malla);

bool coincidir_masas(const masa_t *masa, float coordx, float coordy);

bool en_radio(float etiqueta, float longitud);

float calcular_longitud(struct masa* masa, float coordx, float coordy);

bool malla_masa_es_fija(masa_t*masa);

#endif
#ifndef SIMULACION_H
#define SIMULACION_H

#include "malla.h"
#include "lista.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct datos_masa datos_masa_t;
typedef struct datos_resorte datos_resorte_t;
typedef struct instante instante_t;
typedef struct simulacion simulacion_t;

float calcular_l_actual(float x1, float y1, float x2, float y2);

float calcular_coordy_m(struct instante* uno_anterior, int id_masa);

float calcular_coordx_m(struct instante* uno_anterior, int id_masa);

float calcular_coordy(float y_uno_anterior, float y_dos_anteriores, float l_nueva_uno_anterior, float l_inicial, float sumatoria);

float calcular_coordx(float x_uno_anterior, float x_dos_anteriores, float l_nueva_uno_anterior, float l_inicial, float sumatoria);

struct instante* calcular_nuevo_instante(struct instante* uno_anterior, struct instante* dos_anteriores, float* vector);

struct instante* crear_instante(malla_t* malla);

simulacion_t* _simulacion_crear();

simulacion_t* simulacion_crear(struct instante* un_anterior, struct instante* dos_anteriores, simulacion_t* simulacion);

void simulacion_agregar(struct instante* uno_anterior, struct instante* dos_anteriores, float* vector, simulacion_t* simulacion);

void destruir_simulacion(simulacion_t* simulacion);

void destruir_instante(struct instante* instante);

simulacion_t* simulacion_inicio(malla_t* malla);

void simulacion_a_malla(malla_t* malla_simulacion, simulacion_t* simulacion);

struct instante* simulacion_instante_uno_anterior(simulacion_t* simulacion);

struct instante* simulacion_instante_dos_anteriores(simulacion_t* simulacion);

void dibujar_masa_simulacion(const struct datos_masa *masa, SDL_Renderer *renderer);

void dibujar_resorte_simulacion(const struct datos_resorte *resorte, SDL_Renderer *renderer, const struct datos_masa *masas);

void dibujar_instante_simulacion(const struct instante *instante, SDL_Renderer *renderer);

void dibujar_simulacion(struct simulacion *simulacion, SDL_Renderer *renderer);



#endif
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


// A partir de las coordenadas x e y de dos masas calcular longitud resorte
float calcular_l_actual(float x1, float y1, float x2, float y2);

float calcular_by(float m, float y_uno_anterior, float y_dos_anteriores, float sumatoria);

float calcular_bx(float m, float x_uno_anterior, float x_dos_anteriores, float sumatoria);

float calcular_aj(float m);

float calcular_nueva_coordenada_x(instante_t* uno_anterior, instante_t* dos_anteriores,simulacion_t* simulacion, size_t id_masa, float m);

float calcular_nueva_coordenada_y(instante_t* uno_anterior, instante_t* dos_anteriores, simulacion_t* simulacion, size_t id_masa, float m);

//Recibe dos instantes
//Calcula y devuelve nuevo instante  
instante_t* calcular_instante_nuevo(instante_t* uno_anterior, instante_t* dos_anteriores, simulacion_t* simulacion);

struct instante* copiar_instante(const struct instante* original);

//Crea simulacion vacia
simulacion_t* _simulacion_crear();

//Recibe un instante uno_anterior y dos_anteriores, una simulacion
//Se crea una simulacion y se agregan los instantes a la estructura
simulacion_t* simulacion_crear(struct instante* uno_anterior, struct instante* dos_anteriores, struct simulacion* simulacion);

void destruir_instante(struct instante* instante);

void destruir_simulacion(simulacion_t* simulacion);

//Devuelve un puntero a un primer instante creado a partir de una malla 
struct instante* crear_instante_desde_malla(malla_t* malla);

// Recibe una malla, se crea el primer instante y el segundo y se devuelve una simulacion creada con los instantes adentro
simulacion_t* simulacion_inicio(malla_t* malla);

void simulacion_agregar(simulacion_t* simulacion, malla_t* malla_simulacion);

void simulacion_a_malla(malla_t* malla_simulacion, simulacion_t* simulacion);

struct instante* simulacion_instante_uno_anterior(simulacion_t* simulacion);

struct instante* simulacion_instante_dos_anteriores(simulacion_t* simulacion);

void dibujar_masa_simulacion(const struct datos_masa* masa, SDL_Renderer *renderer) ;

void dibujar_resorte_simulacion(const struct datos_resorte* resorte, SDL_Renderer *renderer, const struct datos_masa *masas);

void dibujar_instante_simulacion(const struct instante *instante, SDL_Renderer *renderer);

void simular(struct simulacion *simulacion, SDL_Renderer *renderer);

bool cumplen_estiramiento(simulacion_t* simulacion, float porcentaje_maximo);

#endif
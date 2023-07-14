#ifndef DIBUJO_H
#define DIBUJO_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

#include "malla.h"
#include "lista.h"

void dibujar_masa(masa_t *masa, SDL_Renderer *renderer);

void dibujar_resorte(resorte_t *resorte, SDL_Renderer *renderer);

void dibujar_lista_masas(malla_t *malla, SDL_Renderer *renderer);

void dibujar_lista_resortes( malla_t *malla, SDL_Renderer *renderer);

void mostrar_malla(malla_t *malla, SDL_Renderer *renderer);

void moviendo_radio_resorte(const masa_t *masa, float x, float y, SDL_Renderer *renderer);

#endif
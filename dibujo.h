#ifndef DIBUJO_H
#define DIBUJO_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "malla.h"
#include "masas.h"
#include "resortes.h"
#include "lista.h"

static void dibujar_masa(const masa_t *masa, SDL_Renderer *renderer);

static void dibujar_resorte(const resorte_t *resorte, SDL_Renderer *renderer);

static void dibujar_lista_masas(const malla_t *malla, SDL_Renderer *renderer);

static void dibujar_lista_resortes(malla_t *malla, SDL_Renderer *renderer);

void renderizar_malla(malla_t *malla, SDL_Renderer *renderer);

void moviendo_radio_resorte(const masa_t *masa, float x, float y, SDL_Renderer *renderer);

#endif
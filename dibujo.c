#include "masas.h"
#include "lista.h"
#include "resortes.h"
#include "malla.h"
#include <SDL2/SDL.h>
#include <math.h>

void dibujar_masa(masa_t *masa, SDL_Renderer *renderer) {
    SDL_Rect r1 = {((masa_obtener_coordx(masa)) - (masa_obtener_tam(masa)) / 2), ((masa_obtener_coordy(masa)) - (masa_obtener_tam(masa)) / 2), (masa_obtener_tam(masa)), (masa_obtener_tam(masa))};
    SDL_RenderDrawRect(renderer, &r1);
}

void dibujar_resorte(resorte_t* resorte, SDL_Renderer* renderer) {
    if (resorte == NULL || renderer == NULL) {
        return;
    }

    masa_t* masa_inicial = resorte_obtener_m1(resorte);
    masa_t* masa_final = resorte_obtener_m2(resorte);

    float x1 = masa_obtener_coordx(masa_inicial);
    float y1 = masa_obtener_coordy(masa_inicial);
    float x2 = masa_obtener_coordx(masa_final);
    float y2 = masa_obtener_coordy(masa_final);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderDrawLine(renderer, (int)x1, (int)y1, (int)x2, (int)y2);
}

void dibujar_lista_masas(malla_t *malla, SDL_Renderer *renderer) {
    lista_iter_t *l_iter = lista_iter_crear(malla_obtener_lista_masas(malla));
    while (!lista_iter_al_final(l_iter)) {
        masa_t *masa = lista_iter_ver_actual(l_iter);
        dibujar_masa(masa, renderer);
        lista_iter_avanzar(l_iter);
    }
    lista_iter_destruir(l_iter);
}

void dibujar_lista_resortes(malla_t *malla, SDL_Renderer *renderer) {
    lista_iter_t *l_iter = lista_iter_crear(malla_obtener_lista_resortes(malla));
    while (!lista_iter_al_final(l_iter)) {
        resorte_t* resorte = lista_iter_ver_actual(l_iter);
        dibujar_resorte(resorte, renderer);
        lista_iter_avanzar(l_iter);
    }
    lista_iter_destruir(l_iter);
}

void renderizar_malla(malla_t *malla, SDL_Renderer *renderer) {
    dibujar_lista_masas(malla, renderer);
    dibujar_lista_resortes(malla, renderer);
}

void moviendo_radio_resorte(const masa_t *masa, float x, float y, SDL_Renderer *renderer) {
    if (masa == NULL || renderer == NULL) {
        return;
    }

    float x1 = masa_obtener_coordx(masa);
    float y1 = masa_obtener_coordy(masa);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

    SDL_RenderDrawLine(renderer, (int)x1, (int)y1, (int)x, (int)y);
}

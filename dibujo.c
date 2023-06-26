
#include "masas.h"
#include "lista.h"
#include "resortes.h"
#include "malla.h"
#include <SDL2/SDL.h>

/*struct masa{
    size_t id;
    float coordx, coordy, tam;
    bool es_fija;
};*/

struct malla{
    lista_t* lista_resortes;
    lista_t* lista_masas;
};

/*struct resorte{
    size_t id;
    float k;
    float lo;
    struct masa *m1;
    struct masa *m2;
};
*/
static void dibujar_masa(const masa_t *masa, SDL_Renderer *renderer) {
    SDL_Rect r1 = {((masa_obtener_coordx(masa)) - (masa_obtener_tam(masa)) / 2), ((masa_obtener_coordy(masa)) - (masa_obtener_tam(masa)) / 2), (masa_obtener_tam(masa)), (masa_obtener_tam(masa))};
    SDL_RenderDrawRect(renderer, &r1);
}

static void dibujar_resorte(const resorte_t *resorte, SDL_Renderer *renderer) {
    masa_t *masa1 = (resorte_obtener_m1(resorte));
    masa_t *masa2 = (resorte_obtener_m2(resorte));
    SDL_RenderDrawLine(renderer, (masa_obtener_coordx(masa1)), (masa_obtener_coordy(masa1)), (masa_obtener_coordx(masa2)), (masa_obtener_coordy(masa2)));
}

static void dibujar_lista_masas(const malla_t *malla, SDL_Renderer *renderer) {
    lista_iter_t *l_iter = lista_iter_crear(malla->lista_masas);
    while (!lista_iter_al_final(l_iter)) {
        masa_t *masa = lista_iter_ver_actual(l_iter);
        dibujar_masa(masa, renderer);
        lista_iter_avanzar(l_iter);
    }
    lista_iter_destruir(l_iter);
}

static void dibujar_lista_resortes(malla_t *malla, SDL_Renderer *renderer) {
    lista_iter_t *l_iter = lista_iter_crear(malla->lista_resortes);
    while (!lista_iter_al_final(l_iter)) {
        resorte_t *resorte = lista_iter_ver_actual(l_iter);
        dibujar_resorte(resorte, renderer);
        lista_iter_avanzar(l_iter);
    }
    lista_iter_destruir(l_iter);
}

void renderizar_malla(malla_t *malla, SDL_Renderer *renderer) {
    dibujar_lista_masas(malla, renderer);
    dibujar_lista_resortes(malla, renderer);
}

void moviendo_radio_resorte(const masa_t *masa, float x, float y, SDL_Renderer *renderer){
    SDL_RenderDrawLine (renderer, (masa_obtener_coordx(masa)), (masa_obtener_coordy(masa)), x , y );
}
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "config.h"
#include "lista.h"
#include "resortes.h"
#include "masas.h"
#include "malla.h"
#include "dibujo.h"

#define MARGEN_ERROR 10



#ifdef TTF
#include <SDL2/SDL_ttf.h>

void escribir_texto(SDL_Renderer *renderer, TTF_Font *font, const char *s, int x, int y) {
    SDL_Color color = {255, 255, 255};  // Estaría bueno si la función recibe un enumerativo con el color, ¿no?
    SDL_Surface *surface = TTF_RenderText_Solid(font, s, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
#endif

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

#ifdef TTF
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("FreeSansBold.ttf", 24);
    if(font == NULL) {
        fprintf(stderr, "No pudo cargarse la tipografía\n");
        return 1;
    }
#endif

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Mono Bridge");

    int dormir = 0;

    // BEGIN código del alumno

    //bool estoy_dibujando = false;
    int coordx = 0, coordy = 0;
    int iniciox, inicioy;
    int x, y;

    //lista_t* lista_masas = lista_crear();
    //lista_t* lista_resortes = lista_crear();
    malla_t* malla = malla_crear();
    size_t id__;
    size_t id_masa;
    struct masa* masa_nueva;

    bool estoy_dibujando = false;
    bool crear_masa = false;
    bool moviendo_masa = false;

    // END código del alumno

    unsigned int ticks = SDL_GetTicks();
    while(1) {
        if(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                break;

            // BEGIN código del alumno
            id_masa = malla_cantidad_resortes(malla);

            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                estoy_dibujando = true;
                iniciox = event.motion.x;
                inicioy = event.motion.y;

                if(!estoy_sobre_masa(iniciox, inicioy, malla)){
                    id_masa = malla_cantidad_resortes(malla);
                    masa_nueva = masa_crear(id_masa, iniciox, inicioy, 10);
                    crear_masa = true;
                }

                if (estoy_sobre_masa(iniciox, inicioy, malla)){
                    moviendo_masa = true;
                    id__ = obtener_id_masa_en_coordenadas(iniciox, inicioy, malla);
                }
            }

            else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT){
                iniciox = event.motion.x;
                inicioy = event.motion.y;
                
                if(estoy_sobre_masa(iniciox, inicioy, malla)){
                    x = iniciox;
                    y = inicioy;
                }
            }

            else if(event.type == SDL_MOUSEMOTION) {
                coordx = event.motion.x;
                coordy = event.motion.y;
            }

            else if(event.type == SDL_MOUSEMOTION && moviendo_masa){
                malla_actualizar_coordx(malla, &id__, event.motion.x);
                malla_actualizar_coordy(malla, &id__, event.motion.y);
            }
            else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && moviendo_masa) {
                malla_actualizar_coordx(malla, &id__, event.motion.x);
                malla_actualizar_coordy(malla, &id__, event.motion.y);
                moviendo_masa = false;
            }    
            else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                if(masa_nueva!= NULL && crear_masa){
                    if (iniciox == coordx && inicioy == coordy){
                        malla_agregar_masa(malla,masa_nueva);
                        id_masa = malla_cantidad_resortes(malla);
                        masa_nueva = NULL;
                    }
                    crear_masa = false;
                    masa_nueva = NULL;
                }    
            }

            else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT){ 
                if(x >= coordx - MARGEN_ERROR && x <= coordx + MARGEN_ERROR &&
                y >= coordy - MARGEN_ERROR && y <= coordy + MARGEN_ERROR){
                    if(estoy_sobre_masa(x, y, malla)){
                        malla_eliminar_masa_por_coordenadas(malla, x, y);
                        id_masa = malla_cantidad_masas(malla);
                    }
                    x = 0;
                    y = 0;
                }
            }
            // END código del alumno

            continue;
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0x91, 0x0D, 0xF4, 0x00);

        // BEGIN código del alumno
#ifdef TTF
        escribir_texto(renderer, font, "Mono Bridge", 100, 20);
        char aux[100];
        sprintf(aux, "%03d, %03d", coordx, coordy);
        escribir_texto(renderer, font, aux, VENTANA_ANCHO - 100, VENTANA_ALTO - 34);
#endif
        renderizar_malla(malla, renderer);
        // END código del alumno

        SDL_RenderPresent(renderer);
        ticks = SDL_GetTicks() - ticks;
        if(dormir) {
            SDL_Delay(dormir);
            dormir = 0;
        }
        else if(ticks < 1000 / JUEGO_FPS)
            SDL_Delay(1000 / JUEGO_FPS - ticks);
        ticks = SDL_GetTicks();
    }

    // BEGIN código del alumno
    // END código del alumno

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

#ifdef TTF
    TTF_CloseFont(font);
    TTF_Quit();
#endif
    SDL_Quit();
    return 0;
}

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "config.h"
#include "dibujo.h"
#include "lista.h"
#include "resortes.h"
#include "masas.h"
#include "malla.h"
#include "simulacion.h"

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

    float tiempo = 0;
    int coordx = 0, coordy = 0;
    int iniciox, inicioy;

    malla_t* malla = malla_crear();
    malla_t* malla_simulacion = malla_crear();
    //instante_t* instante_un_anterior;
    //instante_t* instante_dos_anteriores;
    simulacion_t* simulacion;

    size_t id_masa;
    size_t id_resorte;
    masa_t* masa_inicial_dibujando;
    masa_t* masa_final;
    masa_t* masa_nueva;
    masa_t* masa_moviendose;
    resorte_t* resorte;


    bool simulando = false;
    bool dibujando = false;
    bool moviendo_masa = false;
    bool _en_radio = false; 

    // END código del alumno

    unsigned int ticks = SDL_GetTicks();
    while(1) {
        if(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                break;

            // BEGIN código del alumno

            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                iniciox = event.motion.x;
                inicioy = event.motion.y;

                if(!simulando){
                    masa_moviendose = obtener_masa(malla, iniciox/FACTOR_ESCALA, inicioy/FACTOR_ESCALA);
                    if(masa_moviendose != NULL){
                        moviendo_masa = true;
                    }
                }
            }
               
            else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT){
                iniciox = event.motion.x;
                inicioy = event.motion.y;
            }

            else if(event.type == SDL_MOUSEMOTION) {
                coordx = event.motion.x;
                coordy = event.motion.y;

                if(moviendo_masa){
                    malla_mover_masas(malla, masa_moviendose, coordx/FACTOR_ESCALA, coordy/FACTOR_ESCALA, L0_MAX/FACTOR_ESCALA);
                }

                if(dibujando){
                    float longitud = norma_puntos(masa_obtener_coordx(masa_inicial_dibujando), masa_obtener_coordy(masa_inicial_dibujando),coordx/FACTOR_ESCALA, coordy/FACTOR_ESCALA);
                    if(en_radio(L0_MAX/FACTOR_ESCALA, longitud)){
                        _en_radio = true;
                    }else{
                        _en_radio = false;
                    }
                }
            }

            else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                moviendo_masa = false;
                if(!simulando){
                    if(iniciox == coordx && inicioy == coordy){
                        if(dibujando){
                            float longitud = norma_puntos(masa_obtener_coordx(masa_inicial_dibujando), masa_obtener_coordy(masa_inicial_dibujando), coordx/FACTOR_ESCALA, coordy/FACTOR_ESCALA);
                            if(en_radio(L0_MAX/FACTOR_ESCALA, longitud)) {
                                if(estoy_sobre_masa(coordx/FACTOR_ESCALA, coordy/FACTOR_ESCALA, malla)){
                                    if(coincidir_masas(masa_inicial_dibujando, coordx/FACTOR_ESCALA, coordy/FACTOR_ESCALA)){
                                        dibujando = false;
                                    }else{
                                        masa_final = obtener_masa(malla, coordx/FACTOR_ESCALA, coordy/FACTOR_ESCALA);
                                        id_resorte = malla_cantidad_resortes(malla);
                                        resorte = resorte_crear(id_resorte, masa_inicial_dibujando, masa_final);
                                        if(resorte!= NULL){
                                            malla_agregar_resorte(malla, resorte);
                                        }
                                        dibujando = false;
                                    }
                                }
                                else if(!estoy_sobre_masa(coordx/FACTOR_ESCALA,coordy/FACTOR_ESCALA, malla)){
                                    id_masa = malla_cantidad_masas(malla);
                                    masa_final = masa_crear(id_masa, coordx/FACTOR_ESCALA, coordy/FACTOR_ESCALA, TAM_MASA/FACTOR_ESCALA);
                                    if(masa_final!= NULL){
                                        malla_agregar_masa(malla, masa_final);
                                    }
                                    id_resorte = malla_cantidad_resortes(malla);
                                    resorte = resorte_crear(id_resorte, masa_inicial_dibujando, masa_final);
                                    if(resorte!= NULL){
                                        malla_agregar_resorte(malla, resorte);
                                    }
                                    dibujando = false;
                                }
                            }
                        
                        }else {
                            if(estoy_sobre_masa(coordx/FACTOR_ESCALA, coordy/FACTOR_ESCALA, malla)){
                                masa_inicial_dibujando = obtener_masa(malla, coordx/FACTOR_ESCALA, coordy/FACTOR_ESCALA);
                                dibujando = true;
                            }
                            if(!estoy_sobre_masa(coordx/FACTOR_ESCALA, coordy/FACTOR_ESCALA, malla)){
                                id_masa = malla_cantidad_masas(malla);
                                masa_nueva = masa_crear(id_masa, coordx/FACTOR_ESCALA, coordy/FACTOR_ESCALA, TAM_MASA/FACTOR_ESCALA);
                                if(masa_nueva != NULL){
                                    malla_agregar_masa(malla,masa_nueva);
                                }
                                masa_nueva = NULL;
                            }
                        }
                    }
                }
            }

            else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT){ 

                if(iniciox == coordx && inicioy == coordy){
                    if(!simulando && !dibujando){
                        if(estoy_sobre_masa(iniciox/FACTOR_ESCALA, inicioy/FACTOR_ESCALA, malla)){
                            malla_eliminar_masa_por_coordenadas(malla, iniciox/FACTOR_ESCALA, inicioy/FACTOR_ESCALA);
                            id_masa = malla_cantidad_masas(malla);
                            id_resorte = malla_cantidad_resortes(malla);
                        } 
                        else if(estoy_sobre_resorte(iniciox/FACTOR_ESCALA, inicioy/FACTOR_ESCALA, malla)){
                            malla_eliminar_resorte_por_coordenadas(malla, iniciox/FACTOR_ESCALA, inicioy/FACTOR_ESCALA);
                        }
                        else{
                            simulando = true;
                            reacomodar_id(malla);
                            copiar_malla(malla, malla_simulacion);
                            simulacion = simulacion_inicio(malla_simulacion);
                        }
                    }
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
        if(!simulando){
            escribir_texto(renderer, font, "Mono Bridge", 100, 20);
            char aux[100];
            sprintf(aux, "%03d, %03d", coordx, coordy);
            escribir_texto(renderer, font, aux, VENTANA_ANCHO - 100, VENTANA_ALTO - 34);
        }
        
        if(simulando && tiempo <= DURACION_SIMULACION){
            escribir_texto(renderer, font, "Simulando", 200, 20);
            char aux[100];
            sprintf(aux, "%03d, %03d", coordx, coordy);
            escribir_texto(renderer, font, aux, VENTANA_ANCHO - 100, VENTANA_ALTO - 34);
            char t[100];
            sprintf(t, "%03f", tiempo);
            escribir_texto(renderer, font, t, VENTANA_ANCHO - 200, 20);
        }

#endif  
        if (!simulando){
            if(dibujando && _en_radio){
                moviendo_radio_resorte(masa_inicial_dibujando, coordx/FACTOR_ESCALA, coordy/FACTOR_ESCALA, renderer);
            }
            mostrar_malla(malla, renderer);
        }
        else if(simulando && tiempo <= DURACION_SIMULACION){
            size_t veces =  1.0 / JUEGO_FPS / DT;
            for(size_t i = 0; i < veces; i++) {
                simulacion_agregar(simulacion, malla_simulacion);
                simular(simulacion,renderer);
                /*
                simulacion_a_malla(malla_simulacion, simulacion);
                lista_t* lista = malla_obtener_lista_masas(malla_simulacion);
                lista_iter_t* lista_iter = lista_iter_crear(lista);
                printf("%f ; %f \n",malla_masa_obtener_coordx(lista_iter_ver_actual(lista_iter)),malla_masa_obtener_coordy(lista_iter_ver_actual(lista_iter)));
                */
                tiempo += DT;
            }
        }

        if(simulando && tiempo > DURACION_SIMULACION){
            simulacion_a_malla(malla_simulacion, simulacion);
            tiempo = 0;
            malla_destruir(malla_simulacion);
            simulando = false;
        }
        //END código del alumno

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

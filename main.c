#include <SDL2/SDL.h>
#include <stdbool.h>

#include "config.h"
#include "dibujo.h"
#include "juego.h"
#include "simulacion.h"
#include "lista.h"
#include "resortes.h"
#include "masas.h"
#include "malla.h"

#ifdef TTF
#include <SDL2/SDL_ttf.h>

void escribir_texto(SDL_Renderer *renderer, TTF_Font *font, const char *s, int x, int y)
{
    SDL_Color color = {255, 255, 255}; // Estaría bueno si la función recibe un enumerativo con el color, ¿no?
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

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

#ifdef TTF
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("FreeSansBold.ttf", 24);
    if (font == NULL)
    {
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

    malla_t *malla_construccion;
    malla_t *malla_simulacion;
    simulacion_t *simulacion;

    size_t id_masa;
    size_t id_resorte;
    masa_t *masa_inicial_dibujando;
    masa_t *masa_final;
    masa_t *masa_nueva;
    masa_t *masa_borrar;
    masa_t *masa_moviendose;

    int nivel = 1;
    int puntos = 0;
    char niveles[] = "nivel_#.bin";
    bool construyendo = true;
    bool simulando = false;
    bool dibujando = false;
    bool moviendo_masa = false;
    bool _en_radio = false;

    if (argc == 2)
    {
        malla_simulacion = malla_crear();
        malla_construccion = malla_crear();
        FILE *f_entrada = fopen(argv[1], "rb");
        if (f_entrada == NULL)
        {
            malla_destruir(malla_simulacion);
            malla_destruir(malla_construccion);
            return 1;
        }

        if (!nivel_abrir(f_entrada, malla_simulacion))
        {
            fclose(f_entrada);
            malla_destruir(malla_simulacion);
            malla_destruir(malla_construccion);
            return 1;
        }
        fclose(f_entrada);
        copiar_malla(malla_construccion, malla_simulacion);
        simulacion = simulacion_inicio(malla_simulacion);

        if (simulacion == NULL)
        {
            malla_destruir(malla_simulacion);
            malla_destruir(malla_construccion);
            return 1;
        }

        construyendo = false;
        simulando = true;
    }

    // END código del alumno

    unsigned int ticks = SDL_GetTicks();
    while (1)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                break;

            // BEGIN código del alumno
            if (construyendo)
            {
                malla_construccion = malla_crear();
                nivel_inicio(malla_construccion, nivel);
                construyendo = false;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                iniciox = event.motion.x;
                inicioy = event.motion.y;

                if (!simulando)
                {
                    masa_moviendose = obtener_masa(malla_construccion, iniciox / FACTOR_ESCALA, inicioy / FACTOR_ESCALA);
                    if (masa_moviendose != NULL && !malla_masa_es_fija(masa_moviendose))
                    {
                        moviendo_masa = true;
                    }
                }
            }

            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT)
            {
                iniciox = event.motion.x;
                inicioy = event.motion.y;
            }

            else if (event.type == SDL_MOUSEMOTION)
            {
                coordx = event.motion.x;
                coordy = event.motion.y;

                if (moviendo_masa)
                {
                    malla_mover_masas(malla_construccion, masa_moviendose, coordx / FACTOR_ESCALA, coordy / FACTOR_ESCALA, L0_MAX / FACTOR_ESCALA);
                }

                if (dibujando)
                {
                    float longitud = calcular_longitud(masa_inicial_dibujando, coordx / FACTOR_ESCALA, coordy / FACTOR_ESCALA);
                    if (en_radio(L0_MAX / FACTOR_ESCALA, longitud))
                    {
                        _en_radio = true;
                    }
                    else
                    {
                        _en_radio = false;
                    }
                }
            }

            else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
            {
                moviendo_masa = false;
                if (!simulando)
                {
                    if (iniciox == coordx && inicioy == coordy)
                    {
                        if (dibujando)
                        {
                            float longitud = calcular_longitud(masa_inicial_dibujando, coordx / FACTOR_ESCALA, coordy / FACTOR_ESCALA);
                            if (en_radio(L0_MAX / FACTOR_ESCALA, longitud))
                            {
                                if (estoy_sobre_masa(coordx / FACTOR_ESCALA, coordy / FACTOR_ESCALA, malla_construccion))
                                {
                                    if (coincidir_masas(masa_inicial_dibujando, coordx / FACTOR_ESCALA, coordy / FACTOR_ESCALA))
                                    {
                                        dibujando = false;
                                    }
                                    else
                                    {
                                        masa_final = obtener_masa(malla_construccion, coordx / FACTOR_ESCALA, coordy / FACTOR_ESCALA);
                                        id_resorte = malla_cantidad_resortes(malla_construccion);
                                        malla_resorte_nuevo(malla_construccion, id_resorte, masa_inicial_dibujando, masa_final);
                                        dibujando = false;
                                    }
                                }
                                else if (!estoy_sobre_masa(coordx / FACTOR_ESCALA, coordy / FACTOR_ESCALA, malla_construccion))
                                {
                                    id_masa = malla_cantidad_masas(malla_construccion);
                                    masa_nueva = malla_masa_nueva(malla_construccion, id_masa, coordx / FACTOR_ESCALA, coordy / FACTOR_ESCALA, TAM_MASA / FACTOR_ESCALA);
                                    id_resorte = malla_cantidad_resortes(malla_construccion);
                                    malla_resorte_nuevo(malla_construccion, id_resorte, masa_inicial_dibujando, masa_nueva);
                                    dibujando = false;
                                }
                            }
                        }
                        else
                        {
                            if (estoy_sobre_masa(coordx / FACTOR_ESCALA, coordy / FACTOR_ESCALA, malla_construccion))
                            {
                                masa_inicial_dibujando = obtener_masa(malla_construccion, coordx / FACTOR_ESCALA, coordy / FACTOR_ESCALA);
                                dibujando = true;
                            }
                            if (!estoy_sobre_masa(coordx / FACTOR_ESCALA, coordy / FACTOR_ESCALA, malla_construccion))
                            {
                                id_masa = malla_cantidad_masas(malla_construccion);
                                malla_masa_nueva(malla_construccion, id_masa, coordx / FACTOR_ESCALA, coordy / FACTOR_ESCALA, TAM_MASA / FACTOR_ESCALA);
                            }
                        }
                    }
                }
            }

            else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT)
            {

                if (iniciox == coordx && inicioy == coordy)
                {
                    if (!simulando && !dibujando)
                    {
                        if (estoy_sobre_masa(iniciox / FACTOR_ESCALA, inicioy / FACTOR_ESCALA, malla_construccion))
                        {
                            masa_borrar = obtener_masa(malla_construccion, iniciox / FACTOR_ESCALA, inicioy / FACTOR_ESCALA);
                            if (!malla_masa_es_fija(masa_borrar))
                            {
                                malla_eliminar_masa_por_coordenadas(malla_construccion, iniciox / FACTOR_ESCALA, inicioy / FACTOR_ESCALA);
                                id_masa = malla_cantidad_masas(malla_construccion);
                                id_resorte = malla_cantidad_resortes(malla_construccion);
                            }
                        }
                        else if (estoy_sobre_resorte(iniciox / FACTOR_ESCALA, inicioy / FACTOR_ESCALA, malla_construccion))
                        {
                            malla_eliminar_resorte_por_coordenadas(malla_construccion, iniciox / FACTOR_ESCALA, inicioy / FACTOR_ESCALA);
                        }
                        else
                        {
                            malla_simulacion = malla_crear();
                            reacomodar_id(malla_construccion);
                            copiar_malla(malla_construccion, malla_simulacion);
                            simulacion = simulacion_inicio(malla_simulacion);
                            if (simulacion == NULL){
                                malla_destruir(malla_simulacion);
                                return 1;
                            }
                            simulando = true;
                        }
                    }
                }
                else{
                    destruir_simulacion(simulacion);
                    malla_destruir(malla_simulacion);
                    simulando = false;
                    tiempo = 0;
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
        if (!simulando)
        {
            escribir_texto(renderer, font, "Mono Bridge", 20, 20);
            char n[100];
            sprintf(n, "Nivel: %d", nivel);
            escribir_texto(renderer, font, n, 20, VENTANA_ALTO - 40);
            char p[100];
            sprintf(p, "Puntos: %d", puntos);
            escribir_texto(renderer, font, p, VENTANA_ANCHO - 200, VENTANA_ALTO - 40);
        }

        if (simulando && tiempo <= DURACION_SIMULACION)
        {
            escribir_texto(renderer, font, "Simulando", 20, 20);
            char t[100];
            sprintf(t, "%03f", tiempo);
            escribir_texto(renderer, font, t, VENTANA_ANCHO - 120, 25);
            char n[100];
            sprintf(n, "Nivel: %d", nivel);
            escribir_texto(renderer, font, n, 20, VENTANA_ALTO - 40);
            char p[100];
            sprintf(p, "Puntos: %d", puntos);
            escribir_texto(renderer, font, p, VENTANA_ANCHO - 200, VENTANA_ALTO - 40);
        }

#endif
        if (!simulando)
        {
            if (dibujando && _en_radio)
            {
                moviendo_radio_resorte(masa_inicial_dibujando, coordx / FACTOR_ESCALA, coordy / FACTOR_ESCALA, renderer);
            }
            mostrar_malla(malla_construccion, renderer);
        }
        else if (simulando && tiempo <= DURACION_SIMULACION)
        {
            size_t veces = 1.0 / JUEGO_FPS / DT;
            for (size_t i = 0; i < veces; i++)
            {
                simulacion_agregar(simulacion);
                simular(simulacion, renderer);
                tiempo += DT;
            }
        }

        if (tiempo > DURACION_SIMULACION){
            simulacion_a_malla(malla_simulacion, simulacion);
            tiempo = 0;
            if(cumplen_estiramiento(simulacion, MAXIMO_ESTIRAMIENTO)){
                niveles[6] = nivel + '0';
                FILE *f_salida = fopen(niveles, "wb");
                if (f_salida == NULL){
                    destruir_simulacion(simulacion);
                    malla_destruir(malla_simulacion);
                    malla_destruir(malla_construccion);
                    return 1;
                }
                  
                if (!nivel_guardar(f_salida, malla_construccion))
                {
                    fclose(f_salida);
                    destruir_simulacion(simulacion);
                    malla_destruir(malla_simulacion);
                    malla_destruir(malla_construccion); 
                    return 1;
                }
                
                puntos += puntos_ganar(malla_construccion, nivel);
                malla_destruir(malla_construccion);
                
                if (fclose(f_salida) == EOF){
                    destruir_simulacion(simulacion);
                    malla_destruir(malla_simulacion);
                    return 1;
                }
                nivel++;
            }
            construyendo = true;
            destruir_simulacion(simulacion);
            simulando = false;
            malla_destruir(malla_simulacion);
            printf("ok\n");
        }

        // END código del alumno

        SDL_RenderPresent(renderer);
        ticks = SDL_GetTicks() - ticks;
        if (dormir)
        {
            SDL_Delay(dormir);
            dormir = 0;
        }
        else if (ticks < 1000 / JUEGO_FPS)
            SDL_Delay(1000 / JUEGO_FPS - ticks);
        ticks = SDL_GetTicks();
    }

    // BEGIN código del alumno
    destruir_simulacion(simulacion);
    malla_destruir(malla_simulacion);
    malla_destruir(malla_construccion);

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

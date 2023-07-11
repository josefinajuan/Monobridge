#include "simulacion.h"

#include "lista.h"
#include "malla.h"
#include "config.h"

struct datos_masa
{
    float x;
    float y;
};

struct datos_resorte
{
    size_t id_masa1;
    size_t id_masa2;
    float l_nueva;
    float l_inicial;
};

struct instante
{
    struct datos_masa *datos_masas;
    struct datos_resorte *datos_resortes;
    size_t cant_masas;
    size_t cant_resortes;
};

struct simulacion
{   
    lista_t* instantes;
};


float calcular_l_actual(float x1, float y1, float x2, float y2) {
    float distancia = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return distancia;
}

float calcular_by(float m, float y_uno_anterior, float y_dos_anteriores, float sumatoria){
    float bj = (m /(DT * DT)) * ((2 * y_uno_anterior )- y_dos_anteriores) + (B/DT) * y_uno_anterior + m * G + sumatoria;
    return bj;

}

float calcular_bx(float m, float x_uno_anterior, float x_dos_anteriores, float sumatoria){
    float bj = (m /(DT * DT)) * ((2 * x_uno_anterior )- x_dos_anteriores) + (B/DT) * x_uno_anterior + sumatoria;
    return bj;

}

float calcular_aj(float m){
    float aj = m /(DT * DT) + (B/DT);
    return aj;
}

float calcular_nueva_coordenada_x(instante_t* uno_anterior, instante_t* dos_anteriores,simulacion_t* simulacion, size_t id_masa, float m) {
    float sumatoria_x = 0;
    float xj_uno_anterior = 0;
    float xk_uno_anterior = 0;
    float xj_dos_anteriores = 0;
    float k_resorte;

    for (size_t i = 0; i < uno_anterior->cant_masas; i++) {
        if (i == id_masa) {
            xj_uno_anterior = uno_anterior->datos_masas[i].x;
            xj_dos_anteriores = dos_anteriores->datos_masas[i].x;
            break; // Salir del bucle una vez que se encuentra 
        }
    }

    for (size_t i = 0; i < uno_anterior->cant_resortes; i++) {
        if (uno_anterior->datos_resortes[i].id_masa1 == id_masa) {
            size_t id_masa2 = uno_anterior->datos_resortes[i].id_masa2;
            xk_uno_anterior = uno_anterior->datos_masas[id_masa2].x;
            float l_inicial = uno_anterior->datos_resortes[i].l_inicial;
            float l_uno_anterior = uno_anterior->datos_resortes[i].l_nueva;
            k_resorte = K_BASE / pow(uno_anterior->datos_resortes[i].l_inicial, POTENCIA_K);
        
            sumatoria_x += k_resorte * ((l_inicial - l_uno_anterior) / l_uno_anterior) * (xj_uno_anterior - xk_uno_anterior);

        } else if (uno_anterior->datos_resortes[i].id_masa2 == id_masa) {
            size_t id_masa1 = uno_anterior->datos_resortes[i].id_masa1;
            xk_uno_anterior = uno_anterior->datos_masas[id_masa1].x;
            float l_inicial = uno_anterior->datos_resortes[i].l_inicial;
            float l_uno_anterior = uno_anterior->datos_resortes[i].l_nueva;
            k_resorte = K_BASE / pow(uno_anterior->datos_resortes[i].l_inicial, POTENCIA_K);
        
            sumatoria_x += k_resorte * ((l_inicial - l_uno_anterior) / l_uno_anterior) * (xj_uno_anterior - xk_uno_anterior);
        }
    }
    float bj = calcular_bx(m, xj_uno_anterior, xj_dos_anteriores, sumatoria_x);
    float aj = calcular_aj(m);
    float pos_x = bj / aj;
    
    return pos_x;
}

float calcular_nueva_coordenada_y(instante_t* uno_anterior, instante_t* dos_anteriores, simulacion_t* simulacion, size_t id_masa, float m) {
    float sumatoria_y = 0;
    float yj_uno_anterior = 0;
    float yk_uno_anterior = 0;
    float yj_dos_anteriores = 0;
    float k_resorte;

    for (size_t i = 0; i < uno_anterior->cant_masas; i++) {
        if (i == id_masa) {
            yj_uno_anterior = uno_anterior->datos_masas[i].y;
            yj_dos_anteriores = dos_anteriores->datos_masas[i].y;
            break; // Salir del bucle una vez que se encuentra 
        }
    }


    for (size_t i = 0; i < uno_anterior->cant_resortes; i++) {
        if (uno_anterior->datos_resortes[i].id_masa1 == id_masa) {
            size_t id_masa2 = uno_anterior->datos_resortes[i].id_masa2;
            yk_uno_anterior = uno_anterior->datos_masas[id_masa2].y;
            float l_inicial = uno_anterior->datos_resortes[i].l_inicial;
            float l_uno_anterior = uno_anterior->datos_resortes[i].l_nueva;
            k_resorte = K_BASE / pow(uno_anterior->datos_resortes[i].l_inicial, POTENCIA_K);
        
            sumatoria_y += k_resorte * ((l_inicial - l_uno_anterior) / l_uno_anterior) * (yj_uno_anterior - yk_uno_anterior);

        } else if (uno_anterior->datos_resortes[i].id_masa2 == id_masa) {
            size_t id_masa1 = uno_anterior->datos_resortes[i].id_masa1;
            yk_uno_anterior = uno_anterior->datos_masas[id_masa1].y;
            float l_inicial = uno_anterior->datos_resortes[i].l_inicial;
            float l_uno_anterior = uno_anterior->datos_resortes[i].l_nueva;
            k_resorte = K_BASE / pow(uno_anterior->datos_resortes[i].l_inicial, POTENCIA_K);
        
            sumatoria_y += k_resorte * ((l_inicial - l_uno_anterior) / l_uno_anterior) * (yj_uno_anterior - yk_uno_anterior);
        }
    }
    float bj = calcular_by(m, yj_uno_anterior, yj_dos_anteriores, sumatoria_y);
    float aj = calcular_aj(m);
    float pos_y = bj / aj;

    return pos_y;
}

instante_t* calcular_instante_nuevo(instante_t* uno_anterior, instante_t* dos_anteriores, simulacion_t* simulacion) {
    instante_t* nuevo_instante = malloc(sizeof(instante_t));
    if (nuevo_instante == NULL) return NULL;
    
    nuevo_instante->cant_masas = uno_anterior->cant_masas;
    nuevo_instante->cant_resortes = uno_anterior->cant_resortes;
    
    nuevo_instante->datos_masas = malloc(nuevo_instante->cant_masas * sizeof(datos_masa_t));
    if (nuevo_instante->datos_masas == NULL) {
        free(nuevo_instante);
        return NULL;
    }
    
    nuevo_instante->datos_resortes = malloc(nuevo_instante->cant_resortes * sizeof(datos_resorte_t));
    if (nuevo_instante->datos_resortes == NULL) {
        free(nuevo_instante->datos_masas);
        free(nuevo_instante);
        return NULL;
    }

    float m = MASA_TOTAL / (uno_anterior->cant_masas);
    
    for (size_t i = 0; i < nuevo_instante->cant_masas; i++) {
        nuevo_instante->datos_masas[i].x = calcular_nueva_coordenada_x(uno_anterior, dos_anteriores, simulacion, i, m);
        nuevo_instante->datos_masas[i].y = calcular_nueva_coordenada_y(uno_anterior, dos_anteriores, simulacion, i, m);
    }
    
    for (size_t i = 0; i < nuevo_instante->cant_resortes; i++) {
        nuevo_instante->datos_resortes[i].id_masa1 = uno_anterior->datos_resortes[i].id_masa1;
        nuevo_instante->datos_resortes[i].id_masa2 = uno_anterior->datos_resortes[i].id_masa2;
        nuevo_instante->datos_resortes[i].l_inicial = uno_anterior->datos_resortes[i].l_inicial;

        size_t id_masa1 = nuevo_instante->datos_resortes[i].id_masa1;
        size_t id_masa2 = nuevo_instante->datos_resortes[i].id_masa2;

        float x1 = uno_anterior->datos_masas[id_masa1].x;
        float y1 = uno_anterior->datos_masas[id_masa1].y;
        float x2 = uno_anterior->datos_masas[id_masa2].x;
        float y2 = uno_anterior->datos_masas[id_masa2].y;

        nuevo_instante->datos_resortes[i].l_nueva = calcular_l_actual(x1, y1, x2, y2);
    }
    
    return nuevo_instante;
}

struct instante* copiar_instante(const struct instante* original) {
    if (original == NULL) {
        return NULL;
    }

    struct instante* copia = malloc(sizeof(struct instante));
    if (copia == NULL) {
        return NULL;
    }

    copia->cant_masas = original->cant_masas;
    copia->cant_resortes = original->cant_resortes;

    copia->datos_masas = malloc(sizeof(struct datos_masa) * copia->cant_masas);
    if (copia->datos_masas == NULL) {
        free(copia);
        return NULL;
    }

    copia->datos_resortes = malloc(sizeof(struct datos_resorte) * copia->cant_resortes);
    if (copia->datos_resortes == NULL) {
        free(copia->datos_masas);
        free(copia);
        return NULL;
    }

    for (size_t i = 0; i < copia->cant_masas; i++) {
        copia->datos_masas[i].x = original->datos_masas[i].x;
        copia->datos_masas[i].y = original->datos_masas[i].y;
    }

    for (size_t i = 0; i < copia->cant_resortes; i++) {
        copia->datos_resortes[i].id_masa1 = original->datos_resortes[i].id_masa1;
        copia->datos_resortes[i].id_masa2 = original->datos_resortes[i].id_masa2;
        copia->datos_resortes[i].l_nueva = original->datos_resortes[i].l_nueva;
        copia->datos_resortes[i].l_inicial = original->datos_resortes[i].l_inicial;
    }

    return copia;
}

simulacion_t* _simulacion_crear() {
    simulacion_t* simulacion = malloc(sizeof(simulacion_t));
    if (simulacion == NULL) {
        return NULL;
    }

    simulacion->instantes = lista_crear();
    if (simulacion->instantes == NULL) {
        free(simulacion);
        return NULL;
    }

    return simulacion;
}

simulacion_t* simulacion_crear(struct instante* uno_anterior, struct instante* dos_anteriores, struct simulacion* simulacion) {
    lista_insertar_primero(simulacion->instantes, dos_anteriores);
    lista_insertar_ultimo(simulacion->instantes, uno_anterior);

    return simulacion;
}

void destruir_instante(struct instante* instante) {
    if (instante == NULL) {
        return;
    }
    free(instante->datos_resortes);
    free(instante->datos_masas);
    free(instante);
}

void destruir_simulacion(simulacion_t* simulacion) {
    if (simulacion == NULL) {
        return;
    }

    lista_iter_t* iter = lista_iter_crear(simulacion->instantes);
    if (iter == NULL) {
        return;
    }

    while (!lista_iter_al_final(iter)) {
        struct instante* instante = lista_iter_ver_actual(iter);
        destruir_instante(instante);
        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    lista_destruir(simulacion->instantes, NULL);
    free(simulacion);
}

struct instante* crear_instante_desde_malla(malla_t* malla) {
    struct instante* nuevo_instante = (struct instante*) malloc(sizeof(struct instante));
    if (nuevo_instante == NULL) {
        return NULL;
    }

    // Obtener la cantidad de masas en la malla
    size_t num_masas = malla_cantidad_masas(malla);

    // Asignar memoria para los datos de masas
    nuevo_instante->datos_masas = (struct datos_masa*)malloc(sizeof(struct datos_masa) * num_masas);
    if (nuevo_instante->datos_masas == NULL) {
        free(nuevo_instante);
        return NULL;
    }

    // Asignar los datos de las masas
    for (size_t i = 0; i < num_masas; i++) {
        nuevo_instante->datos_masas[i].x = malla_masa_obtener_coordx(malla_obtener_masa_por_id(malla, i));
        nuevo_instante->datos_masas[i].y = malla_masa_obtener_coordy(malla_obtener_masa_por_id(malla, i));
    }

    // Obtener la cantidad de resortes en la malla
    size_t num_resortes = malla_cantidad_resortes(malla);

    // Asignar memoria para los datos de resortes
    nuevo_instante->datos_resortes = (struct datos_resorte*)malloc(sizeof(struct datos_resorte) * num_resortes);
    if (nuevo_instante->datos_resortes == NULL) {
        free(nuevo_instante->datos_masas);
        free(nuevo_instante);
        return NULL;
    }

    // Asignar los datos de los resortes
    for (size_t i = 0; i < num_resortes; i++) {
        nuevo_instante->datos_resortes[i].id_masa1 = malla_obtener_id_masa(malla_id_resorte_masa1(malla, i));
        nuevo_instante->datos_resortes[i].id_masa2 = malla_obtener_id_masa(malla_id_resorte_masa2(malla, i));
        nuevo_instante->datos_resortes[i].l_nueva = malla_resorte_longitud_actual(malla, i);
        nuevo_instante->datos_resortes[i].l_inicial = malla_resorte_longitud_actual(malla, i);
    }

    nuevo_instante->cant_masas = num_masas;
    nuevo_instante->cant_resortes = num_resortes;

    return nuevo_instante;
}

simulacion_t* simulacion_inicio(malla_t* malla) {
    simulacion_t* simulacion = _simulacion_crear();
    if (simulacion == NULL) {
        return NULL;
    }

    struct instante* uno_anterior = crear_instante_desde_malla(malla);
    if (uno_anterior == NULL) {
        destruir_simulacion(simulacion);
        return NULL;
    }

    struct instante* dos_anteriores = copiar_instante(uno_anterior);
    if (dos_anteriores == NULL) {
        destruir_instante(dos_anteriores);
        destruir_simulacion(simulacion);
        return NULL;
    }

    simulacion = simulacion_crear(uno_anterior, dos_anteriores, simulacion);

    return simulacion;
}

void simulacion_a_malla(malla_t* malla_simulacion, simulacion_t* simulacion) {
    // Obtener el instante uno_anterior de la simulación
    instante_t* uno_anterior = simulacion_instante_uno_anterior(simulacion);
    if (uno_anterior == NULL) {
        return;
    }
    // Recorrer los datos de masas del instante uno_anterior
    for (size_t i = 0; i < uno_anterior->cant_masas; i++) {
        // Obtener la masa correspondiente en la malla de simulación
        masa_t* masa_simulacion = malla_obtener_masa_por_id(malla_simulacion, i);
        if (masa_simulacion == NULL) {
            continue;
        }

        // Obtener las coordenadas de la masa del instante uno_anterior
        float coordx = uno_anterior->datos_masas[i].x;
        float coordy = uno_anterior->datos_masas[i].y;

        // Actualizar las coordenadas de la masa en la malla de simulación
        malla_actualizar_coord(masa_simulacion, coordx, coordy);
    }

    // Recorrer los datos de resortes del instante uno_anterior
    for (size_t i = 0; i < uno_anterior->cant_resortes; i++) {
        // Obtener las masas correspondientes en la malla de simulación
        masa_t* masa1 = malla_obtener_masa_por_id(malla_simulacion, uno_anterior->datos_resortes[i].id_masa1);
        masa_t* masa2 = malla_obtener_masa_por_id(malla_simulacion, uno_anterior->datos_resortes[i].id_masa2);
        if (masa1 == NULL || masa2 == NULL) {
            continue;
        }

        // Obtener el resorte correspondiente en la malla de simulación
        resorte_t* resorte_simulacion = malla_obtener_resorte_por_id(malla_simulacion, i);
        if (resorte_simulacion == NULL) {
            continue;
        }

        // Actualizar la longitud inicial del resorte en la malla de simulación
        malla_resorte_actualizar_l0(resorte_simulacion, uno_anterior->datos_resortes[i].l_inicial);
    }
}

void simulacion_agregar(simulacion_t* simulacion, malla_t* malla_simulacion) {
    struct instante* nuevo_instante = calcular_instante_nuevo(simulacion_instante_uno_anterior(simulacion), simulacion_instante_dos_anteriores(simulacion), simulacion);
    lista_insertar_ultimo(simulacion->instantes, nuevo_instante);
    struct instante* dos_anteriores = lista_borrar_primero(simulacion->instantes);
    destruir_instante(dos_anteriores);
}

struct instante* simulacion_instante_uno_anterior(simulacion_t* simulacion) {
    if (simulacion == NULL || lista_esta_vacia(simulacion->instantes)) {
        return NULL;
    }

    lista_iter_t* iter = lista_iter_crear(simulacion->instantes);
    if (iter == NULL) {
        return NULL;
    }

    struct instante* uno_anterior = NULL;
    while (!lista_iter_al_final(iter)) {
        uno_anterior = lista_iter_ver_actual(iter);
        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return uno_anterior;
}

struct instante* simulacion_instante_dos_anteriores(simulacion_t* simulacion) {
    if (simulacion == NULL || lista_largo(simulacion->instantes) < 2) {
        return NULL;
    }

    lista_iter_t* iter = lista_iter_crear(simulacion->instantes);
    if (iter == NULL) {
        return NULL;
    }

    struct instante* dos_anteriores = lista_iter_ver_actual(iter);
    lista_iter_destruir(iter);

    return dos_anteriores;
}

void dibujar_masa_simulacion(const struct datos_masa* masa, SDL_Renderer* renderer) {
    SDL_Rect r1 = { (masa->x - TAM_MASA / 2) * FACTOR_ESCALA, (masa->y - TAM_MASA / 2) * FACTOR_ESCALA, TAM_MASA * FACTOR_ESCALA, TAM_MASA * FACTOR_ESCALA };
    SDL_RenderDrawRect(renderer, &r1);
}

void dibujar_resorte_simulacion(const struct datos_resorte *resorte, SDL_Renderer *renderer, const struct datos_masa *masas) {
    if (resorte == NULL || renderer == NULL) {
        return;
    }

    size_t id_masa1 = resorte->id_masa1;
    size_t id_masa2 = resorte->id_masa2;

    float x1 = (masas[id_masa1].x);
    float y1 = (masas[id_masa1].y);
    float x2 = (masas[id_masa2].x);
    float y2 = (masas[id_masa2].y);

    SDL_SetRenderDrawColor(renderer, 0xDA, 0xF7, 0xA6, 0xFF);
    SDL_RenderDrawLine(renderer, (int)(x1*FACTOR_ESCALA), (int)(y1*FACTOR_ESCALA), (int)(x2*FACTOR_ESCALA), (int)(y2*FACTOR_ESCALA));
}

void dibujar_instante_simulacion(const struct instante *instante, SDL_Renderer *renderer) {
    struct datos_masa *masas = instante->datos_masas;
    struct datos_resorte *resortes = instante->datos_resortes;

    // Dibujar masas
    for(size_t i = 0; i < instante->cant_masas ; i++) {
        dibujar_masa_simulacion(&masas[i], renderer);
    }

    // Dibujar resortes
    
    for(size_t i = 0; i < instante->cant_resortes; i++) {
        dibujar_resorte_simulacion(&resortes[i], renderer, masas);
    }
}

void simular(struct simulacion *simulacion, SDL_Renderer *renderer) {
    dibujar_instante_simulacion(simulacion_instante_uno_anterior(simulacion), renderer);
}


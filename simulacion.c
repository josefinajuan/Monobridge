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
    struct instante* siguiente;
};

struct simulacion
{
    struct instante* uno_anterior;
    struct instante* dos_anteriores;
};

float calcular_l_actual(float x1, float y1, float x2, float y2) {
    float distancia = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return distancia;
}

float calcular_coordy_m(struct instante* uno_anterior, int id_masa) {
    return uno_anterior->datos_masas[id_masa].y;
}

float calcular_coordx_m(struct instante* uno_anterior, int id_masa) {
    return uno_anterior->datos_masas[id_masa].x;
}

float calcular_coordy(float y_uno_anterior, float y_dos_anteriores, float l_nueva_uno_anterior, float l_inicial, float sumatoria) {
    
    float b = (MASA_TOTAL / (DT * DT)) * ((2 * y_uno_anterior) - y_dos_anteriores) + (B / DT) * y_uno_anterior + MASA_TOTAL * G + sumatoria ;
    float a = (MASA_TOTAL/ (DT * DT))+ (B / DT);
    
    float resultado = b/a;
    
    return resultado;

}

float calcular_coordx(float x_uno_anterior, float x_dos_anteriores, float l_nueva_uno_anterior, float l_inicial, float sumatoria) {
    
    float b = (MASA_TOTAL / (DT * DT)) * ((2 * x_uno_anterior) - x_dos_anteriores) + (B / DT) * x_uno_anterior  + sumatoria ;
    float a = (MASA_TOTAL/ (DT * DT))+ (B / DT);
    
    float resultado = b/a;
    
    return resultado;

}

struct instante* calcular_nuevo_instante(struct instante* uno_anterior, struct instante* dos_anteriores, float* vector) {
    int num_masas = 0;
    int num_resortes = 0;

    // Iterar sobre los datos de masas hasta encontrar un puntero nulo
    struct datos_masa* masas_iter = uno_anterior->datos_masas;
    while (masas_iter != NULL) {
        num_masas++;
        masas_iter++;
    }

    // Iterar sobre los datos de resortes hasta encontrar un puntero nulo
    struct datos_resorte* resortes_iter = uno_anterior->datos_resortes;
    while (resortes_iter != NULL) {
        num_resortes++;
        resortes_iter++;
    }

    struct datos_masa* datos_masas_nuevo = malloc(sizeof(struct datos_masa) * num_masas);
   

    for (size_t i = 0; i < num_masas; i++) {
        float sumatoriay = 0;
        float sumatoriax = 0;
        for(size_t r = 0; r < num_resortes; r++){
            sumatoriay += (K_BASE * ((uno_anterior->datos_resortes[r].l_inicial) - (uno_anterior->datos_resortes[r].l_nueva))/ uno_anterior->datos_resortes[r].l_nueva) * (calcular_coordx_m(uno_anterior, uno_anterior->datos_resortes[r].id_masa1)) - calcular_coordx_m(uno_anterior, uno_anterior->datos_resortes[r].id_masa2);
            sumatoriay += (K_BASE * ((uno_anterior->datos_resortes[r].l_inicial) - (uno_anterior->datos_resortes[r].l_nueva))/ uno_anterior->datos_resortes[r].l_nueva) * (calcular_coordy_m(uno_anterior, uno_anterior->datos_resortes[r].id_masa1)) - calcular_coordy_m(uno_anterior, uno_anterior->datos_resortes[r].id_masa2);
        }
        float x_nuevo = calcular_coordx(uno_anterior->datos_masas[i].x, dos_anteriores->datos_masas[i].x, uno_anterior->datos_resortes[i].l_nueva, vector[i], sumatoriax);
        float y_nuevo = calcular_coordy(uno_anterior->datos_masas[i].y, dos_anteriores->datos_masas[i].y, uno_anterior->datos_resortes[i].l_nueva, vector[i], sumatoriay);

        datos_masas_nuevo[i].x = x_nuevo;
        datos_masas_nuevo[i].y = y_nuevo;


    }

    struct datos_resorte* datos_resortes_nuevo = malloc(sizeof(struct datos_resorte) * num_resortes);

    for (int i = 0; i < num_resortes; i++) {
        struct datos_resorte datos_resorte_anterior = uno_anterior->datos_resortes[i];

        datos_resortes_nuevo[i].id_masa1 = datos_resorte_anterior.id_masa1;
        datos_resortes_nuevo[i].id_masa2 = datos_resorte_anterior.id_masa2;
        datos_resortes_nuevo[i].l_inicial = datos_resorte_anterior.l_inicial;

        float x1 = datos_masas_nuevo[datos_resorte_anterior.id_masa1].x;
        float y1 = datos_masas_nuevo[datos_resorte_anterior.id_masa1].y;
        float x2 = datos_masas_nuevo[datos_resorte_anterior.id_masa2].x;
        float y2 = datos_masas_nuevo[datos_resorte_anterior.id_masa2].y;

        float l_actual_nueva = calcular_l_actual(x1, y1, x2, y2);

        datos_resortes_nuevo[i].l_nueva = l_actual_nueva;
    }

    struct instante* nuevo_instante = malloc(sizeof(struct instante));
    nuevo_instante->datos_masas = datos_masas_nuevo;
    nuevo_instante->datos_resortes = datos_resortes_nuevo;
    nuevo_instante->siguiente = NULL;

    return nuevo_instante;
}

struct instante* crear_instante(malla_t* malla) {
    int num_masas = malla_cantidad_masas(malla);
    int num_resortes = malla_cantidad_resortes(malla);

    struct datos_masa* datos_masas = malloc(sizeof(struct datos_masa) * num_masas);
    struct datos_resorte* datos_resortes = malloc(sizeof(struct datos_resorte) * num_resortes);

    // Copiar datos de las masas
    for (int i = 0; i < num_masas; i++) {
        masa_t* masa = malla_obtener_masa_por_id(malla, i);
        float x = masa_obtener_coordx(masa);
        float y = masa_obtener_coordy(masa);

        datos_masas[i].x = x;
        datos_masas[i].y = y;
    }

    float* vector = obtener_l0_resortes(malla);

    // Copiar datos de los resortes
    for (int i = 0; i < num_resortes; i++) {
        resorte_t* resorte = malla_obtener_resorte_por_id(malla, i);

        size_t id_masa1 = malla_resorte_obtener_id_m1(resorte);
        size_t id_masa2 = malla_resorte_obtener_id_m2(resorte);
        float l_nueva = vector[i];
        float l_inicial = malla_resorte_longitud_actual(malla, i);

        datos_resortes[i].id_masa1 = id_masa1;
        datos_resortes[i].id_masa2 = id_masa2;
        datos_resortes[i].l_nueva = l_nueva;
        datos_resortes[i].l_inicial = l_inicial;
    }

    // Crear el instante
    struct instante* instante = malloc(sizeof(struct instante));
    instante->datos_masas = datos_masas;
    instante->datos_resortes = datos_resortes;
    instante->siguiente = NULL;

    return instante;
}

simulacion_t* _simulacion_crear() {
    simulacion_t* simulacion = malloc(sizeof(simulacion_t));
    if (simulacion == NULL) {
        return NULL;
    }

    simulacion->uno_anterior = NULL;
    simulacion->dos_anteriores = NULL;

    return simulacion;
}

simulacion_t* simulacion_crear(struct instante* uno_anterior, struct instante* dos_anteriores, simulacion_t* simulacion) {
    uno_anterior->siguiente = dos_anteriores;

    simulacion->uno_anterior = uno_anterior;
    simulacion->dos_anteriores = dos_anteriores;

    return simulacion;
}

void simulacion_agregar(struct instante* uno_anterior, struct instante* dos_anteriores, float* vector, simulacion_t* simulacion) {    // Crear el nuevo instante utilizando la función calcular_nuevo_instante
    struct instante* nuevo_instante = calcular_nuevo_instante(uno_anterior, dos_anteriores, vector);

    // Liberar los datos y la memoria asociada al instante dos_anterior actual
    free(dos_anteriores->datos_masas);
    free(dos_anteriores->datos_resortes);
    free(dos_anteriores);

    nuevo_instante->siguiente = uno_anterior;

    simulacion->dos_anteriores = uno_anterior;

    simulacion->uno_anterior = nuevo_instante;
}

void destruir_simulacion(simulacion_t* simulacion) {
    if (simulacion == NULL) {
        return;
    }

    struct instante* uno_anterior = simulacion->uno_anterior;
    struct instante* dos_anteriores = simulacion->dos_anteriores;

    while (uno_anterior != NULL) {
        struct instante* siguiente_uno = uno_anterior->siguiente;
        destruir_instante(uno_anterior);
        uno_anterior = siguiente_uno;
    }

    while (dos_anteriores != NULL) {
        struct instante* siguiente_dos = dos_anteriores->siguiente;
        destruir_instante(dos_anteriores);
        dos_anteriores = siguiente_dos;
    }

    free(simulacion);
}

void destruir_instante(struct instante* instante) {
    if (instante == NULL) {
        return;
    }

    free(instante->datos_masas);
    free(instante->datos_resortes);
    free(instante);
}

simulacion_t* simulacion_inicio(malla_t* malla) {
    // Crear la simulación
    simulacion_t* simulacion = _simulacion_crear();
    if (simulacion == NULL) {
        return NULL;
    }

    // Crear el primer instante desde la malla
    struct instante* primer_instante = crear_instante(malla);
    if (primer_instante == NULL) {
        destruir_simulacion(simulacion);
        return NULL;
    }

    // Crear el segundo instante igual al primero
    struct instante* segundo_instante = crear_instante(malla);
    if (segundo_instante == NULL) {
        destruir_instante(primer_instante);
        destruir_simulacion(simulacion);
        return NULL;
    }

    // Establecer los instantes en la simulación
    simulacion_crear(primer_instante, segundo_instante, simulacion);

    return simulacion;
}

void simulacion_a_malla(malla_t* malla_simulacion, simulacion_t* simulacion) {
    // Obtener el primer instante de la simulación
    struct instante* uno_anterior = simulacion->uno_anterior;

    // Obtener las listas de resortes y masas de la malla simulación
    lista_t* lista_resortes = malla_obtener_lista_resortes(malla_simulacion);
    lista_t* lista_masas = malla_obtener_lista_masas(malla_simulacion);

    // Copiar los datos de masas en la malla simulación
    struct datos_masa* datos_masas = uno_anterior->datos_masas;
    lista_iter_t* iter_masas = lista_iter_crear(lista_masas);
    if (iter_masas == NULL) {
        return;
    }

    while (!lista_iter_al_final(iter_masas) && datos_masas != NULL) {
        masa_t* masa = lista_iter_ver_actual(iter_masas);
        malla_actualizar_coord(masa, datos_masas->x, datos_masas->y);

        lista_iter_avanzar(iter_masas);
    }

    lista_iter_destruir(iter_masas);

    // Copiar los datos de resortes en la malla simulación
    struct datos_resorte* datos_resortes = uno_anterior->datos_resortes;
    lista_iter_t* iter_resortes = lista_iter_crear(lista_resortes);
    if (iter_resortes == NULL) {
        return;
    }

    while (!lista_iter_al_final(iter_resortes) && datos_resortes != NULL) {
        resorte_t* resorte = lista_iter_ver_actual(iter_resortes);
        malla_resorte_actualizar_l0(resorte, datos_resortes->l_inicial);

        lista_iter_avanzar(iter_resortes);
    }

    lista_iter_destruir(iter_resortes);
}

struct instante* simulacion_instante_uno_anterior(simulacion_t* simulacion) {
    if (simulacion == NULL) {
        return NULL;
    }
    return simulacion->uno_anterior;
}

struct instante* simulacion_instante_dos_anteriores(simulacion_t* simulacion) {
    if (simulacion == NULL) {
        return NULL;
    }
    return simulacion->dos_anteriores;
}

void dibujar_masa_simulacion(const struct datos_masa *masa, SDL_Renderer *renderer) {
    SDL_Rect r1 = {((masa->x) - 15 / 2), ((masa->y) - 15 / 2), 15, 15};
    SDL_RenderDrawRect(renderer, &r1);
}

void dibujar_resorte_simulacion(const struct datos_resorte *resorte, SDL_Renderer *renderer, const struct datos_masa *masas) {
    if (resorte == NULL || renderer == NULL) {
        return;
    }

    size_t id_masa1 = resorte->id_masa1;
    size_t id_masa2 = resorte->id_masa2;

    float x1 = masas[id_masa1].x;
    float y1 = masas[id_masa1].y;
    float x2 = masas[id_masa2].x;
    float y2 = masas[id_masa2].y;

    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderDrawLine(renderer, (int)x1, (int)y1, (int)x2, (int)y2);
}

void dibujar_instante_simulacion(const struct instante *instante, SDL_Renderer *renderer) {
    struct datos_masa *masas = instante->datos_masas;
    struct datos_resorte *resortes = instante->datos_resortes;

    // Dibujar masas
    for(size_t i = 0; masas[i].x != 0.0f && masas[i].y != 0.0f; i++) {
        dibujar_masa_simulacion(&masas[i], renderer);
    }

    // Dibujar resortes
    
    for(size_t i = 0; resortes[i].id_masa1 != 0 && resortes[i].id_masa2 != 0; i++) {
        dibujar_resorte_simulacion(&resortes[i], renderer, masas);
    }
}

void dibujar_simulacion(struct simulacion *simulacion, SDL_Renderer *renderer) {
    struct instante *uno_anterior = simulacion->uno_anterior;

    // Dibujar instante uno_anterior
    dibujar_instante_simulacion(uno_anterior, renderer);
}


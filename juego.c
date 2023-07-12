#include "juego.h"
#include "malla.h"
#include "lista.h"
#include "config.h"

void inicializar_nivel(malla_t *malla, int nivel) {
     if (malla == NULL || nivel < 1) {
        return;
    }
    // Calcular la distancia horizontal entre las masas fijas
    float distancia_horizontal = 4.0 + nivel - 1;

    // Calcular las coordenadas de las masas fijas
    float coordx1 = distancia_horizontal / 2.0;
    float coordy1 = 0.5;
    float coordx2 = coordx1 + distancia_horizontal;
    float coordy2 = 0.5;

    size_t id_masa1 = 0;
    size_t id_masa2 = 1;

     // Crear las masas fijas y agregarlas a la malla
    malla_masa_nueva_fija(malla,id_masa1,coordx1,coordy1, TAM_MASA/FACTOR_ESCALA);
    malla_masa_nueva_fija(malla,id_masa2,coordx2,coordy2, TAM_MASA/FACTOR_ESCALA);
}

bool leer_numero_de_datos(FILE *f, size_t *datos) {
    if (f == NULL || datos == NULL) {
        return false;
    }

    if (fread(datos, sizeof(size_t), 1, f) != 1) {
        return false;
    }

    return true;
}

bool leer_masa(FILE *f, size_t *id, float *x, float *y, float *tam, bool *es_fijo, float *masa) {
    if (f == NULL || id == NULL || x == NULL || y == NULL || tam == NULL || es_fijo == NULL || masa == NULL) {
        return false;
    }

    if (fread(id, sizeof(size_t), 1, f) != 1) {
        return false;
    }

    if (fread(x, sizeof(float), 1, f) != 1) {
        return false;
    }

    if (fread(y, sizeof(float), 1, f) != 1) {
        return false;
    }

    if (fread(tam, sizeof(float), 1, f) != 1) {
        return false;
    }

    if (fread(es_fijo, sizeof(bool), 1, f) != 1) {
        return false;
    }

    if (fread(masa, sizeof(float), 1, f) != 1) {
        return false;
    }

    return true;
}

bool leer_resorte(FILE *f, size_t *id, size_t *id_masa_1, size_t *id_masa_2, float *longitud){
    if (f == NULL || id == NULL || id_masa_1 == NULL || id_masa_2 == NULL || longitud == NULL) {
        return false;
    }

    if (fread(id, sizeof(size_t), 1, f) != 1) {
        return false;
    }

    if (fread(id_masa_1, sizeof(size_t), 1, f) != 1) {
        return false;
    }

    if (fread(id_masa_2, sizeof(size_t), 1, f) != 1) {
        return false;
    }

    if (fread(longitud, sizeof(float), 1, f) != 1) {
        return false;
    }
    return true;
}

bool escribir_numero_de_datos(FILE *f, size_t datos) {
     if (f == NULL) {
        return false;
    }

    if (fwrite(&datos, sizeof(size_t), 1, f) != 1) {
        return false;
    }
    return true;
}

bool escribir_masa(FILE *f, size_t id, float x, float y, float tam, bool es_fijo, float masa) {

    if (f == NULL) {
        return false;
    }

    // Escribir el ID de la masa
    if (fwrite(&id, sizeof(size_t), 1, f) != 1) {
        return false;
    }

    // Escribir las coordenadas X e Y de la masa
    if (fwrite(&x, sizeof(float), 1, f) != 1 || fwrite(&y, sizeof(float), 1, f) != 1) {
        return false;
    }

    // Escribir el tamaño de la masa
    if (fwrite(&tam, sizeof(float), 1, f) != 1) {
        return false;
    }

    // Escribir el indicador de si la masa es fija o no
    if (fwrite(&es_fijo, sizeof(bool), 1, f) != 1) {
        return false;
    }

    // Escribir la masa de la masa
    if (fwrite(&masa, sizeof(float), 1, f) != 1) {
        return false;
    }

    return true;
}

bool escribir_resorte(FILE *f, size_t id, size_t id_masa_1, size_t id_masa_2, float longitud) {
     if (f == NULL) {
        return false;
    }

    // Escribir el ID del resorte
    if (fwrite(&id, sizeof(size_t), 1, f) != 1) {
        return false;
    }

    // Escribir los IDs de las masas que conecta el resorte
    if (fwrite(&id_masa_1, sizeof(size_t), 1, f) != 1 || fwrite(&id_masa_2, sizeof(size_t), 1, f) != 1) {
        return false;
    }

    // Escribir la longitud del resorte
    if (fwrite(&longitud, sizeof(float), 1, f) != 1) {
        return false;
    }

    return true;
}

bool guardar_nivel(FILE *f, malla_t *malla) {
    if (f == NULL || malla == NULL) {
        return false;
    }

    // Obtener el número de masas y resortes en la malla
    size_t num_masas = malla_cantidad_masas(malla);
    size_t num_resortes = malla_cantidad_resortes(malla);

    // Escribir el número de masas y resortes en el archivo
    if (!escribir_numero_de_datos(f, num_masas) || !escribir_numero_de_datos(f, num_resortes)) {
        return false;
    }

    // Escribir los datos de cada masa en el archivo
    lista_t *masas = malla_obtener_lista_masas(malla);
    lista_iter_t *masas_iter = lista_iter_crear(masas);
    if (masas_iter == NULL) {
        return false;
    }

    while (!lista_iter_al_final(masas_iter)) {
        masa_t *masa = lista_iter_ver_actual(masas_iter);
        size_t id = malla_obtener_id_masa(masa);
        float x = malla_masa_obtener_coordx(masa);
        float y = malla_masa_obtener_coordy(masa);
        float tam = malla_masa_obtener_tam(masa);
        bool es_fijo = malla_masa_es_fija(masa);
        float masa_total = malla_masa_obtener_masa(masa);

        if (!escribir_masa(f, id, x, y, tam, es_fijo, masa_total)) {
            lista_iter_destruir(masas_iter);
            return false;
        }

        lista_iter_avanzar(masas_iter);
    }

    lista_iter_destruir(masas_iter);

    // Escribir los datos de cada resorte en el archivo
    lista_t *resortes = malla_obtener_lista_resortes(malla);
    lista_iter_t *resortes_iter = lista_iter_crear(resortes);
    if (resortes_iter == NULL) {
        return false;
    }

    while (!lista_iter_al_final(resortes_iter)) {
        resorte_t *resorte = lista_iter_ver_actual(resortes_iter);
        size_t id = malla_resorte_obtener_id(resorte);
        size_t id_masa_1 = malla_resorte_obtener_id_m1(resorte);
        size_t id_masa_2 = malla_resorte_obtener_id_m1(resorte);
        float longitud = malla_resorte_obtener_lo(resorte);

        if (!escribir_resorte(f, id, id_masa_1, id_masa_2, longitud)) {
            lista_iter_destruir(resortes_iter);
            return false;
        }

        lista_iter_avanzar(resortes_iter);
    }

    lista_iter_destruir(resortes_iter);

    return true;
}

bool abrir_nivel(FILE *f, malla_t *malla) {
    if (f == NULL || malla == NULL) {
        return false;
    }

    // Leer el número de masas y resortes del archivo
    size_t num_masas, num_resortes;
    if (!leer_numero_de_datos(f, &num_masas) || !leer_numero_de_datos(f, &num_resortes)) {
        return false;
    }

    // Crear las masas y resortes correspondientes en la malla
    for (size_t i = 0; i < num_masas; i++) {
        size_t id;
        float x, y, tam, masa;
        bool es_fijo;
        if (!leer_masa(f, &id, &x, &y, &tam, &es_fijo, &masa)) {
            return false;
        }
        if(es_fijo){
            masa_t *masa_nueva = malla_masa_nueva_fija(malla, id, x, y, tam);
            insertar_masa_masa(masa_nueva, masa);
        } else if(es_fijo){
            masa_t *  masa_nueva = malla_masa_nueva(malla, id, x, y, tam);
            insertar_masa_masa(masa_nueva, masa);
        }
    }

    for (size_t i = 0; i < num_resortes; i++) {
        size_t id, id_masa_1, id_masa_2;
        float longitud;
        if (!leer_resorte(f, &id, &id_masa_1, &id_masa_2, &longitud)) {
            return false;
        }
        masa_t *masa1 = malla_obtener_masa_por_id(malla, id_masa_1);
        masa_t *masa2 = malla_obtener_masa_por_id(malla, id_masa_2);
        if (masa1 == NULL || masa2 == NULL) {
            return false;
        }
        resorte_t* resorte = malla_resorte_nuevo(malla, id, masa1, masa2);
        resorte_actualizar_l(resorte, longitud);
    }

    return true;
}

int cantidad_de_puntos(malla_t *malla, int nivel) {
    return malla_cantidad_resortes(malla);
}
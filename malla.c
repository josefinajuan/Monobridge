#include "masas.h"
#include "resortes.h"
#include "lista.h"
#include "malla.h"
#include "config.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct malla{
    lista_t* lista_resortes;
    lista_t* lista_masas;
};

float distancia_puntos(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

float distancia_punto_segmento(float xp, float yp, float xj, float yj, float xk, float yk) {
    // Calcular los componentes del vector (pk - pj)
    float dx = xk - xj;
    float dy = yk - yj;

    // Calcular la longitud del segmento al cuadrado (norma al cuadrado de (pk - pj))
    float norma_cuadrado = dx * dx + dy * dy;

    // Calcular el producto interno entre (p - pj) y (pk - pj)
    float producto_interno = (xp - xj) * dx + (yp - yj) * dy;

    // Calcular el parámetro alfa
    float alfa = producto_interno / norma_cuadrado;

    // Determinar el punto más cercano al segmento
    if (alfa <= 0) {
        // El punto más cercano es pj
        return distancia_puntos(xp, yp, xj, yj);
    } else if (alfa >= 1) {
        // El punto más cercano es pk
        return distancia_puntos(xp, yp, xk, yk);
    } else {
        // Calcular las coordenadas del punto más cercano
        float punto_cercano_x = xj + alfa * dx;
        float punto_cercano_y = yj + alfa * dy;

        // Calcular la distancia entre el punto más cercano y p
        return distancia_puntos(xp, yp, punto_cercano_x, punto_cercano_y);
    }
}

malla_t *malla_crear() {
    malla_t *m = malloc(sizeof(malla_t));
    if (m == NULL) return NULL;

    m->lista_resortes = lista_crear();
    if (m->lista_resortes == NULL) {
        free(m);
        return NULL;
    }

    m->lista_masas = lista_crear();
    if (m->lista_masas == NULL) {
        lista_destruir(m->lista_resortes, NULL);
        free(m);
        return NULL;
    }

    return m;
}

void malla_destruir(malla_t *m) {
    if (m == NULL) return;

    lista_destruir(m->lista_resortes, free);
    lista_destruir(m->lista_masas, free);

    free(m);
}

size_t malla_cantidad_masas(const malla_t* malla) {
    return lista_largo(malla->lista_masas);
}

size_t malla_cantidad_resortes(const malla_t* malla) {
    return lista_largo(malla->lista_resortes);
}
 
bool estoy_sobre_masa(float coordx, float coordy, const malla_t *malla) {
    lista_iter_t *iter = lista_iter_crear(malla->lista_masas);
    if (iter == NULL) {
        return false;
    }

    while (!lista_iter_al_final(iter)) {
        masa_t *masa = lista_iter_ver_actual(iter);
        float masa_coordx = masa_obtener_coordx(masa);
        float masa_coordy = masa_obtener_coordy(masa);

        if (coordx >= masa_coordx - MARGEN_ERROR && coordx <= masa_coordx + MARGEN_ERROR &&
            coordy >= masa_coordy - MARGEN_ERROR && coordy <= masa_coordy + MARGEN_ERROR) {
            lista_iter_destruir(iter);
            return true;
        }

        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return false;
}

bool estoy_sobre_resorte(float x, float y, const malla_t* malla) {
    lista_iter_t* iter = lista_iter_crear(malla->lista_resortes);
    if (iter == NULL) {
        return false;
    }

    while (!lista_iter_al_final(iter)) {
        resorte_t* resorte = lista_iter_ver_actual(iter);
        masa_t* masa_inicio = resorte_obtener_m1(resorte);
        masa_t* masa_fin = resorte_obtener_m2(resorte);

        float x1 = masa_obtener_coordx(masa_inicio);
        float y1 = masa_obtener_coordy(masa_inicio);
        float x2 = masa_obtener_coordx(masa_fin);
        float y2 = masa_obtener_coordy(masa_fin);

        float distancia = distancia_punto_segmento(x, y, x1, y1, x2, y2);

        if (distancia <= MARGEN_ERROR) {
            lista_iter_destruir(iter);
            return true;
        }

        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return false;
}

bool malla_eliminar_masa_por_coordenadas(malla_t* malla, float coordx, float coordy) {
    lista_iter_t* iter = lista_iter_crear(malla->lista_masas);
    if (iter == NULL) {
        return false;
    }

    while (!lista_iter_al_final(iter)) {
        masa_t* masa = lista_iter_ver_actual(iter);
        if(!es_fija(masa)){
            float masa_coordx = masa_obtener_coordx(masa);
            float masa_coordy = masa_obtener_coordy(masa);

            if (coordx >= masa_coordx - MARGEN_ERROR && coordx <= masa_coordx + MARGEN_ERROR &&
                coordy >= masa_coordy - MARGEN_ERROR && coordy <= masa_coordy + MARGEN_ERROR) {

                // Eliminar los resortes asociados a la masa
                lista_iter_t* iter_resortes = lista_iter_crear(malla->lista_resortes);
                if (iter_resortes == NULL) {
                    lista_iter_destruir(iter);
                    return true;
                }

                while (!lista_iter_al_final(iter_resortes)) {
                    resorte_t* resorte = lista_iter_ver_actual(iter_resortes);
                    masa_t* masa_inicio = resorte_obtener_m1(resorte);
                    masa_t* masa_fin = resorte_obtener_m2(resorte);

                    if (masa == masa_inicio || masa == masa_fin) {
                        lista_iter_borrar(iter_resortes);
                        resorte_destruir(resorte);
                    } else {
                        lista_iter_avanzar(iter_resortes);
                    }
                }

                lista_iter_destruir(iter_resortes);

                // Actualizar los IDs de los resortes restantes en lista_resortes
                lista_iter_t* iter_actualizado = lista_iter_crear(malla->lista_resortes);
                if (iter_actualizado == NULL) {
                    lista_iter_destruir(iter);
                    return true;
                }

                int nuevo_id = 0;
                while (!lista_iter_al_final(iter_actualizado)) {
                    resorte_t* resorte = lista_iter_ver_actual(iter_actualizado);
                    resorte_actualizar_id(resorte, nuevo_id);
                    nuevo_id++;
                    lista_iter_avanzar(iter_actualizado);
                }

                lista_iter_destruir(iter_actualizado);

                // Borrar la masa y actualizar los IDs de las masas restantes
                lista_iter_borrar(iter);
                masa_actualizar_ids_masa(iter, malla->lista_masas);

                lista_iter_destruir(iter);
                return true;
            }

        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return false;
}

void malla_eliminar_resorte_por_coordenadas(malla_t* malla, float x, float y) {
    lista_iter_t* iter = lista_iter_crear(malla->lista_resortes);
    if (iter == NULL) {
        return;
    }

    bool encontrado = false;
    int id_eliminado = -1;  // ID del resorte eliminado
    while (!lista_iter_al_final(iter)) {
        resorte_t* resorte = lista_iter_ver_actual(iter);
        masa_t* masa_inicio = resorte_obtener_m1(resorte);
        masa_t* masa_fin = resorte_obtener_m2(resorte);

        float x1 = masa_obtener_coordx(masa_inicio);
        float y1 = masa_obtener_coordy(masa_inicio);
        float x2 = masa_obtener_coordx(masa_fin);
        float y2 = masa_obtener_coordy(masa_fin);

        float distancia = distancia_punto_segmento(x, y, x1, y1, x2, y2);

        if (distancia <= MARGEN_ERROR) {
            lista_iter_borrar(iter);
            id_eliminado = resorte_obtener_id(resorte);  // Obtener el ID del resorte eliminado
            resorte_destruir(resorte);
            encontrado = true;
            break;
        }

        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);

    // Si se eliminó un resorte, actualizar los IDs de los resortes restantes
    if (encontrado) {
        lista_iter_t* iter_actualizado = lista_iter_crear(malla->lista_resortes);
        if (iter_actualizado == NULL) {
            return;
        }

        while (!lista_iter_al_final(iter_actualizado)) {
            resorte_t* resorte = lista_iter_ver_actual(iter_actualizado);
            int id_actual = resorte_obtener_id(resorte);
            if (id_actual > id_eliminado) {
                resorte_actualizar_id(resorte, id_actual - 1);  // Actualizar el ID del resorte restante
            }
            lista_iter_avanzar(iter_actualizado);
        }

        lista_iter_destruir(iter_actualizado);
    }
}

bool malla_agregar_masa(malla_t* malla, masa_t* masa) {
    return lista_insertar_ultimo(malla->lista_masas, masa);
}

bool malla_agregar_resorte(malla_t* malla, resorte_t* resorte) {
    return lista_insertar_ultimo(malla->lista_resortes, resorte);
}

masa_t* obtener_masa(const malla_t* malla, float coordx, float coordy) {
    if (malla == NULL)
        return NULL;

    lista_t* lista_masas = malla->lista_masas;
    lista_iter_t* iter = lista_iter_crear(lista_masas);
    if (iter == NULL)
        return NULL;

    while (!lista_iter_al_final(iter)) {
        masa_t* masa = lista_iter_ver_actual(iter);
        
        float masa_coordx = masa_obtener_coordx(masa);
        float masa_coordy = masa_obtener_coordy(masa);


        if (coordx >= masa_coordx - MARGEN_ERROR && coordx <= masa_coordx + MARGEN_ERROR &&
            coordy >= masa_coordy - MARGEN_ERROR && coordy <= masa_coordy + MARGEN_ERROR){
            lista_iter_destruir(iter);
            return masa;
        }
        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return NULL;
}

bool son_iguales(int x1, int y1, int x2, int y2) {
    return (x1 == x2 && y1 == y2);
}

void malla_actualizar_coord(masa_t* masa, float coordx, float coordy) {
    if (masa == NULL)
        return;

    masa_actualizar_coordx(masa, coordx);
    masa_actualizar_coordy(masa, coordy);
}

void malla_resorte_actualizar_l0(resorte_t* resorte, float l_inicial) {
    if (resorte != NULL) {
        resorte_actualizar_l(resorte, l_inicial);
    }
}

bool malla_masas_en_radio(malla_t* malla, masa_t* masa, float l0_max) {
    lista_iter_t* iter = lista_iter_crear(malla->lista_resortes);
    if (iter == NULL) {
        return false;
    }

    while (!lista_iter_al_final(iter)) {
        resorte_t* resorte = lista_iter_ver_actual(iter);
        masa_t* masa_inicio = resorte_obtener_m1(resorte);
        masa_t* masa_fin = resorte_obtener_m2(resorte);

        if (masa_inicio == masa || masa_fin == masa) {
            float longitud = norma_puntos(masa_obtener_coordx(masa_inicio), masa_obtener_coordy(masa_inicio), masa_obtener_coordx(masa_fin), masa_obtener_coordy(masa_fin));
            if (longitud > l0_max) {
                lista_iter_destruir(iter);
                return false; // Se encontró un resorte fuera del rango
            }
        }

        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return true; // Todos los resortes cumplen con el rango
}

bool malla_mover_masas(malla_t* malla, masa_t* masa, float coordx, float coordy, float l0_max) {
    bool tiene_resortes = false;

    lista_iter_t* iter = lista_iter_crear(malla->lista_resortes);
    if (iter == NULL) {
        return false;
    }

    while (!lista_iter_al_final(iter)) {
        resorte_t* resorte = lista_iter_ver_actual(iter);
        masa_t* masa_inicio = resorte_obtener_m1(resorte);
        masa_t* masa_fin = resorte_obtener_m2(resorte);

        if (masa_inicio == masa || masa_fin == masa) {
            tiene_resortes = true;
            float longitud = norma_puntos(masa_obtener_coordx(masa_inicio), masa_obtener_coordy(masa_inicio), masa_obtener_coordx(masa_fin), masa_obtener_coordy(masa_fin));
            if (longitud > l0_max) {
                lista_iter_destruir(iter);
                return false; // Se encontró un resorte fuera del rango
            }
        }

        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);

    // Si la masa no tiene resortes asociados, no hay restricciones de movimiento
    if (!tiene_resortes) {
        malla_actualizar_coord(masa, coordx, coordy);
        return true;
    }

    // La masa tiene resortes asociados, aplicar restricciones de movimiento
    float iniciox = masa_obtener_coordx(masa);
    float inicioy = masa_obtener_coordy(masa);
    
    masa_actualizar_coordx(masa, coordx);
    masa_actualizar_coordy(masa, coordy);

    if (!malla_masas_en_radio(malla, masa, L0_MAX/FACTOR_ESCALA)) {
        malla_actualizar_coord(masa, iniciox, inicioy);
        return false;
    } 
    return true;
}

lista_t* malla_obtener_lista_masas(malla_t* malla) {
    return malla->lista_masas;
}

lista_t* malla_obtener_lista_resortes(malla_t* malla) {
    return malla->lista_resortes;
}

void liberar_vector(float* vector) {
    free(vector);
}

masa_t* malla_obtener_masa_por_id(malla_t* malla, size_t id) {
    lista_iter_t* iter = lista_iter_crear(malla->lista_masas);
    if (iter == NULL) {
        return NULL;
    }
    
    masa_t* masa = NULL;
    
    while (!lista_iter_al_final(iter)) {
        masa_t* actual = lista_iter_ver_actual(iter);
        if (masa_obtener_id(actual)== id) {
            masa = actual;
            break;
        }
        lista_iter_avanzar(iter);
    }
    
    lista_iter_destruir(iter);
    return masa;
}

resorte_t* malla_obtener_resorte_por_id(malla_t* malla, size_t id) {
    lista_iter_t* iter = lista_iter_crear(malla->lista_resortes);
    if (iter == NULL) {
        return NULL;
    }
    
    resorte_t* resorte = NULL;
    
    while (!lista_iter_al_final(iter)) {
        resorte_t* actual = lista_iter_ver_actual(iter);
        if (resorte_obtener_id(actual) == id) {
            resorte = actual;
            break;
        }
        lista_iter_avanzar(iter);
    }
    
    lista_iter_destruir(iter);
    return resorte;
}

masa_t* malla_id_resorte_masa1(malla_t* malla, size_t id) {
    resorte_t* resorte = malla_obtener_resorte_por_id(malla, id);
    if (resorte == NULL) {
        return NULL;
    }
    
    return resorte_obtener_m1(resorte);
}

masa_t* malla_id_resorte_masa2(malla_t* malla, size_t id) {
    resorte_t* resorte = malla_obtener_resorte_por_id(malla, id);
    if (resorte == NULL) {
        return NULL;
    }
    
    return resorte_obtener_m2(resorte);
}

float malla_resorte_longitud_actual(malla_t* malla, size_t id) {
    resorte_t* resorte = malla_obtener_resorte_por_id(malla, id);
    if (resorte == NULL) {
        return 0.0;
    }
    
    float dx = masa_obtener_coordx(resorte_obtener_m2(resorte)) - masa_obtener_coordx(resorte_obtener_m1(resorte));
    float dy = masa_obtener_coordy(resorte_obtener_m2(resorte)) - masa_obtener_coordy(resorte_obtener_m1(resorte));
    
    return sqrt(dx * dx + dy * dy);
}

float malla_masa_obtener_coordx(masa_t* masa) {
    // Acceder a la coordenada x de la masa y retornar su valor
    return masa_obtener_coordx(masa);
}

float malla_masa_obtener_coordy(masa_t* masa) {
    // Acceder a la coordenada y de la masa y retornar su valor
    return masa_obtener_coordy(masa);
}

void copiar_malla(malla_t* malla_origen, malla_t* malla_destino) {
    // Verificar si la malla de origen y destino son válidas
    if (malla_origen == NULL || malla_destino == NULL) {
        return;
    }

    // Copiar lista de resortes
    lista_t* lista_resortes_origen = malla_obtener_lista_resortes(malla_origen);
    lista_t* lista_resortes_destino = malla_obtener_lista_resortes(malla_destino);
    lista_iter_t* iter_resortes = lista_iter_crear(lista_resortes_origen);

    while (!lista_iter_al_final(iter_resortes)) {
        resorte_t* resorte_origen = lista_iter_ver_actual(iter_resortes);
        struct masa* masa1 = resorte_obtener_m1(resorte_origen);
        struct masa* masa2 = resorte_obtener_m2(resorte_origen);
        resorte_t* resorte_copia = resorte_crear(resorte_obtener_id(resorte_origen), masa1, masa2);
        if (resorte_copia != NULL) {
            lista_insertar_ultimo(lista_resortes_destino, resorte_copia);
        }

        lista_iter_avanzar(iter_resortes);
    }

    lista_iter_destruir(iter_resortes);

    // Copiar lista de masas
    lista_t* lista_masas_origen = malla_obtener_lista_masas(malla_origen);
    lista_t* lista_masas_destino = malla_obtener_lista_masas(malla_destino);
    lista_iter_t* iter_masas = lista_iter_crear(lista_masas_origen);

    while (!lista_iter_al_final(iter_masas)) {
        masa_t* masa_origen = lista_iter_ver_actual(iter_masas);
        struct masa* masa_copia = masa_crear(masa_obtener_id(masa_origen), masa_obtener_coordx(masa_origen), masa_obtener_coordy(masa_origen), masa_obtener_tam(masa_origen));
        if (masa_copia != NULL) {
            lista_insertar_ultimo(lista_masas_destino, masa_copia);
        }

        lista_iter_avanzar(iter_masas);
    }

    lista_iter_destruir(iter_masas);
}

size_t malla_resorte_obtener_id_m1(const resorte_t* resorte) {
    if (resorte == NULL || resorte_obtener_m1(resorte)== NULL) {
        // Manejar el caso de resorte nulo o masa1 nula según corresponda
        return 0; // Por ejemplo, devolver un valor predeterminado o lanzar un error
    }

    return masa_obtener_id(resorte_obtener_m1(resorte));
}

size_t malla_resorte_obtener_id_m2(const resorte_t* resorte) {
    if (resorte == NULL || resorte_obtener_m2(resorte) == NULL) {
        // Manejar el caso de resorte nulo o masa1 nula según corresponda
        return 0; // Por ejemplo, devolver un valor predeterminado o lanzar un error
    }

    return masa_obtener_id(resorte_obtener_m2(resorte));
}

size_t malla_obtener_id_masa(const masa_t* masa) {
    // Verificar si el puntero de masa es válido
    if (masa == NULL) {
        return -1;  // Valor de ID inválido
    }

    // Obtener el ID de la masa
    size_t id_masa = masa_obtener_id(masa);

    return id_masa;
}

void reacomodar_id(struct malla *malla)
{
    if (malla == NULL)
    {
        return;
    }
    // Asignar nuevos IDs a las masas de forma creciente
    lista_iter_t *iter_masas = lista_iter_crear(malla->lista_masas);
    masa_actualizar_ids_masa(iter_masas, malla->lista_masas);
    lista_iter_destruir(iter_masas);

    // Asignar nuevos IDs a los resortes de forma creciente
    lista_iter_t *iter_resortes = lista_iter_crear(malla->lista_resortes);
    resorte_actualizar_ids_resortes(iter_resortes, malla->lista_resortes);
    lista_iter_destruir(iter_resortes);
}




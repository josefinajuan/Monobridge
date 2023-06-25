#include "masas.h"
#include "lista.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct masa{
    int id;
    int coordx;
    int coordy;
};

struct masa* masas_crear(int id, int x, int y){
    struct masa* m = malloc (sizeof(struct masa));
    if (m == NULL){
        return NULL;
    } 
    m->id = id;
    m->coordx= x;
    m->coordy = y;

    return m;
}

bool coincidir_masas(void* dato, void* extra){ 
    struct masa *m = (struct masa*) dato; 
    int x = *((int *)extra);
    int y = *((int *)(extra + sizeof(int)));

    return (m->coordx == x && m->coordy == y);
}

bool estoy_sobre_masa(int x, int y, lista_t* l){
   int coordenadas[2]= {x,y};
   lista_recorrer(l, coincidir_masas, coordenadas);
}

void masas_borrar(int x, int y, lista_t *lista_masas) {
    int coordenadas[2] = {x, y};
    lista_iter_t *lista_iter = lista_iter_crear(lista_masas);
    int nuevo_id = 1;

    while(!lista_iter_al_final(lista_iter)){
        if(coincidir_masas(lista_iter_ver_actual(lista_iter), coordenadas)) {
            struct masa *m = lista_iter_borrar(lista_iter);
            free(m);
        } else {
            struct masa *m = lista_iter_ver_actual(lista_iter);
            m->id = nuevo_id++;
            lista_iter_avanzar(lista_iter);
        }
    }

    lista_iter_destruir(lista_iter);
}

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



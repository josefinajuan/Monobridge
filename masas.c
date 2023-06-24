#include "masas.h"
#include "lista.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct masa{
    int id;
    int coordx;
    int coordy;
}

struct masa* crear_masas(int id, int x, int y){
    struct masa* m = malloc (sizeof(struct masa));
    if (m == NULL){
        return NULL;
    } 
    m->id = id;
    m->coordx= x;
    m->coordy = y;

    return m;
}






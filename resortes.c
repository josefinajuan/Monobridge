#include "resortes.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    double k; //constante de resorte
    double lo; //longitud natural del resorte
    double* x1, y1 , x2, y2; //coord x e y de masas 1 y 2
} resortes_t;

#include "resortes.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct resortes{
    double k; //Constante de resorte
    double lo; //Longitud natural del resorte
    double* x1, y1 , x2, y2; //Coord x e y de masas 1 y 2
};

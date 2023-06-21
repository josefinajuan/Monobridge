#include "malla.h"
#include "masas.h"
#include "resortes.h"
#include "lista.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    lista_t* cant_resortes;
    lista_t* resortes_totales;
    masas_t* cant_masas;
    masas_t* masas_totales;
} malla_t;

///OJOOOOOOOOOOOO ESTAMOS DEFINIENDO MAL LA ETSRUCTURA DE LA MALLA, HABRIA QUE VER COMO HACER PARA QUE CON LOS .H NOS ALCANCE PARA DEFINIRLA

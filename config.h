#ifndef CONFIG_H
#define CONFIG_H

#define VENTANA_ANCHO 800
#define VENTANA_ALTO 600

#define JUEGO_FPS 25
#define DT 0.0005
#define DURACION_SIMULACION 10.0

#define G 9.81                      // Gravedad
#define B 1.0                       // Coeficiente de amortiguación

#define L0_MAX 150                   // Máxima longitud de un resorte
#define FACTOR_ESCALA 50.0            // Conversión entre coordenadas de pantalla y mundo

#define MAXIMO_ESTIRAMIENTO 0.5    // Porcentaje de estiramiento de un resorte para perder
#define MASA_TOTAL 10.0             // Suma de todas las masas de la malla
#define K_BASE 200                  // k_resorte = K_BASE / pow(l0, POTENCIA_K)
#define POTENCIA_K 1.5
#define MARGEN_ERROR 0.25

#define TAM_MASA 20.0

#endif

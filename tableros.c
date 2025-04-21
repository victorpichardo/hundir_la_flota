#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "configuracion.h"
#include "tableros.h"

void dibujar_tablero(FILE *archivo, int tamano) {
    fprintf(archivo, "   ");
    for (int i = 0; i < tamano; i++) {
        fprintf(archivo, "%c ", 'A' + i);
    }
    fprintf(archivo, "\n");
    for (int i = 0; i < tamano; i++) {
        fprintf(archivo, "%2d ", i + 1);
        for (int j = 0; j < tamano; j++) {
            fprintf(archivo, "~ ");
        }
        fprintf(archivo, "\n");
    }
    fprintf(archivo, "\n");
}

// Función para inicializar un tablero vacío
Tablero* inicializarTablero(int tamanio) {
    Tablero* tablero = (Tablero*)malloc(sizeof(Tablero));
    tablero->tamanio = tamanio;

    // Asignar memoria para las filas
    tablero->matriz = (char**)malloc(tamanio * sizeof(char*));

    // Asignar memoria para las columnas e inicializar con espacios en blanco
    for (int i = 0; i < tamanio; i++) {
        tablero->matriz[i] = (char*)malloc(tamanio * sizeof(char));
        for (int j = 0; j < tamanio; j++) {
            tablero->matriz[i][j] = ' ';
        }
    }

    return tablero;
}

// Función para verificar si se puede colocar un barco en una posición
bool esPosicionValida(Tablero* tablero, int fila, int columna, int tamanio, char direccion) {
    int n = tablero->tamanio;

    // Verificar límites del tablero
    if (direccion == 'H' && columna + tamanio > n) return false;
    if (direccion == 'V' && fila + tamanio > n) return false;
    if (direccion == 'D' && (fila + tamanio > n || columna + tamanio > n)) return false;

    // Verificar que no haya barcos adyacentes
    for (int i = -1; i <= tamanio; i++) {
        for (int j = -1; j <= 1; j++) {
            int nuevaFila, nuevaColumna;

            if (direccion == 'H') {
                nuevaFila = fila + j;
                nuevaColumna = columna + i;
            } else if (direccion == 'V') {
                nuevaFila = fila + i;
                nuevaColumna = columna + j;
            } else { // Diagonal
                nuevaFila = fila + i;
                nuevaColumna = columna + i;
                // Verificar adyacentes en diagonal
                if (nuevaFila >= 0 && nuevaFila < n && nuevaColumna >= 0 && nuevaColumna < n) {
                    if (tablero->matriz[nuevaFila][nuevaColumna] != ' ') {
                        return false;
                    }
                }
                // Verificar adyacentes en horizontal y vertical para posición diagonal
                if (i >= 0 && i < tamanio) {
                    if (fila + i < n && columna - 1 >= 0 && tablero->matriz[fila + i][columna - 1] != ' ') return false;
                    if (fila + i < n && columna + i + 1 < n && tablero->matriz[fila + i][columna + i + 1] != ' ') return false;
                    if (fila - 1 >= 0 && columna + i < n && tablero->matriz[fila - 1][columna + i] != ' ') return false;
                    if (fila + i + 1 < n && columna + i < n && tablero->matriz[fila + i + 1][columna + i] != ' ') return false;
                }
                continue;
            }

            if (nuevaFila >= 0 && nuevaFila < n && nuevaColumna >= 0 && nuevaColumna < n) {
                if (tablero->matriz[nuevaFila][nuevaColumna] != ' ') {
                    return false;
                }
            }
        }
    }

    return true;
}

// Función para colocar un barco en el tablero
bool colocarBarco(Tablero* tablero, Barco barco, int fila, int columna, char direccion) {
    if (!esPosicionValida(tablero, fila, columna, barco.tamanio, direccion)) {
        return false;
    }

    // Colocar el barco
    for (int i = 0; i < barco.tamanio; i++) {
        if (direccion == 'H') {
            tablero->matriz[fila][columna + i] = barco.identificador;
        } else if (direccion == 'V') {
            tablero->matriz[fila + i][columna] = barco.identificador;
        } else { // Diagonal
            tablero->matriz[fila + i][columna + i] = barco.identificador;
        }
    }

    return true;
}

// Función para imprimir el tablero en pantalla
void imprimirTablero(Tablero* tablero) {
    printf("\n  ");
    for (int j = 0; j < tablero->tamanio; j++) {
        printf("%c ", 'A' + j);
    }
    printf("\n");

    for (int i = 0; i < tablero->tamanio; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < tablero->tamanio; j++) {
            printf("%c ", tablero->matriz[i][j] == ' ' ? '~' : tablero->matriz[i][j]);
        }
        printf("\n");
    }
}

// Función para guardar el tablero en un archivo
void guardarTablero(Tablero* tablero, FILE* archivo, int tamano) {
    fprintf(archivo, "   ");
    for (int j = 0; j < tamano; j++) {
        fprintf(archivo, "%c ", 'A' + j);
    }
    fprintf(archivo, "\n");

    for (int i = 0; i < tamano; i++) {
        fprintf(archivo, "%2d ", i + 1);
        for (int j = 0; j < tamano; j++) {
            char c = (tablero->matriz[i][j] == ' ') ? '~' : tablero->matriz[i][j];
            fprintf(archivo, "%c ", c);
        }
        fprintf(archivo, "\n");
    }
    fprintf(archivo, "\n");
}

// Función para colocar barcos manualmente
void colocarBarcosManual(Tablero* tablero, Barco* barcos, int numBarcos) {
    for (int i = 0; i < numBarcos; i++) {
        int fila, columna;
        char direccion;
        bool posicionValida = false;

        while (!posicionValida) {
            imprimirTablero(tablero);

            printf("\nColocando barco: %s (tamaño: %d)\n", barcos[i].nombre, barcos[i].tamanio);
            printf("Ingrese fila (1-%d): ", tablero->tamanio);
            scanf("%d", &fila);
            fila--; // Ajustar a índice base 0

            printf("Ingrese columna (A-%c): ", 'A' + tablero->tamanio - 1);
            char colChar;
            scanf(" %c", &colChar);
            columna = toupper(colChar) - 'A'; // Convertir letra a índice

            printf("Ingrese dirección (H: horizontal, V: vertical, D: diagonal): ");
            scanf(" %c", &direccion);
            direccion = toupper(direccion);

            if (direccion != 'H' && direccion != 'V' && direccion != 'D') {
                printf("Dirección inválida. Use H, V o D.\n");
                continue;
            }

            posicionValida = colocarBarco(tablero, barcos[i], fila, columna, direccion);

            if (!posicionValida) {
                printf("No se puede colocar el barco en esa posición. Intente de nuevo.\n");
            }
        }

        printf("Barco colocado exitosamente.\n");
    }
}

// Función para colocar barcos aleatoriamente
void colocarBarcosAleatorio(Tablero* tablero, Barco* barcos, int numBarcos) {
    srand(time(NULL));

    for (int i = 0; i < numBarcos; i++) {
        int fila, columna;
        char direcciones[] = {'H', 'V', 'D'};
        char direccion;
        bool posicionValida = false;

        while (!posicionValida) {
            fila = rand() % tablero->tamanio;
            columna = rand() % tablero->tamanio;
            direccion = direcciones[rand() % 3];

            posicionValida = colocarBarco(tablero, barcos[i], fila, columna, direccion);
        }
    }

    printf("Barcos colocados aleatoriamente.\n");
}

// Función para liberar la memoria del tablero
void liberarTablero(Tablero* tablero) {
    for (int i = 0; i < tablero->tamanio; i++) {
        free(tablero->matriz[i]);
    }
    free(tablero->matriz);
    free(tablero);
}

// Función para configurar tablero y colocar barcos
void configurarTablero(void) {
    // Cargar configuración desde archivo
    FILE *configFile = fopen("config.txt", "r");
    if (configFile == NULL) {
        printf("Error: No se puede abrir el archivo de configuración.\n");
        return;
    }

    char linea[100];
    fgets(linea, sizeof(linea), configFile);

    int tamanioTablero, numBarcosTotal, tiposBarcos;
    sscanf(linea, "%d-%d-%d", &tamanioTablero, &numBarcosTotal, &tiposBarcos);

    // Saltar las líneas que contienen la información del jugador 1 y sus tableros
    for (int i = 0; i < 2 * tamanioTablero + 3; i++) {
        fgets(linea, sizeof(linea), configFile);
    }

    // Cerrar el archivo
    fclose(configFile);

    // Cargar tipos de barcos disponibles desde barcos.txt
    FILE *barcosFile = fopen("barcos.txt", "r");
    if (barcosFile == NULL) {
        printf("Error: No se puede abrir el archivo de barcos.\n");
        return;
    }

    // Leer tipos de barcos disponibles
    Barco barcosDisponibles[MAX_BARCOS];
    int numBarcosDisponibles = 0;

    while (fgets(linea, sizeof(linea), barcosFile) && numBarcosDisponibles < MAX_BARCOS) {
        linea[strcspn(linea, "\n")] = 0;
        char *nombre = strtok(linea, "-");
        char *id = strtok(NULL, "-");

        if (nombre != NULL && id != NULL && strlen(id) == 1) {
            strcpy(barcosDisponibles[numBarcosDisponibles].nombre, nombre);
            barcosDisponibles[numBarcosDisponibles].identificador = id[0];
            barcosDisponibles[numBarcosDisponibles].tamanio = numBarcosDisponibles + 1; // Asignar tamaño según orden
            numBarcosDisponibles++;
        }
    }

    fclose(barcosFile);

    if (numBarcosDisponibles == 0) {
        printf("No se encontraron tipos de barcos válidos en el archivo.\n");
        return;
    }

    // Seleccionar barcos para la partida
    Barco *barcosSeleccionados = (Barco*)malloc(numBarcosTotal * sizeof(Barco));
    int barcosSeleccionadosCount = 0;

    printf("Tipos de barcos disponibles:\n");
    for (int i = 0; i < numBarcosDisponibles; i++) {
        printf("%d. %s (%c) - Tamaño: %d\n", i + 1,
               barcosDisponibles[i].nombre,
               barcosDisponibles[i].identificador,
               barcosDisponibles[i].tamanio);
    }

    printf("\nSeleccione %d barcos para la flota:\n", numBarcosTotal);

    for (int i = 0; i < numBarcosTotal; i++) {
        int seleccion;
        printf("Barco %d (1-%d): ", i + 1, numBarcosDisponibles);
        scanf("%d", &seleccion);

        if (seleccion < 1 || seleccion > numBarcosDisponibles) {
            printf("Selección inválida. Intente de nuevo.\n");
            i--;
            continue;
        }

        barcosSeleccionados[i] = barcosDisponibles[seleccion - 1];
    }

    // Inicializar tablero
    Tablero* tablero = inicializarTablero(tamanioTablero);

    // Colocar barcos
    char modoColocacion;
    printf("\n¿Colocar barcos manualmente (M) o aleatoriamente (A)? ");
    scanf(" %c", &modoColocacion);
    modoColocacion = toupper(modoColocacion);

    if (modoColocacion == 'M') {
        colocarBarcosManual(tablero, barcosSeleccionados, numBarcosTotal);
    } else {
        colocarBarcosAleatorio(tablero, barcosSeleccionados, numBarcosTotal);
    }

    // Mostrar tablero final
    printf("\nTablero con barcos colocados:\n");
    imprimirTablero(tablero);

    // Guardar tablero en el archivo
    FILE *configUpdate = fopen("config.txt", "r+");
    if (configUpdate == NULL) {
        printf("Error al abrir el archivo para actualizar.\n");
        liberarTablero(tablero);
        free(barcosSeleccionados);
        return;
    }

    // Posicionarse después de la línea del jugador 1
    fgets(linea, sizeof(linea), configUpdate); // Primera línea con configuración
    fgets(linea, sizeof(linea), configUpdate); // Línea con datos del jugador 1

    // Guardar tablero en la posición actual
    long pos = ftell(configUpdate);
    fseek(configUpdate, pos, SEEK_SET);
    guardarTablero(tablero, configUpdate, tamanioTablero);

    // Cerrar archivo
    fclose(configUpdate);

    printf("Tablero guardado correctamente.\n");

    // Liberar memoria
    liberarTablero(tablero);
    free(barcosSeleccionados);
}

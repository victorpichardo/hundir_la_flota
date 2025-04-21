#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NOMBRE 21 // 20 caracteres + '\0'
#define MAX_BARCOS 10

// Estructura para los barcos
typedef struct {
    char nombre[MAX_NOMBRE];
    char identificador;
    int tamanio;
} Barco;

// Estructura para el tablero
typedef struct {
    char** matriz;
    int tamanio;
} Tablero;

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

// Función para colocar barcos manualmente
void colocarBarcosManual(Tablero* tablero, Barco* barcos, int numBarcos) {
    for (int i = 0; i < numBarcos; i++) {
        int fila, columna;
        char direccion;
        bool posicionValida = false;

        while (!posicionValida) {
            imprimirTablero(tablero);

            printf("\nColocando barco: %s (tamaño: %d)\n", barcos[i].nombre, barcos[i].tamanio);
            printf("Ingrese fila (0-%d): ", tablero->tamanio - 1);
            scanf("%d", &fila);
            printf("Ingrese columna (0-%d): ", tablero->tamanio - 1);
            scanf("%d", &columna);
            printf("Ingrese dirección (H: horizontal, V: vertical, D: diagonal): ");
            scanf(" %c", &direccion);

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

// Función para imprimir el tablero
void imprimirTablero(Tablero* tablero) {
    printf("\n  ");
    for (int j = 0; j < tablero->tamanio; j++) {
        printf("%d ", j);
    }
    printf("\n");

    for (int i = 0; i < tablero->tamanio; i++) {
        printf("%d ", i);
        for (int j = 0; j < tablero->tamanio; j++) {
            printf("%c ", tablero->matriz[i][j]);
        }
        printf("\n");
    }
}

// Función para liberar la memoria del tablero
void liberarTablero(Tablero* tablero) {
    for (int i = 0; i < tablero->tamanio; i++) {
        free(tablero->matriz[i]);
    }
    free(tablero->matriz);
    free(tablero);
}

// Función principal para colocar barcos en el tablero
void configurarTablero() {
    int tamanioTablero;
    int numBarcos;
    char modoColocacion;

    printf("Ingrese el tamaño del tablero: ");
    scanf("%d", &tamanioTablero);

    Tablero* tablero = inicializarTablero(tamanioTablero);

    printf("Ingrese el número de barcos a colocar: ");
    scanf("%d", &numBarcos);

    Barco* barcos = (Barco*)malloc(numBarcos * sizeof(Barco));

    // Pedir información de cada barco
    for (int i = 0; i < numBarcos; i++) {
        printf("\nBarco %d:\n", i + 1);
        printf("Nombre: ");
        scanf("%s", barcos[i].nombre);
        printf("Identificador (1 carácter): ");
        scanf(" %c", &barcos[i].identificador);
        printf("Tamaño: ");
        scanf("%d", &barcos[i].tamanio);
    }

    printf("\n¿Colocar barcos manualmente (M) o aleatoriamente (A)? ");
    scanf(" %c", &modoColocacion);

    if (modoColocacion == 'M' || modoColocacion == 'm') {
        colocarBarcosManual(tablero, barcos, numBarcos);
    } else {
        colocarBarcosAleatorio(tablero, barcos, numBarcos);
    }

    printf("\nTablero con barcos colocados:\n");
    imprimirTablero(tablero);

    // Liberación de memoria
    liberarTablero(tablero);
    free(barcos);
}

int main() {
    configurarTablero();
    return 0;
}

#ifndef TABLEROS_H
#define TABLEROS_H

#include <stdio.h>
#include <stdbool.h>

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

// Funciones de manejo de tableros
void dibujar_tablero(FILE *archivo, int tamano);
Tablero* inicializarTablero(int tamanio);
bool esPosicionValida(Tablero* tablero, int fila, int columna, int tamanio, char direccion);
bool colocarBarco(Tablero* tablero, Barco barco, int fila, int columna, char direccion);
void colocarBarcosManual(Tablero* tablero, Barco* barcos, int numBarcos);
void colocarBarcosAleatorio(Tablero* tablero, Barco* barcos, int numBarcos);
void imprimirTablero(Tablero* tablero);
void liberarTablero(Tablero* tablero);
void configurarTablero(void);
void guardarTablero(Tablero* tablero, FILE* archivo, int tamano);

#endif

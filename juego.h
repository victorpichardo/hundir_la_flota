#ifndef JUEGO_H
#define JUEGO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tableros.h"

// Estructura para almacenar la configuración del juego
typedef struct {
    int tamanoTablero;
    int numBarcosTotal;
    int tiposBarcos;
} ConfiguracionJuego;

// Estructura para almacenar información de los jugadores
typedef struct {
    int id;
    char nombre[MAX_NOMBRE];
    char tipoDisparo;  // 'M' manual, 'A' automático
    int numDisparos;
    int ganador;       // 0 = no, 1 = sí
    Tablero* tableroFlota;
    Tablero* tableroOponente;
} Jugador;

// Funciones de juego
void cargarConfiguracion(ConfiguracionJuego* config, Jugador* jugador1, Jugador* jugador2);
void iniciarJuego(ConfiguracionJuego config, Jugador* jugador1, Jugador* jugador2);
bool realizarDisparo(Jugador* atacante, Jugador* defensor, int fila, int columna);
void disparoAutomatico(Jugador* atacante, Jugador* defensor);
bool esFinDeJuego(Jugador* jugador1, Jugador* jugador2);
void mostrarResumen(Jugador* jugador1, Jugador* jugador2);
void reiniciarPartida(ConfiguracionJuego config, Jugador* jugador1, Jugador* jugador2);
void jugarPartida(void);
bool verificarHundido(Jugador* defensor, char identificador, int fila, int columna);
void marcarCasillasAdyacentes(Jugador* atacante, Jugador* defensor, int fila, int columna);
void guardarEstadoJuego(ConfiguracionJuego config, Jugador* jugador1, Jugador* jugador2);
void menuJugarPartida(void);

#endif // JUEGO_H

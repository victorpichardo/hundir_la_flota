#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include "juego.h"
#include "tableros.h"
#include "configuracion.h"

// Función para cargar la configuración desde el archivo
void cargarConfiguracion(ConfiguracionJuego* config, Jugador* jugador1, Jugador* jugador2) {
    FILE* archivo = fopen("config.txt", "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo de configuración.\n");
        return;
    }

    char linea[200];

    // Leer primera línea con configuración general
    fgets(linea, sizeof(linea), archivo);
    sscanf(linea, "%d-%d-%d", &config->tamanoTablero, &config->numBarcosTotal, &config->tiposBarcos);

    // Leer datos del jugador 1
    fgets(linea, sizeof(linea), archivo);
    sscanf(linea, "%d-%[^-]-%d-%c-%d",
           &jugador1->id,
           jugador1->nombre,
           &jugador1->numDisparos,
           &jugador1->tipoDisparo,
           &jugador1->ganador);

    jugador1->tipoDisparo = tolower(jugador1->tipoDisparo);

    // Inicializar tableros del jugador 1
    jugador1->tableroFlota = inicializarTablero(config->tamanoTablero);
    jugador1->tableroOponente = inicializarTablero(config->tamanoTablero);

    // Cargar tablero Flota del jugador 1
    char letraColumna;
    int numFila;
    for (int i = 0; i < config->tamanoTablero; i++) {
        fgets(linea, sizeof(linea), archivo);
        if (i == 0) continue; // Saltar primera línea con etiquetas

        char* token = strtok(linea, " ");
        numFila = atoi(token) - 1; // Ajustar a índice base 0

        int j = 0;
        while ((token = strtok(NULL, " ")) != NULL && j < config->tamanoTablero) {
            if (token[0] != '~' && token[0] != '\n') {
                jugador1->tableroFlota->matriz[numFila][j] = token[0];
            }
            j++;
        }
    }

    // Saltar línea en blanco
    fgets(linea, sizeof(linea), archivo);

    // Cargar tablero Oponente del jugador 1
    for (int i = 0; i < config->tamanoTablero + 1; i++) {
        fgets(linea, sizeof(linea), archivo);
    }

    // Leer datos del jugador 2
    fgets(linea, sizeof(linea), archivo);
    sscanf(linea, "%d-%[^-]-%d-%c-%d",
           &jugador2->id,
           jugador2->nombre,
           &jugador2->numDisparos,
           &jugador2->tipoDisparo,
           &jugador2->ganador);

    jugador2->tipoDisparo = tolower(jugador2->tipoDisparo);

    // Inicializar tableros del jugador 2
    jugador2->tableroFlota = inicializarTablero(config->tamanoTablero);
    jugador2->tableroOponente = inicializarTablero(config->tamanoTablero);

    // Cargar tablero Flota del jugador 2
    for (int i = 0; i < config->tamanoTablero; i++) {
        fgets(linea, sizeof(linea), archivo);
        if (i == 0) continue; // Saltar primera línea con etiquetas

        char* token = strtok(linea, " ");
        numFila = atoi(token) - 1; // Ajustar a índice base 0

        int j = 0;
        while ((token = strtok(NULL, " ")) != NULL && j < config->tamanoTablero) {
            if (token[0] != '~' && token[0] != '\n') {
                jugador2->tableroFlota->matriz[numFila][j] = token[0];
            }
            j++;
        }
    }

    fclose(archivo);

    printf("Configuración cargada correctamente.\n");
}

// Función para iniciar el juego
void iniciarJuego(ConfiguracionJuego config, Jugador* jugador1, Jugador* jugador2) {
    Jugador* jugadorActual;
    Jugador* jugadorOponente;

    // Decidir quién comienza (jugador 1 por defecto)
    jugadorActual = jugador1;
    jugadorOponente = jugador2;

    printf("\n¡Comienza la partida!\n");

    // Bucle principal de juego
    while (!esFinDeJuego(jugador1, jugador2)) {
        printf("\nTurno de %s\n", jugadorActual->nombre);

        // Mostrar tableros del jugador actual
        printf("\nTu flota:\n");
        imprimirTablero(jugadorActual->tableroFlota);

        printf("\nTu registro de disparos:\n");
        imprimirTablero(jugadorActual->tableroOponente);

        bool impacto = false;

        // Realizar disparo según el tipo de jugador
        if (jugadorActual->tipoDisparo == 'm') {
            // Disparo manual
            int fila, columna;
            char columnaLetra;

            printf("\nIntroduce coordenadas para disparar:\n");
            printf("Fila (1-%d): ", config.tamanoTablero);
            scanf("%d", &fila);
            fila--; // Ajustar a índice base 0

            printf("Columna (A-%c): ", 'A' + config.tamanoTablero - 1);
            scanf(" %c", &columnaLetra);
            columna = toupper(columnaLetra) - 'A'; // Convertir letra a índice

            if (fila < 0 || fila >= config.tamanoTablero ||
                columna < 0 || columna >= config.tamanoTablero) {
                printf("\n¡Coordenadas inválidas! Intenta de nuevo.\n");
                continue;
            }

            if (jugadorActual->tableroOponente->matriz[fila][columna] != ' ') {
                printf("\n¡Ya has disparado a esa casilla! Intenta de nuevo.\n");
                continue;
            }

            impacto = realizarDisparo(jugadorActual, jugadorOponente, fila, columna);
        } else {
            // Disparo automático
            printf("\nEl sistema realizará un disparo automático...\n");
            impacto = disparoAutomatico(jugadorActual, jugadorOponente);
        }

        jugadorActual->numDisparos++;

        // Si no impactó, cambiar de turno
        if (!impacto) {
            printf("\nCambiando de turno...\n");
            Jugador* temp = jugadorActual;
            jugadorActual = jugadorOponente;
            jugadorOponente = temp;
        } else {
            printf("\n¡Impacto! %s tiene otro turno.\n", jugadorActual->nombre);
        }

        // Pausa para que el jugador lea los resultados
        printf("\nPresiona Enter para continuar...");
        getchar(); // Consumir el newline pendiente
        getchar(); // Esperar por Enter
        system("cls"); // Limpiar pantalla
    }

    // Determinar ganador
    if (jugador1->ganador && jugador2->ganador) {
        printf("\n¡Empate! Ambos jugadores han hundido la flota enemiga.\n");
    } else if (jugador1->ganador) {
        printf("\n¡%s ha ganado la partida!\n", jugador1->nombre);
    } else {
        printf("\n¡%s ha ganado la partida!\n", jugador2->nombre);
    }

    // Guardar estado final
    guardarEstadoJuego(config, jugador1, jugador2);
}

// Función para realizar un disparo
bool realizarDisparo(Jugador* atacante, Jugador* defensor, int fila, int columna) {
    char casilla = defensor->tableroFlota->matriz[fila][columna];

    printf("\nDisparo en [%d,%c]... ", fila + 1, 'A' + columna);

    if (casilla == ' ') {
        printf("¡Agua!\n");
        atacante->tableroOponente->matriz[fila][columna] = '*';
        return false;
    } else {
        // Verificar si el barco ha sido hundido
        bool hundido = verificarHundido(defensor, casilla, fila, columna);

        if (hundido) {
            printf("¡HUNDIDO!\n");
            atacante->tableroOponente->matriz[fila][columna] = 'H';

            // Marcar todo el barco como hundido
            for (int i = 0; i < defensor->tableroFlota->tamanio; i++) {
                for (int j = 0; j < defensor->tableroFlota->tamanio; j++) {
                    if (defensor->tableroFlota->matriz[i][j] == casilla) {
                        atacante->tableroOponente->matriz[i][j] = 'H';
                    }
                }
            }

            // Marcar casillas adyacentes como agua
            marcarCasillasAdyacentes(atacante, defensor, fila, columna);

            // Verificar si todos los barcos han sido hundidos
            bool todosHundidos = true;
            for (int i = 0; i < defensor->tableroFlota->tamanio && todosHundidos; i++) {
                for (int j = 0; j < defensor->tableroFlota->tamanio && todosHundidos; j++) {
                    if (defensor->tableroFlota->matriz[i][j] != ' ' &&
                        atacante->tableroOponente->matriz[i][j] != 'H') {
                        todosHundidos = false;
                    }
                }
            }

            if (todosHundidos) {
                atacante->ganador = 1;
            }
        } else {
            printf("¡TOCADO!\n");
            atacante->tableroOponente->matriz[fila][columna] = 'T';
        }

        return true;
    }
}

// Función para verificar si un barco ha sido hundido
bool verificarHundido(Jugador* defensor, char identificador, int fila, int columna) {
    // Buscar todas las casillas con el mismo identificador
    for (int i = 0; i < defensor->tableroFlota->tamanio; i++) {
        for (int j = 0; j < defensor->tableroFlota->tamanio; j++) {
            // Si hay alguna casilla del barco que no ha sido atacada, el barco no está hundido
            if (defensor->tableroFlota->matriz[i][j] == identificador &&
                (i != fila || j != columna)) {
                return false;
            }
        }
    }

    return true;
}

// Función para marcar casillas adyacentes como agua al hundir un barco
void marcarCasillasAdyacentes(Jugador* atacante, Jugador* defensor, int fila, int columna) {
    char identificador = defensor->tableroFlota->matriz[fila][columna];
    int n = defensor->tableroFlota->tamanio;

    // Encontrar todas las casillas del barco
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (defensor->tableroFlota->matriz[i][j] == identificador) {
                // Marcar casillas adyacentes como agua
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        int ni = i + di;
                        int nj = j + dj;

                        if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                            // Si la casilla adyacente está vacía en el tablero del defensor
                            // y no está marcada en el tablero del atacante
                            if (defensor->tableroFlota->matriz[ni][nj] != identificador &&
                                atacante->tableroOponente->matriz[ni][nj] == ' ') {
                                atacante->tableroOponente->matriz[ni][nj] = '*';
                            }
                        }
                    }
                }
            }
        }
    }
}

// Función para realizar un disparo automático
bool disparoAutomatico(Jugador* atacante, Jugador* defensor) {
    int n = atacante->tableroOponente->tamanio;
    int fila, columna;
    bool casillaValida = false;

    // Primero buscar casillas con barcos tocados pero no hundidos
    for (int i = 0; i < n && !casillaValida; i++) {
        for (int j = 0; j < n && !casillaValida; j++) {
            if (atacante->tableroOponente->matriz[i][j] == 'T') {
                // Buscar casillas adyacentes no disparadas
                int direcciones[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // derecha, abajo, izquierda, arriba

                for (int d = 0; d < 4 && !casillaValida; d++) {
                    int ni = i + direcciones[d][0];
                    int nj = j + direcciones[d][1];

                    if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                        if (atacante->tableroOponente->matriz[ni][nj] == ' ') {
                            fila = ni;
                            columna = nj;
                            casillaValida = true;
                        }
                    }
                }
            }
        }
    }

    // Si no hay casillas tocadas, elegir una casilla al azar
    while (!casillaValida) {
        fila = rand() % n;
        columna = rand() % n;

        if (atacante->tableroOponente->matriz[fila][columna] == ' ') {
            casillaValida = true;
        }
    }

    return realizarDisparo(atacante, defensor, fila, columna);
}

// Función para verificar si el juego ha terminado
bool esFinDeJuego(Jugador* jugador1, Jugador* jugador2) {
    return jugador1->ganador || jugador2->ganador;
}

// Función para mostrar el resumen de la partida
void mostrarResumen(Jugador* jugador1, Jugador* jugador2) {
    printf("\n===== RESUMEN DE LA PARTIDA =====\n");
    printf("Jugador: %s\n", jugador1->nombre);
    printf("Disparos realizados: %d\n", jugador1->numDisparos);
    printf("Estado: %s\n\n", jugador1->ganador ? "GANADOR" : "PERDEDOR");

    printf("Jugador: %s\n", jugador2->nombre);
    printf("Disparos realizados: %d\n", jugador2->numDisparos);
    printf("Estado: %s\n", jugador2->ganador ? "GANADOR" : "PERDEDOR");
    printf("===============================\n");
}

// Función para reiniciar la partida
void reiniciarPartida(ConfiguracionJuego config, Jugador* jugador1, Jugador* jugador2) {
    // Reiniciar contadores
    jugador1->numDisparos = 0;
    jugador1->ganador = 0;
    jugador2->numDisparos = 0;
    jugador2->ganador = 0;

    // Reiniciar tablero oponente (disparos)
    for (int i = 0; i < config.tamanoTablero; i++) {
        for (int j = 0; j < config.tamanoTablero; j++) {
            jugador1->tableroOponente->matriz[i][j] = ' ';
            jugador2->tableroOponente->matriz[i][j] = ' ';
        }
    }

    // Guardar estado inicial
    guardarEstadoJuego(config, jugador1, jugador2);

    printf("\nPartida reiniciada correctamente.\n");
}

// Función para guardar el estado actual del juego
void guardarEstadoJuego(ConfiguracionJuego config, Jugador* jugador1, Jugador* jugador2) {
    FILE* archivo = fopen("config.txt", "w");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo para guardar el estado.\n");
        return;
    }

    // Guardar configuración general
    fprintf(archivo, "%d-%d-%d\n", config.tamanoTablero, config.numBarcosTotal, config.tiposBarcos);

    // Guardar datos del jugador 1
    fprintf(archivo, "%d-%s-%d-%c-%d\n",
           jugador1->id,
           jugador1->nombre,
           jugador1->numDisparos,
           jugador1->tipoDisparo,
           jugador1->ganador);

    // Guardar tablero Flota del jugador 1
    guardarTablero(jugador1->tableroFlota, archivo, config.tamanoTablero);

    // Guardar tablero Oponente del jugador 1
    guardarTablero(jugador1->tableroOponente, archivo, config.tamanoTablero);

    // Guardar datos del jugador 2
    fprintf(archivo, "%d-%s-%d-%c-%d\n",
           jugador2->id,
           jugador2->nombre,
           jugador2->numDisparos,
           jugador2->tipoDisparo,
           jugador2->ganador);

    // Guardar tablero Flota del jugador 2
    guardarTablero(jugador2->tableroFlota, archivo, config.tamanoTablero);

    // Guardar tablero Oponente del jugador 2
    guardarTablero(jugador2->tableroOponente, archivo, config.tamanoTablero);

    fclose(archivo);
}

// Función principal para jugar partida
void jugarPartida(void) {
    ConfiguracionJuego config;
    Jugador jugador1, jugador2;

    // Cargar configuración y datos de los jugadores
    cargarConfiguracion(&config, &jugador1, &jugador2);

    // Iniciar juego
    iniciarJuego(config, &jugador1, &jugador2);

    // Liberar memoria
    liberarTablero(jugador1.tableroFlota);
    liberarTablero(jugador1.tableroOponente);
    liberarTablero(jugador2.tableroFlota);
    liberarTablero(jugador2.tableroOponente);
}

// Función para el menú de jugar partida
void menuJugarPartida(void) {
    int opcion;
    ConfiguracionJuego config;
    Jugador jugador1, jugador2;
    bool configCargada = false;

    do {
        printf("\nMenu:\n");
        printf("Jugar partida\n");
        printf("1. Jugar partida\n");
        printf("2. Reiniciar partida\n");
        printf("3. Resumen\n");
        printf("4. Volver\n");
        printf("Opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                if (!configCargada) {
                    cargarConfiguracion(&config, &jugador1, &jugador2);
                    configCargada = true;
                }
                iniciarJuego(config, &jugador1, &jugador2);
                break;

            case 2:
                if (!configCargada) {
                    cargarConfiguracion(&config, &jugador1, &jugador2);
                    configCargada = true;
                }
                reiniciarPartida(config, &jugador1, &jugador2);
                break;

            case 3:
                if (!configCargada) {
                    cargarConfiguracion(&config, &jugador1, &jugador2);
                    configCargada = true;
                }
                mostrarResumen(&jugador1, &jugador2);
                break;

            case 4:
                printf("Volviendo al menú principal...\n");
                if (configCargada) {
                    liberarTablero(jugador1.tableroFlota);
                    liberarTablero(jugador1.tableroOponente);
                    liberarTablero(jugador2.tableroFlota);
                    liberarTablero(jugador2.tableroOponente);
                }
                break;

            default:
                printf("Opción no válida.\n");
                break;
        }
    } while (opcion != 4);
}

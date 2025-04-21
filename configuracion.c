#include "configuracion.h"
#include <string.h>
#include "tableros.h"

void introducir_datos(void) {
    char nombre1[100];
    char nombre2[100];
    char disparo1[100];
    char disparo2[100];
    int tamano_tablero;
    int total_barcos;
    int tipos_barcos;
    char linea[100];
    int tipo_disponibles = 0;
    char identificadores[4];
    int cantidades[4] = {0};
    int suma = 0;

    FILE *fb = fopen("barcos.txt", "r");
    if (fb == NULL) {
        printf("No se pudo abrir barcos.txt\n");
        return;
    }
    while (fgets(linea, sizeof(linea), fb) && tipo_disponibles < 4) {
        linea[strcspn(linea, "\n")] = 0;
        char *nombre = strtok(linea, "-");
        char *id = strtok(NULL, "-");

        if (nombre != NULL && id != NULL && strlen(id) == 1) {
            identificadores[tipo_disponibles] = id[0];
            tipo_disponibles++;
        }
    }

    fclose(fb);

    if (tipo_disponibles == 0) {
        printf("No se encontraron tipos de barcos válidos en el archivo.\n");
        return;
    }

    printf("introduce el tamano de los tableros ");
    scanf("%d",&tamano_tablero);
    printf("introduce el numero de barcos total en la flota ");
    scanf("%d", &total_barcos);
    printf("introduce el numero total de tipos de barcos ");
    scanf("%d", &tipos_barcos);
    printf("introduce el nombre del primer jugador (poner _ entre nombre y apellido de lo controrario solo leera el nombre): ");
    scanf("%99s", nombre1);
    printf("introduce el tipo de disparo del primer jugador (m/a): ");
    scanf("%99s", disparo1);
    printf("introduce el nombre del segundo jugador (poner _ entre nombre y apellido de lo controrario solo leera el nombre): ");
    scanf("%99s", nombre2);
    printf("introduce el tipo de disparo del segundo jugador (m/a): ");
    scanf("%99s", disparo2);

    if (tipos_barcos > tipo_disponibles || tipos_barcos < 1) {
        printf("Número de tipos inválido.\n");
        return;
    }
    for (int i = 0; i < tipos_barcos; i++) {
        int cantidad;
        printf("¿Cuántos barcos del tipo '%c' quieres?: ", identificadores[i]);
        scanf("%d", &cantidad);

        if (suma + cantidad > total_barcos) {
            printf("¡Te has pasado del total de barcos disponibles!\n");
            return;
        }

        cantidades[i] = cantidad;
        suma += cantidad;
    }

    if (suma != total_barcos) {
        printf("Advertencia: la suma total es %d, no coincide con %d barcos esperados.\n", suma, total_barcos);
        return;
    }

    FILE *f = fopen("config.txt", "w");
    if (f == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    fprintf(f, "%d-%d-%d\n", tamano_tablero,total_barcos,tipos_barcos);
    fprintf(f, "1-%s-0-%s-0\n", nombre1,disparo1);
    dibujar_tablero(f, tamano_tablero);
    dibujar_tablero(f, tamano_tablero);
    fprintf(f, "2-%s-0-%s-0\n", nombre2,disparo2);
    dibujar_tablero(f, tamano_tablero);
    dibujar_tablero(f, tamano_tablero);
    printf("Datos guardados en config.txt.\n");
    fclose(f);

    printf("¿Desea configurar los tableros ahora? (s/n): ");
    char respuesta;
    scanf(" %c", &respuesta);

    if (respuesta == 's' || respuesta == 'S') {
        configurarTablero();
    }
}

void configuracion_(void) {
    int opciones;

    printf("Introduzca lo que quiere configurar:\n");
    printf("1. Introducir datos\n");
    printf("2. Cargar datos\n");
    printf("3. Configurar tableros\n");
    printf("Opción: ");
    scanf("%d", &opciones);

    switch (opciones) {
        case 1:
            introducir_datos();
            break;
        case 2:
            printf("Función de cargar datos aún no implementada.\n");
            break;
        case 3:
            configurarTablero();
            break;
        default:
            printf("Opción no válida.\n");
            break;
    }
}

#include <stdio.h>      // printf, scanf
#include <stdlib.h>     // system
#include <windows.h>    // Sleep
#include "configuracion.h"
#include "menu.h"

// Función para limpiar pantalla
void limpiarPantalla() {
    system("cls");  // Funciona en Windows
}

// Función de menú principal


// Función principal
int main() {
int opcion;

    titulo();
    Sleep(3000);
    limpiarPantalla();
    opcion = menuPrincipal();

    switch(opcion) {
        case 1:
           configuracion_();
            break;
        case 2:
            printf("Has seleccionado Jugar\n");
            break;
        case 3:
            printf("Saliendo del juego...\n");
            break;
        default:
            printf("Opcion no valida\n");
    }

    return 0;
}

#include <stdio.h>

void mostrarTitulo() {
    printf("                          *******************************************************\n");
    printf("                          *                                                     *\n");
    printf("                          *    H   H   U   U   N     N   DDDD   III   RRRR      *\n");
    printf("                          *    H   H   U   U   N N   N   D   D   I    R   R     *\n");
    printf("                          *    HHHHH   U   U   N  N  N   D   D   I    RRRR      *\n");
    printf("                          *    H   H   U   U   N   N N   D   D   I    R  R      *\n");
    printf("                          *    H   H   UUUU    N     N   DDDD   III   R   R     *\n");
    printf("                          *                                                     *\n");
    printf("                          *    LL       AAAAA                                   *\n");
    printf("                          *    LL      A     A                                  *\n");
    printf("                          *    LL      AAAAAAA                                  *\n");
    printf("                          *    LL      A     A                                  *\n");
    printf("                          *    LLLLL   A     A                                  *\n");
    printf("                          *                                                     *\n");
    printf("                          *    FFFFF   LL      OOOO  TTTTTT  AAAAA              *\n");
    printf("                          *    FF      LL     OO  OO   TT   A     A             *\n");
    printf("                          *    FFFF    LL     OO  OO   TT   AAAAAAA             *\n");
    printf("                          *    FF      LL     OO  OO   TT   A     A             *\n");
    printf("                          *    FF      LLLLL   OOOO    TT   A     A             *\n");
    printf("                          *                                                     *\n");
    printf("                          *******************************************************\n");
}

int main() {
    mostrarTitulo();  // Llamada para mostrar el título antes del menú principal

    int opcion;

    while (1) {
        printf("\nMenu Principal:\n");
        printf("1. Iniciar Juego\n");
        printf("2. Configurar Juego\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Iniciando el juego... (aquí iría la lógica del juego)\n");
                break;
            case 2:
                printf("Accediendo a la configuración...\n");
                break;
            case 3:
                printf("Saliendo del juego. ¡Hasta luego!\n");
                return 0;
            default:
                printf("Opción no válida, intenta de nuevo.\n");
                break;
        }
    }

    return 0;
}


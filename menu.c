#include <stdio.h>
#include <stdlib.h>

void titulo(){
  printf("\n");
  printf("\n");
  printf("\n");
  printf("                             ========================================================\n");
  printf("                             =  HH   HH  UUU   UUU  NNN    NN  DDDDD   II  RRRRR    =\n");
  printf("                             =  HH   HH  UUU   UUU  NNNN   NN  DD   D  II  RR   R   =\n");
  printf("                             =  HHHHHHH  UUU   UUU  NN NN  NN  DD   D  II  RR   R   =\n");
  printf("                             =  HHHHHHH  UUU   UUU  NN NN  NN  DD   D  II  RRRRRR   =\n");
  printf("                             =  HH   HH  UUUUUUUUU  NN  NN NN  DD   D  II  RR  RR   =\n");
  printf("                             =  HH   HH   UUUUUUU   NN   NNNN  DDDDD   II  RR   RR  =\n");
  printf("                             =                                                      =\n");
  printf("                             =                   LL        AAAAA                    =\n");
  printf("                             =                   LL       AA   AA                   =\n");
  printf("                             =                   LL       AAAAAAA                   =\n");
  printf("                             =                   LL       AA   AA                   =\n");
  printf("                             =                   LLLLLLL  AA   AA                   =\n");
  printf("                             =                                                      =\n");
  printf("                             =    FFFFFF  LL         OOOOOO    TTTTTTTTT  AAAAA     =\n");
  printf("                             =    FF      LL        OO    OO      TTT    AA   AA    =\n");
  printf("                             =    FFFF    LL        OO    OO      TTT    AAAAAAA    =\n");
  printf("                             =    FF      LL        OO    OO      TTT    AA   AA    =\n");
  printf("                             =    FF      LLLLLLL    OOOOOO       TTT    AA   AA    =\n");
  printf("                             ========================================================\n");
}

int menuPrincipal() {
   int opcion;
    printf("\n");
    printf("                             ====================================================\n");
    printf("                             =                                                  =\n");
    printf("                             =                    Menu:                         =\n");
    printf("                             =                Hundir la flota                   =\n");
    printf("                             =                                                  =\n");
    printf("                             ====================================================\n");
    printf("                             =                                                  =\n");
    printf("                             =    1. Configuracion                              =\n");
    printf("                             =    2. Jugar                                      =\n");
    printf("                             =    3. Salir                                      =\n");
    printf("                             =                                                  =\n");
    printf("                             ====================================================\n");
    printf("\n");
    printf("                             Seleccione una opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

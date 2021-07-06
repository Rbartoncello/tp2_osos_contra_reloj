#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test_de_personalidad.h"
#include "osos_contra_reloj.h"
#include "utiles.h"

const int CLOE_NO_ENCONTRADA = 0;
const int MAX_TIEMPO_PARA_GANAR = 120;

void mensaje_ingresar_jugada(char* jugada){
	printf("\nIngrese la jugada que desee realizar\n");
	scanf(" %c", jugada);
	while (!es_jugada_valida(*jugada)){
		printf("Tecla ingresada no valida, por favor una jugada valida.");
		scanf(" %c", jugada);
	}
}

int main(){
    juego_t juego;
	char jugada = ' ';
    char tipo_personaje = ' ';
    
    test_de_personalidad(&tipo_personaje);

    srand (( unsigned)time(NULL));

    inicializar_juego(&juego, tipo_personaje);
    mostrar_juego(juego);
	iniciar_cronometro();

	while (estado_juego(juego) == CLOE_NO_ENCONTRADA){
        mensaje_ingresar_jugada(&jugada);
        realizar_jugada(&juego, jugada);
    }
    system("clear");
    if ((tiempo_actual() + juego.personaje.tiempo_perdido) <= MAX_TIEMPO_PARA_GANAR){
        printf("\
  #####      #     #     #     #      #####   #######  #######  ###  ###\n\
 #     #    # #    ##    #    # #    #     #     #     #        ###  ###\n\
 #         #   #   # #   #   #   #   #           #     #        ###  ###\n\
 #  ####  #     #  #  #  #  #     #   #####      #     #####     #    # \n\
 #     #  #######  #   # #  #######        #     #     #                \n\
 #     #  #     #  #    ##  #     #  #     #     #     #        ###  ###\n\
  #####   #     #  #     #  #     #   #####      #     #######  ###  ###\n");
    } else {
        printf("\
.########..########.########..########..####..######..########.########....####.####\n\
.##.....##.##.......##.....##.##.....##..##..##....##....##....##..........####.####\n\
.##.....##.##.......##.....##.##.....##..##..##..........##....##..........####.####\n\
.########..######...########..##.....##..##...######.....##....######.......##...##.\n\
.##........##.......##...##...##.....##..##........##....##....##...................\n\
.##........##.......##....##..##.....##..##..##....##....##....##..........####.####\n\
.##........########.##.....##.########..####..######.....##....########....####.####\n");
    }
    printf("Finalizaste el juego en:  %.0f segundos.\n", (tiempo_actual() + juego.personaje.tiempo_perdido));
    detener_cronometro();
    
	

	return 0;
}
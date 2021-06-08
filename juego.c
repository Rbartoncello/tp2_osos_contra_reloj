#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test_de_personalidad.h"
#include "osos_contra_reloj.h"
#include "utiles.h"

const int CLOE_NO_ENCONTRADA = 0;
const int MAX_TIEMPO_PARA_GANAR = 0; 

int main(){

    juego_t juego;
	char jugada = ' ';
    char tipo_personaje = ' ';
    
    test_de_personalidad(&tipo_personaje);

    srand (( unsigned)time(NULL));

    inicializar_juego(&juego, tipo_personaje);
	iniciar_cronometro();

	while (estado_juego(juego) == CLOE_NO_ENCONTRADA){
        realizar_jugada(&juego, jugada);
    }
    detener_cronometro();
    if (tiempo_actual() <= MAX_TIEMPO_PARA_GANAR){
        printf("GANASTE!!!!\nFinalizates el juego en:  %.0f segundos.\n", tiempo_actual());
    } else {
        printf("Perdiste :´( \nFinalizates el juego en:  %.0f segundos.\n", tiempo_actual());
    }
	

	return 0;
}
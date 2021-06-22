#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "test_de_personalidad.h"

#define CANAL_TV_ANIME 'A'
#define CANAL_TV_MUSICA 'M'
#define CANAL_TV_LIMPIEZA 'L'

#define COMIDA_BAMBO 'B'
#define COMIDA_PEZCADO 'P'
#define COMIDA_FOCAS 'F'

const int MIN_PISO_EDIFICIO = 1;
const int MAX_PISO_EDIFICIO = 18;

const int MIN_PODER_GRITO = 1;
const int MAX_PODER_GRITO = 18;


const int PUNTAJE_CANAL_TV_LIMPIEZA = 1;
const int PUNTAJE_CANAL_TV_ANIME = 2;
const int PUNTAJE_CANAL_TV_MUSICA = 3;

const int PUNTAJE_COMIDA_FOCAS = 3;
const int PUNTAJE_COMIDA_BAMBO = 6;
const int PUNTAJE_COMIDA_PEZCADO = 9;


const int PUNTAJE_MIN_OSO_POLAR = 5;
const int PUNTAJE_MAX_OSO_POLAR = 24;

const int PUNTAJE_MIN_OSO_PANDA = 25;
const int PUNTAJE_MAX_OSO_PANDA = 43;

const int PUNTAJE_MIN_OSO_PARDO = 44;
const int PUNTAJE_MAX_OSO_PARDO = 63;

const char POLAR = 'I';
const char PANDA = 'P';
const char PARDO = 'G';


void mensaje_inicio_programa(){
	printf("\
########  #### ######## ##    ## ##     ## ######## ##    ## #### ########   #######        ###    ##                ## ##     ## ########  ######    #######     ########  ########  ##  \n\
##     ##  ##  ##       ###   ## ##     ## ##       ###   ##  ##  ##     ## ##     ##      ## ##   ##                ## ##     ## ##       ##    ##  ##     ##    ##     ## ##       #### \n\
##     ##  ##  ##       ####  ## ##     ## ##       ####  ##  ##  ##     ## ##     ##     ##   ##  ##                ## ##     ## ##       ##        ##     ##    ##     ## ##        ##  \n\
########   ##  ######   ## ## ## ##     ## ######   ## ## ##  ##  ##     ## ##     ##    ##     ## ##                ## ##     ## ######   ##   #### ##     ##    ##     ## ######        \n\
##     ##  ##  ##       ##  ####  ##   ##  ##       ##  ####  ##  ##     ## ##     ##    ######### ##          ##    ## ##     ## ##       ##    ##  ##     ##    ##     ## ##        ##  \n\
##     ##  ##  ##       ##   ###   ## ##   ##       ##   ###  ##  ##     ## ##     ##    ##     ## ##          ##    ## ##     ## ##       ##    ##  ##     ##    ##     ## ##       #### \n\
########  #### ######## ##    ##    ###    ######## ##    ## #### ########   #######     ##     ## ########     ######   #######  ########  ######    #######     ########  ########  ##  \n");
	printf("\
 \t _                   ___                              _          _                                             _                                _           _ \n\
 \t| |     ___   ___   | __|  ___  __   __ _   _ _    __| |  __ _  | |  ___   ___  ___   ___    __   ___   _ _   | |_   _ _   __ _     _ _   ___  | |  ___    (_)\n\
 \t| |__  / _ \\ (_-<   | _|  (_-< / _| / _` | | ' \\  / _` | / _` | | | / _ \\ (_-< / _ \\ (_-<   / _| / _ \\ | ' \\  |  _| | '_| / _` |   | '_| / -_) | | / _ \\   | |\n\
 \t|____| \\___/ /__/   |___| /__/ \\__| \\__,_| |_||_| \\__,_| \\__,_| |_| \\___/ /__/ \\___/ /__/   \\__| \\___/ |_||_|  \\__| |_|   \\__,_|   |_|   \\___| |_| \\___/  _/ |\n\
 \t                                                                                                                                                         |__/ \n");
	printf ("\n	¦Antes de empezar a jugar al juego de Los Escandalosos contra reloj© se le hara un test de personalidad para saber que tipo de escandaloso es¦\n\n");
	printf("\n[Aguarde 7 segundos para que comience el juego de Los Escandalosos contra reloj©]\n");
	sleep(7);
	system("clear");
}

/*
 * Pre: -
 * Post: Devolvera TRUE si el char ingresado para canal_tv es CANAL_TV_ANIME o CANAL_TV_MUSICA o  CANAL_TV_LIMPIEZA 
 *	o FALSE si no.
 */
bool es_canal_tv_valido(char canal_tv_letra){
	return(canal_tv_letra == CANAL_TV_ANIME || canal_tv_letra == CANAL_TV_MUSICA || canal_tv_letra == CANAL_TV_LIMPIEZA);
}
/*
 * Pre: -
 * Post: Devolvera TRUE si el char ingresado para tipo_comida es COMIDA_BAMBO o COMIDA_PEZCADO o  COMIDA_FOCAS 
 *	o FALSE si no.
 */
bool es_comida_valida(char comida_letra){
	return(comida_letra == COMIDA_BAMBO || comida_letra == COMIDA_PEZCADO || comida_letra == COMIDA_FOCAS);
}
/*
 * Pre: -
 * Post: Devolvera TRUE si el numero ingresado entre MIN_PISO_EDIFICIO y MAX_PISO_EDIFICIO o FALSE si no.
 */
bool es_numero_piso_edificio_valido(int numero_piso_edificio){
	return(numero_piso_edificio >= MIN_PISO_EDIFICIO && numero_piso_edificio <= MAX_PISO_EDIFICIO);
}
/*
 * Pre: -
 * Post: Devolvera TRUE si el numero ingresado entre MIN_PODER_GRITO y MAX_PODER_GRITO o FALSE si no.
 */
bool es_grito_puntaje_valido(int grito_puntaje){
	return(grito_puntaje >= MIN_PODER_GRITO && grito_puntaje <= MAX_PODER_GRITO);
}
/*
 * Pre: -
 * Post: Imprimira un mensaje al usuario para que ingrese el char que le corresponde a canal_tv
 		 Tomara el valor ingrasado por el usuario
 */
void mensaje_selecion_canal_tv(char* canal_tv_seleccion){
	printf("Vas a ver televisión un rato, pones el canal de:\n1. Anime (Ingrese la tecla '%c' y toque Enter)\n2. Musica Pop (Ingrese la tecla '%c' y toque Enter)\n3. Limpieza (Ingrese la tecla '%c' y toque Enter)\n", CANAL_TV_ANIME, CANAL_TV_MUSICA, CANAL_TV_LIMPIEZA);
	scanf("%c", canal_tv_seleccion);

	while (!es_canal_tv_valido(*canal_tv_seleccion)){
		
		printf("ERROR!!!.\nNo quiero ser malo pero el dato incresado NO es valido. Ponete la 10 e ingresa:\n1. Anime (%c)\n2. Musica Pop (%c)\n3. Limpieza (%c)\n", CANAL_TV_ANIME, CANAL_TV_MUSICA, CANAL_TV_LIMPIEZA);
		scanf(" %c", canal_tv_seleccion);
	}
}
/*
 * Pre: -
 * Post: Imprimira un mensaje al usuario para que ingrese el char que le corresponde a tipo_comida
 		 Tomara el valor ingrasado por el usuario
 */
void mensaje_selecion_tipo_comida(char* tipo_comida_selection){
	printf("Solo podes guardar un alimento en tu vianda:\n1. Bambú (Ingrese la tecla '%c' y toque Enter)\n2. Pescado (Ingrese la tecla '%c' y toque Enter)\n3. Focas (Ingrese la tecla '%c' y toque Enter)\n", COMIDA_BAMBO, COMIDA_PEZCADO, COMIDA_FOCAS);
	scanf(" %c", tipo_comida_selection);

	while (!es_comida_valida(*tipo_comida_selection)){
		
		printf("ERROR!!!.\nNo quiero ser malo pero el dato incresado NO es valido. Ponete la 10  ingresa:\n1. Bambú (%c)\n2. Pescado (%c)\n3. Focas (%c)\n", COMIDA_BAMBO, COMIDA_PEZCADO, COMIDA_FOCAS);
		scanf(" %c", tipo_comida_selection);
	}
}
/*
 * Pre: -
 * Post: Imprimira un mensaje al usuario para que ingrese el numero que le corresponde a numero_piso_edificio
 		 Tomara el valor ingrasado por el usuario
 */
void mensaje_selecion_piso_edificio(int* numero_piso_edificio_selection){
	printf("Encontras la olla con el oro del duende maldito y te compras una torre con tus dos hermanos de %i pisos.\n¿En que piso te gustaría vivir?: \n[Ingrese un numero del 1 al %i y toque Enter] ", MAX_PISO_EDIFICIO, MAX_PISO_EDIFICIO);
	scanf("%i", numero_piso_edificio_selection);

	while (!es_numero_piso_edificio_valido(*numero_piso_edificio_selection)){
		
		printf("ERROR!!!.\nNo quiero ser malo pero el dato incresado NO es valido. Ponete la 10  ingresa un numero entre %i y %i: ", MIN_PISO_EDIFICIO, MAX_PISO_EDIFICIO);
		scanf("%i", numero_piso_edificio_selection);
	}
}
/*
 * Pre: -
 * Post: Imprimira un mensaje al usuario para que ingrese el numero que le corresponde a nivel_grito
         Tomara el valor ingrasado por el usuario
 */
void mensaje_selecion_nivel_grito(int* nivel_grito_selection){
	printf("¡Oh, el duende maldito se entero que le robaste su oro y viene por ti!\n¿Que tan fuerte gritas del %i al %i? Siendo %i no gritar y %i desgarrarse la garganta:\n[Ingrese un numero del %i al %i y toque Enter] ", MIN_PODER_GRITO, MAX_PODER_GRITO, MIN_PODER_GRITO, MAX_PODER_GRITO, MIN_PODER_GRITO, MAX_PODER_GRITO);
	scanf("%i", nivel_grito_selection);

	while (!es_grito_puntaje_valido(*nivel_grito_selection)){
		
		printf("ERROR!!!.\nNo quiero ser malo pero el dato incresado NO es valido. Ponete la 10  ingresa un numero entre %i y %i: ", MIN_PODER_GRITO, MAX_PODER_GRITO);
		scanf("%i", nivel_grito_selection);
	}
}
/*
 * Pre: Necesito tener el char correspondiente a canal_tv, el cual fue ingresado por el usuario
 * Post: Devolvera el valor numero que corresponde a canal_tv
 */
int puntaje_canal_tv(char canal_tv_letra){

	int puntaje = 0;

	switch(canal_tv_letra){
		case CANAL_TV_ANIME:
		puntaje = PUNTAJE_CANAL_TV_ANIME;
		break;
		case CANAL_TV_MUSICA:
		puntaje = PUNTAJE_CANAL_TV_MUSICA;
		break; 
		case CANAL_TV_LIMPIEZA:
		puntaje = PUNTAJE_CANAL_TV_LIMPIEZA;
		break;
	}

	return puntaje;
}
/*
 * Pre: Necesito tener el char correspondiente a tipo_comida, el cual fue ingresado por el usuario
 * Post: Devolvera el valor numero que corresponde a tipo_comida
 */
int puntaje_tipo_comida(char comida_letra){

	int puntaje = 0;

	switch(comida_letra){
		case COMIDA_BAMBO:
		puntaje = PUNTAJE_COMIDA_BAMBO;
		break;
		case COMIDA_PEZCADO:
		puntaje = PUNTAJE_COMIDA_PEZCADO;
		break;
		case COMIDA_FOCAS:
		puntaje = PUNTAJE_COMIDA_FOCAS;
		break;
	}

	return puntaje;
}
/*
 * Pre: Necesito tener todos los datos solicitados de forma numerica las cuales tiene que representar su respectivo puntaje > 0
 * Post: Devolvera el calculo del total [(t * f) + b + s] de todos los elememtos solicitados
 */
int calculo_puntaje_total(int puntaje_canal_tv, int puntaje_tipo_comida, int numero_piso_edificio_puntaje, int grito_puntaje){
		return (puntaje_canal_tv * puntaje_tipo_comida) + numero_piso_edificio_puntaje + grito_puntaje;

}

/*
 * Pre: -
 * Post: Devolvera el tipo de oso que es
 */
void tipo_oso(int puntaje, char* tipo_oso){
	if (puntaje >= PUNTAJE_MIN_OSO_POLAR && puntaje <= PUNTAJE_MAX_OSO_POLAR){
		*tipo_oso = POLAR;
	} else if (puntaje >= PUNTAJE_MIN_OSO_PANDA && puntaje <= PUNTAJE_MAX_OSO_PANDA){
		*tipo_oso = PANDA;
	} else if (puntaje >= PUNTAJE_MIN_OSO_PARDO && puntaje <= PUNTAJE_MAX_OSO_PARDO){
		*tipo_oso = PARDO;
	}
}

void test_de_personalidad(char* personalidad_detectada ){
	
	char canal_tv, tipo_comida;
	int numero_piso_edificio, nivel_grito;
	int puntaje = 0;
	

	mensaje_inicio_programa();

	mensaje_selecion_canal_tv(&canal_tv);
	mensaje_selecion_tipo_comida(&tipo_comida);
	mensaje_selecion_piso_edificio(&numero_piso_edificio);
	mensaje_selecion_nivel_grito(&nivel_grito);
	
	puntaje = calculo_puntaje_total(puntaje_canal_tv(canal_tv), puntaje_tipo_comida(tipo_comida), numero_piso_edificio, nivel_grito);

	system("clear");

	tipo_oso(puntaje, personalidad_detectada);

};
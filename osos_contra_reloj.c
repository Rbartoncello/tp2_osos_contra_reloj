#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test_de_personalidad.h"
#include "osos_contra_reloj.h"
#include "utiles.h"

#define RESET_COLOR		"\x1b[0m"
#define ROJO_T     		"\x1b[31m"
#define BLANCO_F   		"\x1b[47m"
#define ARRIBA 'W'
#define ABAJO 'S'
#define DERECHA 'D'
#define IZQUIERDA 'A'
#define ENCENDER_LINTERNA 'L'
#define ENCENDER_VELA 'V'
#define ENCENDER_BENGALA 'E'
#define TIEMPO_RESTANTE 'T'

const int MAX_FILAS = 20;
const int MAX_COLUMAS = 30;
const int MAX_ARBOLES = 250;
const int MAX_PIEDRAS = 80;
const int MAX_PILAS = 30;
const int MAX_VELAS = 30;
const int MAX_BENGALAS = 10;
const char OSCURIDAD = '-';

const char ARBOL = 'A';
const char PIEDRA = 'R';
const char KOALAS = 'K';
const char LINTERNA = 'L';
const char VELA = 'V';
const char BENGALA = 'E';
const char PILA = 'B';
const char CLOE = 'C';

bool hay_obstaculo(juego_t juego, int fila, int columna){
    bool hay_obstaculo = false;
    for (int i = 0; i < juego.cantidad_obstaculos; i++){
        if ((fila == juego.obstaculos[i].posicion.fil) && (columna == juego.obstaculos[i].posicion.col)){
            hay_obstaculo = true;
		} else if ((juego.personaje.posicion.fil == fila) && (juego.personaje.posicion.col == columna)){
            hay_obstaculo = true;
		} else if ((juego.amiga_chloe.fil == fila) && (juego.amiga_chloe.col == columna)){
            hay_obstaculo = true;
		} else if ((juego.herramientas[i].posicion.fil == fila) && (juego.herramientas[i].posicion.col == columna)){
            hay_obstaculo = true;
		}
    }
    return hay_obstaculo;
}

bool es_jugada_ingresada_valida(char jugada){
	return (jugada == ARRIBA || jugada == ABAJO || jugada == DERECHA || jugada == IZQUIERDA || jugada == ENCENDER_LINTERNA || jugada == ENCENDER_VELA || jugada == ENCENDER_BENGALA || jugada == TIEMPO_RESTANTE);
}

bool es_posicion_personaje_posicion_cloe(juego_t juego){
	return ((juego.amiga_chloe.fil == juego.personaje.posicion.fil) && (juego.amiga_chloe.col == juego.personaje.posicion.col));
}

void mensaje_tipo_de_jugadas(){
	printf("Juegadas validas:\n\t-> %c: Si el personaje debe moverse para la arriba.\n\t-> %c: Si el personaje debe moverse para la izquierda.\n\t-> %c: Si el personaje debe moverse para la abajo.\n\t-> %c: Si el personaje debe moverse para la derecha.\n\t-> %c: Si el personaje quiere encender una linterna.\n\t-> %c: Si el personaje quiere encender una vela.\n\t-> %c: Si el personaje quiere encender la bengala.\n\t-> %c: Si el personaje quiere ver el tiempo restante.\n", ARRIBA,IZQUIERDA, ABAJO, DERECHA, ENCENDER_LINTERNA, ENCENDER_VELA, ENCENDER_BENGALA, TIEMPO_RESTANTE);
}

void mensaje_ingresar_jugada(char* jugada){
	
	printf("Ingrese la jugada que desee realidar\n");
	scanf(" %c", jugada);

	while (!es_jugada_ingresada_valida(*jugada)){
		printf("Tecla ingresada no valida, por favor ingrese: ");
		mensaje_tipo_de_jugadas();
		scanf(" %c", jugada);
		system("clear");
	}
}

void agregar_cloe(juego_t* juego, char item){

		juego->amiga_chloe.fil = rand() % MAX_FILAS;
		juego->amiga_chloe.col = rand() % MAX_COLUMAS;	
}

void agregar_personaje(juego_t* juego, char item){

		juego->personaje.posicion.fil = rand() % MAX_FILAS;
		juego->personaje.posicion.col = 0;
		if ((juego->personaje.posicion.fil==juego->amiga_chloe.fil) && (juego->personaje.posicion.col==juego->amiga_chloe.col)){
			juego->personaje.posicion.fil = rand() % MAX_FILAS;
		}		
}

void agregar_obstaculos(juego_t* juego, char item, int max_items, int* primer_item){
	
	int posicion_fil = 0;
	int posicion_col = 0;

	for (int i = (*primer_item) ; i < max_items; i++){
		juego->obstaculos[i].tipo = item;
		posicion_fil = rand() % MAX_FILAS;
		posicion_col = rand() % MAX_COLUMAS;

		while (hay_obstaculo(*juego, posicion_fil, posicion_col)){
			posicion_fil = rand() % MAX_FILAS;
			posicion_col = rand() % MAX_COLUMAS;
		}

		juego->obstaculos[i].posicion.fil = posicion_fil;
		juego->obstaculos[i].posicion.col = posicion_col;
		(*primer_item) = i;
	}
}

void agregar_herramientas(juego_t* juego, char item, int max_items, int* primer_item){

	int posicion_fil = 0;
	int posicion_col = 0;

	for (int i = (*primer_item) ; i < max_items; i++){
		juego->herramientas[i].tipo = item;
		posicion_fil = rand() % MAX_FILAS;
		posicion_col = rand() % MAX_COLUMAS;

		while (hay_obstaculo(*juego, posicion_fil, posicion_col)){
			posicion_fil = rand() % MAX_FILAS;
			posicion_col = rand() % MAX_COLUMAS;
		}

		juego->herramientas[i].posicion.fil = posicion_fil;
		juego->herramientas[i].posicion.col = posicion_col;
	}
	(*primer_item) = max_items;
}

void crear_campo(char campo[MAX_FILAS][MAX_COLUMAS]){
	
	for (int i = 0; i < MAX_FILAS; i++){
		for (int j = 0; j < MAX_COLUMAS; j++){
			campo[i][j]=OSCURIDAD;	
		}
	}
}

void mostrar_obstaculos_matriz(juego_t juego, char campo[MAX_FILAS][MAX_COLUMAS], int max_obstaculos, int* primer_obstaculo){

	for (int i = (*primer_obstaculo); i < max_obstaculos; i++){
		if(juego.obstaculos[i].visible == true){
			campo[juego.obstaculos[i].posicion.fil][juego.obstaculos[i].posicion.col] = juego.obstaculos[i].tipo;
		}
	}
	(*primer_obstaculo) = max_obstaculos;
}

void mostrar_herramientas_matriz(juego_t juego, char campo[MAX_FILAS][MAX_COLUMAS], int max_herramientas, int* primer_herramientas){

	for (int i = (*primer_herramientas); i < max_herramientas; i++){
		if(juego.herramientas[i].visible == true){
			campo[juego.herramientas[i].posicion.fil][juego.herramientas[i].posicion.col] = juego.herramientas[i].tipo;
			}
	}	
	(*primer_herramientas) = max_herramientas;
}

void mostrar_juego(juego_t juego){

	mensaje_tipo_de_jugadas();

	char campo[MAX_FILAS][MAX_COLUMAS];

	crear_campo(campo);
	if(juego.chloe_visible == true){
		campo[juego.amiga_chloe.fil][juego.amiga_chloe.col]=CLOE;
	}
	campo[juego.personaje.posicion.fil][juego.personaje.posicion.col]=juego.personaje.tipo;

	int i = 0;

	mostrar_obstaculos_matriz(juego, campo, MAX_ARBOLES + i, &i);
	mostrar_obstaculos_matriz(juego, campo, MAX_PIEDRAS + i, &i);

	i = 0;

	mostrar_herramientas_matriz(juego, campo, MAX_VELAS + i, &i);
	mostrar_herramientas_matriz(juego, campo, MAX_PILAS + i, &i);
	mostrar_herramientas_matriz(juego, campo, MAX_BENGALAS + i, &i);

/* 	int cantidad_arboles = 0;
	int cantidad_piedras = 0;
	int cantidad_velas = 0;
	int cantidad_pilas = 0;
	int cantidad_bengalas = 0;
	for (int i = 0; i < MAX_FILAS; i++)
	{
		for (int j = 0; j < MAX_COLUMAS; j++)
		{
			if(campo[i][j] == ARBOL){
				cantidad_arboles ++;
			} else if(campo[i][j] == PIEDRA){
				cantidad_piedras ++;
			} else if(campo[i][j] == VELA){
				cantidad_velas ++;
			} else if(campo[i][j] == PILA){
				cantidad_pilas ++;
			} else if(campo[i][j] == BENGALA){
				cantidad_bengalas ++;
			}
			
		}
		
	}
	printf("Cantidad de arboles %i\n", cantidad_arboles);
	printf("Cantidad de piedras %i\n", cantidad_piedras);
	printf("Cantidad de velas %i\n", cantidad_velas);
	printf("Cantidad de pilas %i\n", cantidad_pilas);
	printf("Cantidad de bengalas %i\n", cantidad_bengalas); */

	for (int i = 0; i < MAX_FILAS; i++){
		for (int j = 0; j < MAX_COLUMAS; j++){
			if (campo[i][j]==CLOE){
				printf(ROJO_T BLANCO_F " %c " RESET_COLOR, campo[i][j]);
			} else if (campo[i][j]==juego.personaje.tipo){
				printf(ROJO_T BLANCO_F " %c " RESET_COLOR, campo[i][j]);
			} else{
				printf(" %c ", campo[i][j]);
			}
		}
		printf("\t%i \n", i+1);
	}
	
}

void inicializar_juego(juego_t* juego, char tipo_personaje){

	juego->chloe_visible=false;
	juego->obstaculos->visible=false;
	juego->herramientas->visible=false;

	juego->cantidad_obstaculos=(MAX_ARBOLES + MAX_PIEDRAS);
	juego->cantidad_herramientas=(MAX_PILAS + MAX_VELAS + MAX_BENGALAS);
	juego->personaje.tipo = tipo_personaje;

	agregar_cloe(juego, CLOE);

	agregar_personaje(juego, tipo_personaje);

	int i = 0;

	agregar_obstaculos(juego, ARBOL, MAX_ARBOLES + 1, &i);
	agregar_obstaculos(juego, PIEDRA, MAX_PIEDRAS + i, &i);

	i = 0;

	agregar_herramientas(juego, VELA, MAX_VELAS + i, &i);
	agregar_herramientas(juego, PILA, MAX_PILAS + i, &i);
	agregar_herramientas(juego, BENGALA, MAX_BENGALAS + i, &i);

	mostrar_juego(*juego);
}

void realizar_jugada(juego_t* juego, char jugada){

	mensaje_ingresar_jugada(&jugada);

	switch (jugada)
	{
	case ARRIBA:
		if(juego->personaje.posicion.fil > 0)
			juego->personaje.posicion.fil--;
		break;
	case ABAJO:
		if(juego->personaje.posicion.fil < MAX_FILAS-1)
			juego->personaje.posicion.fil++;
		break;
	case DERECHA:
		if(juego->personaje.posicion.col < MAX_COLUMAS-1)
			juego->personaje.posicion.col++;
		break;
	case IZQUIERDA:
		if(juego->personaje.posicion.col > 0)
			juego->personaje.posicion.col--;
		break;
	case ENCENDER_LINTERNA:
		//Si el personaje quiere encender una linterna.
		break;
	case ENCENDER_VELA:
		//Si el personaje quiere encender una vela.
		break;
	case ENCENDER_BENGALA:
		//Si el personaje quiere encender la bengala.
		break;
	case TIEMPO_RESTANTE:
		//Si el personaje quiere ver el tiempo restante.
		break;
	}
	system("clear");
	mostrar_juego(*juego);
	
}

int estado_juego(juego_t juego){
	
	int estado_juego = 0;
	if (es_posicion_personaje_posicion_cloe(juego)){
		estado_juego = -1;
	}
	return estado_juego;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test_de_personalidad.h"
#include "osos_contra_reloj.h"
#include "utiles.h"

#define RESET_COLOR    "\x1b[0m"
#define ROJO_T     "\x1b[31m"
#define BLANCO_F   "\x1b[47m"

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

void agrgar_cloe(char campo[MAX_FILAS][MAX_COLUMAS]){
	int numero_fila = rand () % MAX_FILAS;
	int numero_columna = rand () % MAX_COLUMAS;
	
	campo[numero_fila][numero_columna]=CLOE;
}

void agregar_items(char campo[MAX_FILAS][MAX_COLUMAS], char item, int max_items){
	int numero_fila = rand () % MAX_FILAS;
	int numero_columna = rand () % MAX_COLUMAS;

	for (int p = 1; p <= max_items; p++){
			numero_fila = rand () % MAX_FILAS;
			numero_columna = rand () % MAX_COLUMAS;
			if (campo[numero_fila][numero_columna]==OSCURIDAD && campo[numero_fila][numero_columna] != CLOE){
				campo[numero_fila][numero_columna]=item;
			}		
	}

}

int main(){

	char campo[MAX_FILAS][MAX_COLUMAS];

	srand (( unsigned)time(NULL));
	
	

	for (int i = 0; i < MAX_FILAS; i++){
		for (int j = 0; j < MAX_COLUMAS; j++){
			campo[i][j]=OSCURIDAD;	
		}
	}

	agrgar_cloe(campo);
		
	agregar_items(campo, PIEDRA, MAX_PIEDRAS);
	agregar_items(campo, ARBOL, MAX_ARBOLES);
	agregar_items(campo, PILA, MAX_PILAS);
	agregar_items(campo, VELA, MAX_VELAS);
	agregar_items(campo, BENGALA, MAX_BENGALAS);

	for (int i = 0; i < MAX_FILAS; i++){
		for (int j = 0; j < MAX_COLUMAS; j++){
			if (campo[i][j]==CLOE){
				printf(ROJO_T BLANCO_F "%c " RESET_COLOR, campo[i][j]);
			} else{
				printf("%c ", campo[i][j]);
			}
		}

		printf(" %i", i+1);
		printf("\n");
	}

	return 0;
}
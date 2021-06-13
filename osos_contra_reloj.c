#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "test_de_personalidad.h"
#include "osos_contra_reloj.h"
#include "utiles.h"

#define RESET_COLOR "\x1b[0m"
#define NEGRO_T    "\x1b[30m"
#define ROJO_T     "\x1b[31m"
#define VERDE_T    "\x1b[32m"
#define AMARILLO_T "\x1b[33m"
#define AZUL_T     "\x1b[34m"
#define MAGENTA_T  "\x1b[35m"
#define CYAN_T     "\x1b[36m"
#define BLANCO_T   "\x1b[37m"
#define BLANCO_F   "\x1b[47m"
#define ARRIBA 'W'
#define ABAJO 'S'
#define DERECHA 'D'
#define IZQUIERDA 'A'
#define ENCENDER_APAGAR_LINTERNA 'L'
#define ENCENDER_APAGAR_VELA 'V'
#define ENCENDER_APAGAR_BENGALA 'E'
#define TIEMPO_RESTANTE 'T'
#define STR_PILAS "pilas"
#define STR_VELAS "velas"
#define STR_BENGALAS "bengalas"
#define MAX_PALABRA 256

const int MAX_FILAS = 20;
const int MAX_COLUMAS = 30;
const int MAX_ARBOLES = 350;
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

const int CANTIDAD_LINTERNA_INICIO = 1;
const int DURACION_MOVIMIENTOS_LINTERNA = 10;
const int DURACION_MOVIMIENTOS_LINTERNA_PARDO = 15;

const int CANTIDAD_VELAS_INICIO = 4;
const int CANTIDAD_VELAS_INICIO_POLAR = 6;
const int DURACION_MOVIMIENTOS_VELA = 5;

const int CANTIDAD_BENGALAS_INICIO = 0;
const int CANTIDAD_BENGALAS_INICIO_PANDA = 2;
const int DURACION_MOVIMIENTOS_BENGALA = 3;

const char POLAR_OSO = 'I';
const char PANDA_OSO = 'P';
const char PARDO_OSO = 'G';

const double SUMAR_TIEMPO_PARDO_ARBOL = 0.5;
const double SUMAR_TIEMPO_NO_PARDO_ARBOL = 1;
const double SUMAR_TIEMPO_PIEDRA = 2;

const int ELEMENTO_NO_USO = -1;
const int SIN_HERRAMIENTA = 0;

const int EN_JUEGO = 0;
const int JUEGO_FINALIZADO = -1;

const int DISTANCIA_MANHATTAM = 3;

void cargar_cloe(juego_t* juego){
	juego->amiga_chloe.fil = rand() % MAX_FILAS;
	juego->amiga_chloe.col = rand() % MAX_COLUMAS;	
}

bool es_posicion_personaje_posicion_cloe(juego_t juego){
	return ((juego.amiga_chloe.fil == juego.personaje.posicion.fil) && (juego.amiga_chloe.col == juego.personaje.posicion.col));
}

void cargar_personaje(juego_t* juego){
	juego->personaje.posicion.fil = rand() % MAX_FILAS;
	juego->personaje.posicion.col = 0;
	if (es_posicion_personaje_posicion_cloe(*juego)){
		juego->personaje.posicion.fil = rand() % MAX_FILAS;
	}
}

void cargar_mochila_inicio(juego_t* juego, char item, int max_items, int duracion_movimiento, int* primer_item){
	for (int i = *primer_item; i < max_items; i++){
		juego->personaje.mochila[i].tipo = item;
		juego->personaje.mochila[i].movimientos_restantes = duracion_movimiento;
	}
	(*primer_item) = max_items;
}

bool es_alguna_herramienta(juego_t juego, int i){
	return ((juego.personaje.mochila[i].tipo == PILA) || (juego.personaje.mochila[i].tipo == VELA) || (juego.personaje.mochila[i].tipo == BENGALA));
}

void calcular_cantidad_elementos(juego_t* juego){
	int i = 0;

	while(es_alguna_herramienta(*juego, i)){
		juego->personaje.cantidad_elementos++;
		i++;
	}	
}

void inicializar_mochila(juego_t* juego){
	int cantidad_inicio_vela = 0;
	int cantidad_inicio_bengala = 0;
	int duracion_tiempo_linterna = 0;
	int primera_posicion_vector = 0;

	if(juego->personaje.tipo == PARDO_OSO)
		duracion_tiempo_linterna = DURACION_MOVIMIENTOS_LINTERNA_PARDO;
	else
		duracion_tiempo_linterna = DURACION_MOVIMIENTOS_LINTERNA;

	cargar_mochila_inicio(juego, PILA, CANTIDAD_LINTERNA_INICIO, duracion_tiempo_linterna, &primera_posicion_vector);
	
	if(juego->personaje.tipo == POLAR_OSO)
		cantidad_inicio_vela = CANTIDAD_VELAS_INICIO_POLAR;
	else
		cantidad_inicio_vela = CANTIDAD_VELAS_INICIO;

	cargar_mochila_inicio(juego, VELA, (CANTIDAD_LINTERNA_INICIO + cantidad_inicio_vela), DURACION_MOVIMIENTOS_VELA, &primera_posicion_vector);
	
	if(juego->personaje.tipo == PANDA_OSO)
		cantidad_inicio_bengala = CANTIDAD_BENGALAS_INICIO_PANDA;
	else
		cantidad_inicio_bengala = CANTIDAD_BENGALAS_INICIO;

	cargar_mochila_inicio(juego, BENGALA, (CANTIDAD_LINTERNA_INICIO + cantidad_inicio_vela + cantidad_inicio_bengala), DURACION_MOVIMIENTOS_BENGALA, &primera_posicion_vector);

	calcular_cantidad_elementos(juego);
}

bool hay_algo_en_posicion(juego_t juego, int fila, int columna){
    bool hay_algo_en_posicion = false;
    for (int i = 0; i < juego.cantidad_obstaculos; i++){
        if ((fila == juego.obstaculos[i].posicion.fil) && (columna == juego.obstaculos[i].posicion.col)){
            hay_algo_en_posicion = true;
		} else if ((juego.personaje.posicion.fil == fila) && (juego.personaje.posicion.col == columna)){
            hay_algo_en_posicion = true;
		} else if ((juego.amiga_chloe.fil == fila) && (juego.amiga_chloe.col == columna)){
            hay_algo_en_posicion = true;
		} else if ((juego.herramientas[i].posicion.fil == fila) && (juego.herramientas[i].posicion.col == columna)){
            hay_algo_en_posicion = true;
		}
    }
    return hay_algo_en_posicion;
}

void cargar_obstaculos(juego_t* juego, char item, int max_items, int* primer_item){	
	int posicion_fil = 0;
	int posicion_col = 0;

	for (int i = (*primer_item) ; i < max_items; i++){
		juego->obstaculos[i].tipo = item;
		posicion_fil = rand() % MAX_FILAS;
		posicion_col = rand() % MAX_COLUMAS;

		while (hay_algo_en_posicion(*juego, posicion_fil, posicion_col)){
			posicion_fil = rand() % MAX_FILAS;
			posicion_col = rand() % MAX_COLUMAS;
		}
		juego->obstaculos[i].posicion.fil = posicion_fil;
		juego->obstaculos[i].posicion.col = posicion_col;
		(*primer_item) = i;
	}
}

void cargar_herramientas(juego_t* juego, char item, int max_items, int* primer_item){
	int posicion_fil = 0;
	int posicion_col = 0;

	for (int i = (*primer_item) ; i < max_items; i++){
		juego->herramientas[i].tipo = item;
		posicion_fil = rand() % MAX_FILAS;
		posicion_col = rand() % MAX_COLUMAS;

		while (hay_algo_en_posicion(*juego, posicion_fil, posicion_col)){
			posicion_fil = rand() % MAX_FILAS;
			posicion_col = rand() % MAX_COLUMAS;
		}
		juego->herramientas[i].posicion.fil = posicion_fil;
		juego->herramientas[i].posicion.col = posicion_col;
	}
	(*primer_item) = max_items;
}

void imprimir_elementos_en_mochila(juego_t juego){	
	int contador_pilas = 0, contador_velas = 0, contador_bengalas = 0;
	int mov_pilas = 0, mov_velas = 0, mov_bengalas = 0;
	
	for (int i = 0; i < juego.personaje.cantidad_elementos; i++){
		if (juego.personaje.mochila[i].tipo == PILA){
			contador_pilas++;
			if(juego.personaje.mochila [juego.personaje.elemento_en_uso].tipo == PILA){
				mov_pilas = juego.personaje.mochila[juego.personaje.elemento_en_uso].movimientos_restantes;
			} else {
				mov_pilas = juego.personaje.mochila[i].movimientos_restantes;
			}
		} else if (juego.personaje.mochila[i].tipo == VELA){
			contador_velas++;
			if(juego.personaje.mochila [juego.personaje.elemento_en_uso].tipo == VELA){
				mov_velas = juego.personaje.mochila[juego.personaje.elemento_en_uso].movimientos_restantes;
			} else {
				mov_velas = juego.personaje.mochila[i].movimientos_restantes;
			}
		} else if (juego.personaje.mochila[i].tipo == BENGALA){
			contador_bengalas++;
			if(juego.personaje.mochila [juego.personaje.elemento_en_uso].tipo == BENGALA){
				mov_bengalas = juego.personaje.mochila[juego.personaje.elemento_en_uso].movimientos_restantes;
			} else {
				mov_bengalas = juego.personaje.mochila[i].movimientos_restantes;
			}
		}
	}
	printf("\n");
	printf("\t\t|Tenes %i pilas con %i movimientos cada una  |\n", contador_pilas, mov_pilas);
	printf("\t\t|Tenes %i velas con %i movimientos cada una   |\n", contador_velas, mov_velas);
	printf("\t\t|Tenes %i bengalas con %i movimientos cada una|\n", contador_bengalas, mov_bengalas);
}

void crear_campo(char campo[MAX_FILAS][MAX_COLUMAS]){
	for (int i = 0; i < MAX_FILAS; i++){
		for (int j = 0; j < MAX_COLUMAS; j++){
			campo[i][j]=OSCURIDAD;	
		}
	}
}

void mensaje_si_cloe_visible(juego_t juego){
	if(juego.chloe_visible == true)
		printf("Cloe visible!! Rum Forest rum»»»\n");
}

void visibilizar_cloe(juego_t juego, char campo[MAX_FILAS][MAX_COLUMAS]){
	if((juego.personaje.tipo == PANDA_OSO) && (tiempo_actual() >=30)){
		juego.chloe_visible = true;
		mensaje_si_cloe_visible(juego);
		campo[juego.amiga_chloe.fil][juego.amiga_chloe.col]=CLOE;
	}
}

void mensaje_si_herramientas_activada(juego_t juego){
	if(juego.personaje.mochila [juego.personaje.elemento_en_uso].tipo == LINTERNA)
		printf("Linterna activada!!");
	if(juego.personaje.mochila [juego.personaje.elemento_en_uso].tipo == VELA)
		printf("Vela activada!!");
	if(juego.personaje.mochila [juego.personaje.elemento_en_uso].tipo == BENGALA)
		printf("Bengala activada!!");
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

void imprimir_elementos_campo(juego_t juego, char campo[MAX_FILAS][MAX_COLUMAS]){
	for (int i = 0; i < MAX_FILAS; i++){
		printf("\t»");
		for (int j = 0; j < MAX_COLUMAS; j++){
			if (campo[i][j]==CLOE){
				printf(NEGRO_T BLANCO_F " %c " RESET_COLOR, campo[i][j]);
			} else if (campo[i][j]== juego.personaje.tipo){
				printf(AMARILLO_T " %c " RESET_COLOR, campo[i][j]);
			} else if (campo[i][j]== ARBOL){
				printf(VERDE_T " %c " RESET_COLOR, campo[i][j]);
			} else if (campo[i][j]== PIEDRA){
				printf(CYAN_T " %c " RESET_COLOR, campo[i][j]);
			} else if (campo[i][j]== BENGALA){
				printf(ROJO_T " %c " RESET_COLOR, campo[i][j]);
			} else if (campo[i][j]== KOALAS){
				printf(MAGENTA_T " %c " RESET_COLOR, campo[i][j]);
			} else{
				printf(" %c ", campo[i][j]);
			}
		}
		printf("«\t%i \n", i+1);
	}
}

void mensaje_tipo_de_jugadas(){
	printf("Juegadas validas:\n\t→ %c: Si el personaje debe moverse para la arriba.\n\t→ %c: Si el personaje debe moverse para la izquierda.\n\t→ %c: Si el personaje debe moverse para la abajo.\n\t→ %c: Si el personaje debe moverse para la derecha.\n\t→ %c: Si el personaje quiere encender una linterna.\n\t→ %c: Si el personaje quiere encender una vela.\n\t→ %c: Si el personaje quiere encender la bengala.\n\t→ %c: Si el personaje quiere ver el tiempo restante.\n\n", ARRIBA,IZQUIERDA, ABAJO, DERECHA, ENCENDER_APAGAR_LINTERNA, ENCENDER_APAGAR_VELA, ENCENDER_APAGAR_BENGALA, TIEMPO_RESTANTE);
}

void mostrar_juego(juego_t juego){
	mensaje_tipo_de_jugadas();

	char campo[MAX_FILAS][MAX_COLUMAS];

	crear_campo(campo);

	visibilizar_cloe(juego, campo);

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

	imprimir_elementos_campo(juego, campo);
	imprimir_elementos_en_mochila(juego);
	mensaje_si_herramientas_activada(juego);
}

void inicializar_juego(juego_t* juego, char tipo_personaje){
	int primera_posicion_vector = 0;

	juego->chloe_visible=false;
	juego->obstaculos->visible=false;
	juego->herramientas->visible=false;

	juego->cantidad_obstaculos=(MAX_ARBOLES + MAX_PIEDRAS);
	juego->cantidad_herramientas=(MAX_PILAS + MAX_VELAS + MAX_BENGALAS);
	juego->personaje.tipo = tipo_personaje;
	juego->personaje.cantidad_elementos = 0;
	juego->personaje.elemento_en_uso = ELEMENTO_NO_USO;

	cargar_cloe(juego);
	cargar_personaje(juego);

	inicializar_mochila(juego);

	cargar_obstaculos(juego, ARBOL, MAX_ARBOLES + 1, &primera_posicion_vector);
	cargar_obstaculos(juego, PIEDRA, MAX_PIEDRAS + primera_posicion_vector, &primera_posicion_vector);

	primera_posicion_vector = 0;

	cargar_herramientas(juego, VELA, MAX_VELAS + primera_posicion_vector, &primera_posicion_vector);
	cargar_herramientas(juego, PILA, MAX_PILAS + primera_posicion_vector, &primera_posicion_vector);
	cargar_herramientas(juego, BENGALA, MAX_BENGALAS + primera_posicion_vector, &primera_posicion_vector);

	mostrar_juego(*juego);
}

bool es_jugada_ingresada_valida(char jugada){
	return (jugada == ARRIBA || jugada == ABAJO || jugada == DERECHA || jugada == IZQUIERDA || jugada == ENCENDER_APAGAR_LINTERNA || jugada == ENCENDER_APAGAR_VELA || jugada == ENCENDER_APAGAR_BENGALA || jugada == TIEMPO_RESTANTE);
}

void mensaje_ingresar_jugada(char* jugada){
	
	printf("\nIngrese la jugada que desee realidar\n");
	scanf(" %c", jugada);

	while (!es_jugada_ingresada_valida(*jugada)){
		printf("Tecla ingresada no valida, por favor ingrese las jugadas validas.");
		scanf(" %c", jugada);
	}
}

void encotrar_obstaculo(juego_t* juego){
	for (int i = 0; i < juego->cantidad_obstaculos; i++){
		if((juego->personaje.posicion.fil == juego->obstaculos[i].posicion.fil) && (juego->personaje.posicion.col == juego->obstaculos[i].posicion.col)){
			juego->obstaculos[i].visible = false;

			if(juego->obstaculos[i].tipo == ARBOL){
				if(juego->personaje.tipo == PARDO_OSO){
					juego->personaje.tiempo_perdido += SUMAR_TIEMPO_PARDO_ARBOL;
				} else {
					juego->personaje.tiempo_perdido += SUMAR_TIEMPO_NO_PARDO_ARBOL;
				}
			}
			if(juego->obstaculos[i].tipo == PIEDRA){
				if(juego->personaje.tipo != POLAR_OSO){
					juego->personaje.tiempo_perdido += SUMAR_TIEMPO_PIEDRA;
				}
			}
			if(juego->obstaculos[i].tipo == KOALAS){
				cargar_personaje(juego);
			}
		} else if((juego->personaje.posicion.fil == juego->herramientas[i].posicion.fil) && (juego->personaje.posicion.col == juego->herramientas[i].posicion.col)){
			juego->herramientas[i].posicion.fil = MAX_FILAS;
			juego->herramientas[i].posicion.col = MAX_COLUMAS;
			juego->personaje.cantidad_elementos++;
			if(juego->herramientas[i].tipo == PILA){
				juego->personaje.mochila[juego->personaje.cantidad_elementos].tipo = PILA;
				juego->personaje.mochila[juego->personaje.cantidad_elementos].movimientos_restantes = DURACION_MOVIMIENTOS_LINTERNA;
			}
			if(juego->herramientas[i].tipo == VELA){
				juego->personaje.mochila[juego->personaje.cantidad_elementos].tipo = VELA;
				juego->personaje.mochila[juego->personaje.cantidad_elementos].movimientos_restantes = DURACION_MOVIMIENTOS_VELA;
			}
			if(juego->herramientas[i].tipo == BENGALA){
				juego->personaje.mochila[juego->personaje.cantidad_elementos].tipo = BENGALA;
				juego->personaje.mochila[juego->personaje.cantidad_elementos].movimientos_restantes = DURACION_MOVIMIENTOS_BENGALA;
			}
			juego->personaje.cantidad_elementos++;
			juego->herramientas[i].tipo = OSCURIDAD;
		}
	}
}

int calcular_cantidad_herramienta(juego_t* juego, char herramienta){
	int cantidad_herramienta = SIN_HERRAMIENTA;

	for (int i = 0; i < juego->personaje.cantidad_elementos; i++){
				if(juego->personaje.mochila[i].tipo == herramienta)
					cantidad_herramienta++;
	}

	return cantidad_herramienta;
}

void iluminar_campo(juego_t* juego, int posicion_fil, int posicion_col){
	for (int i = 0; i < juego->cantidad_obstaculos; i++){
			if((posicion_fil == juego->obstaculos[i].posicion.fil) && (posicion_col == juego->obstaculos[i].posicion.col)){
				juego->obstaculos[i].visible = true;
			}
			if((posicion_fil == juego->herramientas[i].posicion.fil) && (posicion_col == juego->herramientas[i].posicion.col)){
				juego->herramientas[i].visible = true;
			}
			if((posicion_fil == juego->amiga_chloe.fil) && (posicion_col == juego->amiga_chloe.col)){
				juego->chloe_visible = true;
			}
		}

}

void ocultar_elementos_del_campo(juego_t* juego){
	for (int i = 0; i < juego->cantidad_herramientas; i++){
		if(juego->herramientas[i].visible == true)
			juego->herramientas[i].visible = false;
	}
	for (int i = 0; i < juego->cantidad_obstaculos; i++){
		if(juego->obstaculos[i].visible == true)
			juego->obstaculos[i].visible = false;
	}
	if(juego->chloe_visible == true)
		juego->chloe_visible = false;
}

void iluminar_elementos_alrrededor_personaje(juego_t* juego, int fila, int columna){
	for (int i = fila-1; i <= fila+1; i++){
		for (int j = columna-1; j <= columna+1; j++){
			if((i != juego->personaje.posicion.fil) || (j != juego->personaje.posicion.col)){
				iluminar_campo(juego, i, j);
			}
		}
	}
}

void iluminar_distacia_mahattam(juego_t* juego, int numero_fila, int numero_columna){
	for (int i = 0; i < MAX_FILAS; i++){
		for (int j = 0; j < MAX_COLUMAS; j++){
			if((fabs (numero_fila - i) + fabs (numero_columna - j)) <= DISTANCIA_MANHATTAM){
				iluminar_campo(juego, i, j);
			}
		}
	}
}

void intercambio_herramienta_uso_con_ultima(juego_t* juego, int posicion_herramienta_uso){
	elemento_mochila_t aux;

	aux = juego->personaje.mochila[posicion_herramienta_uso];
	juego->personaje.mochila[posicion_herramienta_uso] = juego->personaje.mochila[juego->personaje.cantidad_elementos-1];
	juego->personaje.mochila[juego->personaje.cantidad_elementos-1] = aux;
	juego->personaje.elemento_en_uso = juego->personaje.cantidad_elementos-1;
}

void movimiento_vela(juego_t *juego){
	if(juego->personaje.mochila [juego->personaje.elemento_en_uso].tipo == VELA){
		ocultar_elementos_del_campo(juego);		
		iluminar_elementos_alrrededor_personaje(juego, juego->personaje.posicion.fil, juego->personaje.posicion.col);
	}
}

void mensaje_sin_herramienta_mochila(char herramienta[MAX_PALABRA]){
	 printf("Te quedastes sin %s\n", herramienta);
}

void realizar_jugada(juego_t* juego, char jugada){

	int fila_bengala = 0;
	int columna_bengala = 0;
	int cantidad_bengalas, cantidad_velas;
	int i = 0;

	mensaje_ingresar_jugada(&jugada);

	switch (jugada){
	case ARRIBA:
		if(juego->personaje.posicion.fil > 0){
			juego->personaje.posicion.fil--;
			if(juego->personaje.elemento_en_uso != ELEMENTO_NO_USO)
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
		}
		encotrar_obstaculo(juego);
		movimiento_vela(juego);
		system("clear");
		break;
	case ABAJO:
		if(juego->personaje.posicion.fil < MAX_FILAS-1){
			juego->personaje.posicion.fil++;
			if(juego->personaje.elemento_en_uso != ELEMENTO_NO_USO)
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
		}
		encotrar_obstaculo(juego);
		movimiento_vela(juego);
		system("clear");
		break;
	case DERECHA:
		if(juego->personaje.posicion.col < MAX_COLUMAS-1){
			juego->personaje.posicion.col++;
			if(juego->personaje.elemento_en_uso != ELEMENTO_NO_USO)
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
		}
		encotrar_obstaculo(juego);
		movimiento_vela(juego);
		system("clear");
		break;
	case IZQUIERDA:
		if(juego->personaje.posicion.col > 0){
			juego->personaje.posicion.col--;
			if(juego->personaje.elemento_en_uso != ELEMENTO_NO_USO)
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
		}
		encotrar_obstaculo(juego);
		movimiento_vela(juego);
		system("clear");
		break;
	case ENCENDER_APAGAR_LINTERNA:
		mensaje_sin_herramienta_mochila(STR_PILAS);
		//Si el personaje quiere encender una linterna.
		break;
	case ENCENDER_APAGAR_VELA:
		system("clear");
		cantidad_velas = calcular_cantidad_herramienta(juego, VELA);
		if(cantidad_velas <= SIN_HERRAMIENTA){
			mensaje_sin_herramienta_mochila(STR_VELAS);
		} else if(juego->personaje.mochila [juego->personaje.elemento_en_uso].tipo == BENGALA){
			printf("La bengala esta en uso esta en uso maestro!!!\n");
		} else if(juego->personaje.mochila [juego->personaje.elemento_en_uso].tipo == VELA){
			ocultar_elementos_del_campo(juego);
			intercambio_herramienta_uso_con_ultima(juego, juego->personaje.elemento_en_uso);
			juego->personaje.elemento_en_uso = ELEMENTO_NO_USO;
			printf("Vela desactivada\n");
		} else {
			ocultar_elementos_del_campo(juego);
			while((juego->personaje.mochila[i].tipo != VELA) && (i < juego->personaje.cantidad_elementos)){
				i++;
			}
			if(juego->personaje.mochila[i].tipo == VELA){
				iluminar_elementos_alrrededor_personaje(juego, juego->personaje.posicion.fil, juego->personaje.posicion.col);
				
				intercambio_herramienta_uso_con_ultima(juego, i);
			}
			cantidad_velas--;
		} 
		break;
	case ENCENDER_APAGAR_BENGALA:
		system("clear");
		cantidad_bengalas = calcular_cantidad_herramienta(juego, BENGALA);
		if(cantidad_bengalas <= SIN_HERRAMIENTA){
				mensaje_sin_herramienta_mochila(STR_BENGALAS);
		} else if(juego->personaje.mochila [juego->personaje.elemento_en_uso].tipo != BENGALA){
			ocultar_elementos_del_campo(juego);
			while((juego->personaje.mochila[i].tipo != BENGALA) && (i < juego->personaje.cantidad_elementos)){
				i++;
			}
			if(juego->personaje.mochila[i].tipo == BENGALA){
				fila_bengala = rand() % MAX_FILAS;
				columna_bengala = rand() % MAX_COLUMAS;
				iluminar_distacia_mahattam(juego, fila_bengala, columna_bengala);

				intercambio_herramienta_uso_con_ultima(juego, i);
			}
			cantidad_bengalas--;
		} else {
			printf("La bengala esta en uso esta en uso maestro!!!\n");
		}
		break;
	case TIEMPO_RESTANTE:
		system("clear");
		printf("*Tiempo Actual: %.0f segundos.\n", tiempo_actual());
		break;
	}
	

	if(juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes == 0){
		for (int i = 0; i < juego->personaje.cantidad_elementos; i++){
			if(juego->personaje.mochila[i].movimientos_restantes == 0){
				intercambio_herramienta_uso_con_ultima(juego, i);
				juego->personaje.cantidad_elementos--;
			}
		}
		ocultar_elementos_del_campo(juego);
		
		juego->personaje.elemento_en_uso = ELEMENTO_NO_USO;
	}
	mostrar_juego(*juego);
}

int estado_juego(juego_t juego){
	
	int estado_juego = EN_JUEGO;
	if (es_posicion_personaje_posicion_cloe(juego)){
		estado_juego = JUEGO_FINALIZADO;
	}
	return estado_juego;
}

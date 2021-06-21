#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
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
const int MAX_TIEMPO = 120; 

const int MAX_FILAS = 20;
const int MAX_COLUMAS = 30;
const int MAX_ARBOLES = 350;
const int MAX_PIEDRAS = 80;
const int MAX_KOALAS = 1;
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
const char CHLOE = 'C';

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

const char SIN_MOVIMIENTO = ' ';

void cargar_chloe(coordenada_t* chloe){
	chloe->fil = rand() % MAX_FILAS;
	chloe->col = rand() % MAX_COLUMAS;	
}

bool es_posicion_personaje_posicion_chloe(juego_t juego){
	return ((juego.amiga_chloe.fil == juego.personaje.posicion.fil) && (juego.amiga_chloe.col == juego.personaje.posicion.col));
}

void cargar_personaje(juego_t* juego){
	juego->personaje.posicion.fil = rand() % MAX_FILAS;
	juego->personaje.posicion.col = 0;
	if (es_posicion_personaje_posicion_chloe(*juego)){
		juego->personaje.posicion.fil = rand() % MAX_FILAS;
	}
}

void cargar_mochila_inicio(personaje_t* personaje, char item, int max_items, int duracion_movimiento, int primer_item){
	for (int i = primer_item; i < max_items; i++){
		personaje->mochila[i].tipo = item;
		personaje->mochila[i].movimientos_restantes = duracion_movimiento;
		personaje->cantidad_elementos++;
	}
}

bool es_alguna_herramienta(juego_t juego, int i){
	return ((juego.personaje.mochila[i].tipo == PILA) || (juego.personaje.mochila[i].tipo == VELA) || (juego.personaje.mochila[i].tipo == BENGALA));
}

void inicializar_mochila(juego_t* juego){
	int cantidad_inicio_vela = 0;
	int cantidad_inicio_bengala = 0;
	int duracion_tiempo_linterna = 0;
	juego->personaje.cantidad_elementos = 0;

	if(juego->personaje.tipo == PARDO_OSO)
		duracion_tiempo_linterna = DURACION_MOVIMIENTOS_LINTERNA_PARDO;
	else
		duracion_tiempo_linterna = DURACION_MOVIMIENTOS_LINTERNA;
	cargar_mochila_inicio(&juego->personaje, PILA, CANTIDAD_LINTERNA_INICIO, duracion_tiempo_linterna, juego->personaje.cantidad_elementos);
	
	if(juego->personaje.tipo == POLAR_OSO)
		cantidad_inicio_vela = CANTIDAD_VELAS_INICIO_POLAR;
	else
		cantidad_inicio_vela = CANTIDAD_VELAS_INICIO;
	cargar_mochila_inicio(&juego->personaje, VELA, (CANTIDAD_LINTERNA_INICIO + cantidad_inicio_vela), DURACION_MOVIMIENTOS_VELA, juego->personaje.cantidad_elementos);
	
	if(juego->personaje.tipo == PANDA_OSO)
		cantidad_inicio_bengala = CANTIDAD_BENGALAS_INICIO_PANDA;
	else
		cantidad_inicio_bengala = CANTIDAD_BENGALAS_INICIO;
	cargar_mochila_inicio(&juego->personaje, BENGALA, (CANTIDAD_LINTERNA_INICIO + cantidad_inicio_vela + cantidad_inicio_bengala), DURACION_MOVIMIENTOS_BENGALA, juego->personaje.cantidad_elementos);
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

void cargar_obstaculos(juego_t* juego, char item, int tope, int primer_item){	
	int posicion_fil = 0;
	int posicion_col = 0;
	for (int i = primer_item ; i < tope; i++){
		juego->obstaculos[i].tipo = item;
		posicion_fil = rand() % MAX_FILAS;
		posicion_col = rand() % MAX_COLUMAS;
		while (hay_algo_en_posicion(*juego, posicion_fil, posicion_col)){
			posicion_fil = rand() % MAX_FILAS;
			posicion_col = rand() % MAX_COLUMAS;
		}
		juego->obstaculos[i].posicion.fil = posicion_fil;
		juego->obstaculos[i].posicion.col = posicion_col;
		juego->cantidad_obstaculos++;
	}
}

void cargar_herramientas(juego_t* juego, char item, int tope, int primer_item){
	int posicion_fil = 0;
	int posicion_col = 0;
	for (int i = primer_item ; i < tope; i++){
		juego->herramientas[i].tipo = item;
		posicion_fil = rand() % MAX_FILAS;
		posicion_col = rand() % MAX_COLUMAS;
		while (hay_algo_en_posicion(*juego, posicion_fil, posicion_col)){
			posicion_fil = rand() % MAX_FILAS;
			posicion_col = rand() % MAX_COLUMAS;
		}
		juego->herramientas[i].posicion.fil = posicion_fil;
		juego->herramientas[i].posicion.col = posicion_col;
		juego->cantidad_herramientas++;
	}
}

void imprimir_elementos_en_mochila(personaje_t personaje){	
	int contador_pilas = 0, contador_velas = 0, contador_bengalas = 0;
	int mov_pilas = 0, mov_velas = 0, mov_bengalas = 0;
	for (int i = 0; i < personaje.cantidad_elementos; i++){
		if (personaje.mochila[i].tipo == PILA){
			contador_pilas++;
			if(personaje.mochila [personaje.elemento_en_uso].tipo == PILA){
				mov_pilas = personaje.mochila[personaje.elemento_en_uso].movimientos_restantes;
			} else {
				mov_pilas = personaje.mochila[i].movimientos_restantes;
			}
		} else if (personaje.mochila[i].tipo == VELA){
			contador_velas++;
			if(personaje.mochila [personaje.elemento_en_uso].tipo == VELA){
				mov_velas = personaje.mochila[personaje.elemento_en_uso].movimientos_restantes;
			} else {
				mov_velas = personaje.mochila[i].movimientos_restantes;
			}
		} else if (personaje.mochila[i].tipo == BENGALA){
			contador_bengalas++;
			if(personaje.mochila [personaje.elemento_en_uso].tipo == BENGALA){
				mov_bengalas = personaje.mochila[personaje.elemento_en_uso].movimientos_restantes;
			} else {
				mov_bengalas = personaje.mochila[i].movimientos_restantes;
			}
		}
	}
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

void mensaje_si_chloe_visible(juego_t juego){
	if(juego.chloe_visible == true)
		printf("Chloe visible!! Run Forest run»»»\n");
}

void visibilizar_chloe(juego_t juego, char campo[MAX_FILAS][MAX_COLUMAS]){
	if((juego.personaje.tipo == PANDA_OSO) && (tiempo_actual() >=30)){
		juego.chloe_visible = true;
		mensaje_si_chloe_visible(juego);
		campo[juego.amiga_chloe.fil][juego.amiga_chloe.col]=CHLOE;
	}
}

void mensaje_si_herramientas_activada(juego_t juego){
	if(juego.personaje.mochila [juego.personaje.elemento_en_uso].tipo == PILA)
		printf("Linterna activada!!\n");
	if(juego.personaje.mochila [juego.personaje.elemento_en_uso].tipo == VELA)
		printf("Vela activada!!\n");
	if(juego.personaje.mochila [juego.personaje.elemento_en_uso].tipo == BENGALA)
		printf("Bengala activada!!\n");
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

void mostrar_chloe_matriz(juego_t juego, char campo[MAX_FILAS][MAX_COLUMAS]){
	if(juego.chloe_visible == true){
		campo[juego.amiga_chloe.fil][juego.amiga_chloe.col] = CHLOE;
	}
}

void imprimir_elementos_campo(juego_t juego, char campo[MAX_FILAS][MAX_COLUMAS]){
	for (int i = 0; i < MAX_FILAS; i++){
		printf("\t»");
		for (int j = 0; j < MAX_COLUMAS; j++){
			if (campo[i][j]==CHLOE){
				printf(NEGRO_T BLANCO_F " %c " RESET_COLOR, campo[i][j]);
			} else if (campo[i][j]== juego.personaje.tipo){
				printf(AZUL_T " %c " RESET_COLOR, campo[i][j]);
			} else if (campo[i][j]== ARBOL){
				printf(VERDE_T " %c " RESET_COLOR, campo[i][j]);
			} else if (campo[i][j]== PIEDRA){
				printf(CYAN_T " %c " RESET_COLOR, campo[i][j]);
			} else if (campo[i][j]== KOALAS){
				printf(MAGENTA_T " %c " RESET_COLOR, campo[i][j]);
			} else if (campo[i][j]== PILA){
				printf(AMARILLO_T " %c " RESET_COLOR, campo[i][j]);
			} else if (campo[i][j]== BENGALA){
				printf(ROJO_T " %c " RESET_COLOR, campo[i][j]);
			} else{
				printf(" %c ", campo[i][j]);
			}
		}
		printf("«\t%i \n", i+1);
	}
}

void mensaje_tipo_de_jugadas(){
	printf("Juegadas validas:\n");
	printf("\tIngrese la tecla '%c': Si el personaje debe moverse para ↑.\n", ARRIBA);
	printf("\tIngrese la tecla '%c': Si el personaje debe moverse para ↓.\n", ABAJO);
	printf("\tIngrese la tecla '%c': Si el personaje debe moverse para ←.\n", IZQUIERDA);
	printf("\tIngrese la tecla '%c': Si el personaje debe moverse para →.\n", DERECHA);
	printf("\tIngrese la tecla '%c': Si el personaje quiere encender una linterna.\n",  ENCENDER_APAGAR_LINTERNA);
	printf("\tIngrese la tecla '%c': Si el personaje quiere encender una vela.\n",  ENCENDER_APAGAR_VELA);
	printf("\tIngrese la tecla '%c': Si el personaje quiere encender la bengala.\n",  ENCENDER_APAGAR_BENGALA);
	printf("\tIngrese la tecla '%c': Si Si el personaje quiere ver el tiempo restante.\n",  TIEMPO_RESTANTE);
}

void mensaje_obstaculos_herramientas(){
	printf("Obstaculos:");
	printf(VERDE_T " %c " RESET_COLOR"(Arbol),", ARBOL);
	printf(CYAN_T " %c " RESET_COLOR"(Piedra) y", PIEDRA);
	printf(MAGENTA_T " %c " RESET_COLOR"(Koala Nom Nom y sus sekoalaces)\n", KOALAS);
	printf("Herramietas:");
	printf(AMARILLO_T " %c " RESET_COLOR"(Pila, para la linterna),", PILA);
	printf(" %c (Vela) y", VELA);
	printf(ROJO_T " %c " RESET_COLOR"(Bengala)\n", BENGALA);
}

void mostrar_juego(juego_t juego){
	mensaje_tipo_de_jugadas();
	mensaje_obstaculos_herramientas();
	mensaje_si_chloe_visible(juego);

	char campo[MAX_FILAS][MAX_COLUMAS];

	crear_campo(campo);

	visibilizar_chloe(juego, campo);

	campo[juego.personaje.posicion.fil][juego.personaje.posicion.col]=juego.personaje.tipo;

	int i = 0;
	
	mostrar_obstaculos_matriz(juego, campo, MAX_ARBOLES + i, &i);
	mostrar_obstaculos_matriz(juego, campo, MAX_PIEDRAS + i, &i);
	mostrar_obstaculos_matriz(juego, campo, MAX_KOALAS + juego.cantidad_obstaculos, &i);

	i = 0;

	mostrar_herramientas_matriz(juego, campo, MAX_VELAS + i, &i);
	mostrar_herramientas_matriz(juego, campo, MAX_PILAS + i, &i);
	mostrar_herramientas_matriz(juego, campo, MAX_BENGALAS + i, &i);

	mostrar_chloe_matriz(juego, campo);

	imprimir_elementos_campo(juego, campo);
	imprimir_elementos_en_mochila(juego.personaje);
	mensaje_si_herramientas_activada(juego);
	/* printf("Cantidad de obstaculos: %i\n", juego.cantidad_obstaculos);
	printf("Cantidad de elementos: %i\n", juego.personaje.cantidad_elementos);
	printf("Elemento en uso %i\n", juego.personaje.elemento_en_uso);
	for (int i = 0; i < juego.personaje.cantidad_elementos; i++){
		
		printf("El elemento %i es una %c y tiene %i movimentos\n", i, juego.personaje.mochila[i].tipo, juego.personaje.mochila[i].movimientos_restantes);
	} */
}

void mensaje_como_jugar(personaje_t personaje){
	double tiempo_perdido_arbol = 0;
	printf("Despues de un extenso analisis de las respuestas que nos dio llegamos que usted es: ");
	if(personaje.tipo == POLAR_OSO){
		printf("- Polar (%c) -\n", POLAR_OSO);
		tiempo_perdido_arbol = SUMAR_TIEMPO_NO_PARDO_ARBOL;
	} else if(personaje.tipo == PARDO_OSO){
		printf("- Pardo (%c) -\n", PARDO_OSO);
		tiempo_perdido_arbol = SUMAR_TIEMPO_PARDO_ARBOL;
	} else if(personaje.tipo == PANDA_OSO){
		printf("- Panda (%c) -\n", PANDA_OSO);
		tiempo_perdido_arbol = SUMAR_TIEMPO_NO_PARDO_ARBOL;
	}
	printf("\nEl objetivo del juego es encontrar a Chloe (" NEGRO_T BLANCO_F " %c " RESET_COLOR") en menos de %i segundos\n", CHLOE, MAX_TIEMPO);
	printf("\nPara eso vas a tener una mochila con:\n");
	imprimir_elementos_en_mochila(personaje);
	printf("\nPero ojo porque si te encotras y te pones en la misma posicion que:\n");
	printf("\tUn arbol ("VERDE_T " %c " RESET_COLOR") vas a perder %.1f segundos\n", ARBOL, tiempo_perdido_arbol);
	printf("\tUna piedra ("CYAN_T " %c " RESET_COLOR") vas a perder %.1f segundos\n", PIEDRA, SUMAR_TIEMPO_PIEDRA);	
	printf("\tEl Koala Nom Nom o sus sekoalaces ("MAGENTA_T " %c " RESET_COLOR") vas a volver la la primera columna\n", KOALAS);
	printf("\n[Aguarde 15 segundos para que comience el juego de Los Escandalosos contra reloj©]\n");
	sleep(15);
	system("clear");
}

void inicializar_juego(juego_t* juego, char tipo_personaje){
	juego->chloe_visible=false;
	juego->obstaculos->visible=false;
	juego->herramientas->visible=false;

	juego->cantidad_obstaculos = 0;
	juego->cantidad_herramientas = 0;
	juego->personaje.tipo = tipo_personaje;
	juego->personaje.cantidad_elementos = 0;
	juego->personaje.elemento_en_uso = ELEMENTO_NO_USO;
	juego->personaje.ultimo_movimiento = SIN_MOVIMIENTO;

	cargar_chloe(&(juego->amiga_chloe));
	cargar_personaje(juego);

	inicializar_mochila(juego);

	cargar_obstaculos(juego, ARBOL, MAX_ARBOLES, juego->cantidad_obstaculos);
	cargar_obstaculos(juego, PIEDRA, MAX_PIEDRAS + juego->cantidad_obstaculos, juego->cantidad_obstaculos);

	cargar_herramientas(juego, VELA, MAX_VELAS, juego->cantidad_herramientas);
	cargar_herramientas(juego, PILA, MAX_PILAS + juego->cantidad_herramientas, juego->cantidad_herramientas);
	cargar_herramientas(juego, BENGALA, MAX_BENGALAS + juego->cantidad_herramientas, juego->cantidad_herramientas);

	mensaje_como_jugar(juego->personaje);
	mostrar_juego(*juego);
}

bool es_jugada_ingresada_valida(char jugada){
	return (jugada == ARRIBA || jugada == ABAJO || jugada == DERECHA || jugada == IZQUIERDA || jugada == ENCENDER_APAGAR_LINTERNA || jugada == ENCENDER_APAGAR_VELA || jugada == ENCENDER_APAGAR_BENGALA || jugada == TIEMPO_RESTANTE);
}

void mensaje_ingresar_jugada(char* jugada){
	printf("\nIngrese la jugada que desee realizar\n");
	scanf(" %c", jugada);
	while (!es_jugada_ingresada_valida(*jugada)){
		printf("Tecla ingresada no valida, por favor una jugada valida.");
		scanf(" %c", jugada);
	}
}

void encotrar_obstaculo_herramienta(juego_t* juego){
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
				if(juego->personaje.tipo == PARDO_OSO){
					juego->personaje.mochila[juego->personaje.cantidad_elementos].movimientos_restantes = DURACION_MOVIMIENTOS_LINTERNA_PARDO;
				} else {
					juego->personaje.mochila[juego->personaje.cantidad_elementos].movimientos_restantes = DURACION_MOVIMIENTOS_LINTERNA;
				}
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

void iluminar_elementos_linterna(juego_t *juego){
	if ((juego->personaje.ultimo_movimiento == ARRIBA)){
		for (int j = 0; j < juego->personaje.posicion.fil; j++){
			iluminar_campo(juego, j, juego->personaje.posicion.col);
		}
	} else if ((juego->personaje.ultimo_movimiento == ABAJO)){
		for (int j = juego->personaje.posicion.fil+1; j < MAX_FILAS; j++){
			iluminar_campo(juego, j, juego->personaje.posicion.col);
		}
	} else if ((juego->personaje.ultimo_movimiento == DERECHA)){
		for (int j = juego->personaje.posicion.col+1; j < MAX_COLUMAS; j++){
			iluminar_campo(juego, juego->personaje.posicion.fil, j);
		}
	} else if ((juego->personaje.ultimo_movimiento == IZQUIERDA)){
		for (int j = 0; j < juego->personaje.posicion.col; j++){
			iluminar_campo(juego, juego->personaje.posicion.fil, j);
		}
	} else {
		for (int j = juego->personaje.posicion.col + 1 ; j < MAX_COLUMAS; j++){
			iluminar_campo(juego, juego->personaje.posicion.fil, j);
		}
	}
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
}

void movimiento_linterna(juego_t *juego){
	ocultar_elementos_del_campo(juego);
	iluminar_elementos_linterna(juego);
}

void movimiento_vela(juego_t *juego){
	ocultar_elementos_del_campo(juego);		
	iluminar_elementos_alrrededor_personaje(juego, juego->personaje.posicion.fil, juego->personaje.posicion.col);
}

void mensaje_sin_herramienta_mochila(char herramienta[MAX_PALABRA]){
	 printf("Te quedastes sin %s\n", herramienta);
}

bool se_movio_personaje(char movimiento){
	return((movimiento == ARRIBA) || (movimiento == ABAJO) || (movimiento == DERECHA) || (movimiento == IZQUIERDA));
}

void ordenar_elementos_mochila(elemento_mochila_t mochila[MAX_HERRAMIENTAS], int tope){
	int i,j;
	elemento_mochila_t aux;
	for (i = 1; i < tope; i++){
		j = i;
		aux = mochila[i];
		while( (j > 0) && (aux.movimientos_restantes < mochila[j - 1].movimientos_restantes) ){
			mochila[j] = mochila[j - 1];
			j--;
		}
		mochila[j] = aux;
	}
}

void cargar_koala(juego_t* juego, char item, int tope, int primer_item, char posicion){
	int posicion_fil = 0;
	int posicion_col = 0;
	for (int i = primer_item ; i < tope; i++){
		juego->obstaculos[i].tipo = item;
		switch (posicion){
			case ARRIBA:
				posicion_fil = rand() % juego->personaje.posicion.fil;
				posicion_col = juego->personaje.posicion.col;
				break;
			case ABAJO:
				posicion_fil = juego->personaje.posicion.fil + rand() % MAX_FILAS;
				posicion_col = juego->personaje.posicion.col;
				break;
			case IZQUIERDA:
				posicion_fil = juego->personaje.posicion.fil;
				posicion_col = rand() % juego->personaje.posicion.col;
				break;
			case DERECHA:
				posicion_fil = juego->personaje.posicion.fil;
				posicion_col = juego->personaje.posicion.col + rand() % MAX_COLUMAS;
				break;
			default:
				posicion_fil = juego->personaje.posicion.fil;
				posicion_col = rand() % MAX_COLUMAS;
				break;
		}
		while (hay_algo_en_posicion(*juego, posicion_fil, posicion_col)){
			switch (posicion){
				case ARRIBA:
					posicion_fil = rand() % juego->personaje.posicion.fil;
					break;
				case ABAJO:
					posicion_fil = juego->personaje.posicion.fil + rand() % MAX_FILAS;
					break;
				case IZQUIERDA:
					posicion_col = rand() % juego->personaje.posicion.col;
					break;
				case DERECHA:
					posicion_col = juego->personaje.posicion.col + rand() % MAX_COLUMAS;
					break;
				default:
					posicion_col = rand() % MAX_COLUMAS;
					break;
			}
		}
		juego->obstaculos[i].posicion.fil = posicion_fil;
		juego->obstaculos[i].posicion.col = posicion_col;
		juego->cantidad_obstaculos++;
	}
}

void realizar_jugada(juego_t* juego, char jugada){
	int fila_bengala = 0;
	int columna_bengala = 0;
	int cantidad_pilas, cantidad_velas, cantidad_bengalas;
	int i = 0;

	mensaje_ingresar_jugada(&jugada);

	if(se_movio_personaje(jugada)){
		juego->personaje.ultimo_movimiento = jugada;
	}

	switch (jugada){
	case ARRIBA:
		if(juego->personaje.posicion.fil > 0){
			juego->personaje.posicion.fil--;
			if(juego->personaje.elemento_en_uso != ELEMENTO_NO_USO)
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
			encotrar_obstaculo_herramienta(juego);
		}
		system("clear");
		break;
	case ABAJO:
		if(juego->personaje.posicion.fil < MAX_FILAS-1){
			juego->personaje.posicion.fil++;
			if(juego->personaje.elemento_en_uso != ELEMENTO_NO_USO)
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
			encotrar_obstaculo_herramienta(juego);
		}
		system("clear");
		break;
	case DERECHA:
		if(juego->personaje.posicion.col < MAX_COLUMAS-1){
			juego->personaje.posicion.col++;
			if(juego->personaje.elemento_en_uso != ELEMENTO_NO_USO)
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
			encotrar_obstaculo_herramienta(juego);
		}
		system("clear");
		break;
	case IZQUIERDA:
		if(juego->personaje.posicion.col > 0){
			juego->personaje.posicion.col--;
			if(juego->personaje.elemento_en_uso != ELEMENTO_NO_USO)
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
			encotrar_obstaculo_herramienta(juego);
		}
		system("clear");
		break;
	case ENCENDER_APAGAR_LINTERNA:
		system("clear");
		cantidad_pilas = calcular_cantidad_herramienta(juego, PILA);
		if(cantidad_pilas <= SIN_HERRAMIENTA){
			mensaje_sin_herramienta_mochila(STR_PILAS);
		} else if(juego->personaje.mochila [juego->personaje.elemento_en_uso].tipo == BENGALA){
			printf("La bengala esta en uso esta en uso maestro!!!\n");
		} else if(juego->personaje.mochila [juego->personaje.elemento_en_uso].tipo == PILA){
			ocultar_elementos_del_campo(juego);
			intercambio_herramienta_uso_con_ultima(juego, juego->personaje.elemento_en_uso);
			juego->personaje.elemento_en_uso = ELEMENTO_NO_USO;
			printf("Linterna desactivada\n");
		} else {
			ocultar_elementos_del_campo(juego);
			cargar_koala(juego, KOALAS, MAX_KOALAS + juego->cantidad_obstaculos, juego->cantidad_obstaculos, juego->personaje.ultimo_movimiento);
			while((juego->personaje.mochila[i].tipo != PILA) && (i < juego->personaje.cantidad_elementos)){
				i++;
			}
			if(juego->personaje.mochila[i].tipo == PILA){
				juego->personaje.elemento_en_uso = i;
				iluminar_elementos_linterna(juego);
				intercambio_herramienta_uso_con_ultima(juego, juego->personaje.elemento_en_uso);
				juego->personaje.elemento_en_uso = juego->personaje.cantidad_elementos-1;
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
			}
			cantidad_pilas--;
		} 
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
				juego->personaje.elemento_en_uso = i;
				iluminar_elementos_alrrededor_personaje(juego, juego->personaje.posicion.fil, juego->personaje.posicion.col);
				intercambio_herramienta_uso_con_ultima(juego, juego->personaje.elemento_en_uso);
				juego->personaje.elemento_en_uso = juego->personaje.cantidad_elementos-1;
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
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
				juego->personaje.elemento_en_uso = i;
				fila_bengala = rand() % MAX_FILAS;
				columna_bengala = rand() % MAX_COLUMAS;
				iluminar_distacia_mahattam(juego, fila_bengala, columna_bengala);
				intercambio_herramienta_uso_con_ultima(juego, i);
				juego->personaje.elemento_en_uso = juego->personaje.cantidad_elementos-1;
			}
			cantidad_bengalas--;
		} else {
			printf("La bengala esta en uso esta en uso maestro!!!\n");
		}
		break;
	case TIEMPO_RESTANTE:
		system("clear");
		printf("»Te quedan: %.1f segundos.«\n", MAX_TIEMPO - tiempo_actual());
		break;
	}

	if(se_movio_personaje(jugada)){
		if(juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == PILA)
			movimiento_linterna(juego);
		if(juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == VELA)
			movimiento_vela(juego);
	}

	if(juego->personaje.elemento_en_uso == ELEMENTO_NO_USO){
		ordenar_elementos_mochila(juego->personaje.mochila, juego->personaje.cantidad_elementos);
	}

	if((juego->personaje.elemento_en_uso != ELEMENTO_NO_USO) && (juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes <= 0)){
		intercambio_herramienta_uso_con_ultima(juego, juego->personaje.elemento_en_uso);
		juego->personaje.cantidad_elementos--;
		ocultar_elementos_del_campo(juego);
		
		juego->personaje.elemento_en_uso = ELEMENTO_NO_USO;
	}
	for (int i = 0; i < juego->personaje.cantidad_elementos; i++){
		if(juego->personaje.mochila[i].movimientos_restantes <= 0){
			intercambio_herramienta_uso_con_ultima(juego, i);
			juego->personaje.cantidad_elementos--;
		}
	}
	mostrar_juego(*juego);
}

int estado_juego(juego_t juego){
	
	int estado_juego = EN_JUEGO;
	if (es_posicion_personaje_posicion_chloe(juego)){
		estado_juego = JUEGO_FINALIZADO;
	}
	return estado_juego;
}

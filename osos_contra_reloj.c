#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "test_de_personalidad.h"
#include "osos_contra_reloj.h"
#include "utiles.h"

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
#define EMOJI_POLAR_OSO "\U0001f43b\u200D\u2744\uFE0F"
#define EMOJI_PANDA_OSO "\U0001f43c"
#define EMOJI_PARDO_OSO "\U0001f43b"
#define EMOJI_ARRIBA "\u2B06\uFE0F"
#define EMOJI_ABAJO "\u2B07\uFE0F"
#define EMOJI_DERECHA "\u27A1\uFE0F"
#define EMOJI_IZQUIERDA "\u2B05\uFE0F"
#define EMOJI_ARBOL "\U0001f333"
#define EMOJI_PIEDRA "\U0001faa8"
#define EMOJI_KOALAS "\U0001f428"
#define EMOJI_LINTERNA "\U0001f526"
#define EMOJI_VELA "\U0001f56f\uFE0F"
#define EMOJI_BENGALA "\U0001f9e8"
#define EMOJI_PILA "\U0001f50b"
#define EMOJI_CHLOE "\U0001f478"
#define EMOJI_OSCURIDAD "\U0001f7eb"
#define MAX_PALABRA 256

const int MAX_TIEMPO = 120; 
const int MAX_FILAS = 20;
const int MAX_COLUMNAS = 30;
const int MAX_ARBOLES = 350;
const int MAX_PIEDRAS = 80;
const int MAX_KOALAS = 1;
const int MAX_PILAS = 30;
const int MAX_VELAS = 30;
const int MAX_BENGALAS = 10;
const char OSCURIDAD = '-';
const int TIEMPO_PERDIDO_PANDA = 10;

const char ARBOL = 'A';
const char PIEDRA = 'R';
const char KOALAS = 'K';
const char LINTERNA = 'L';
const char VELA = 'V';
const char BENGALA = 'E';
const char PILA = 'B';
const char CHLOE = 'C';

const int CANTIDAD_LINTERNA_INICIO = 1;
const int DURACION_LINTERNA = 10;
const int DURACION_LINTERNA_PARDO = 15;

const int CANTIDAD_VELAS_INICIO = 4;
const int CANTIDAD_VELAS_INICIO_POLAR = 6;
const int DURACION_VELA = 5;

const int CANTIDAD_BENGALAS_INICIO = 0;
const int CANTIDAD_BENGALAS_INICIO_PANDA = 2;
const int DURACION_BENGALA = 3;

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

/*
 * Pre: -
 * Post: Devolvera tanto una fila random y una columna random para chloe
 */
void cargar_chloe(coordenada_t* chloe){
	chloe->fil = rand() % MAX_FILAS;
	chloe->col = rand() % MAX_COLUMNAS;	
}

/*
 * Pre: Necesito que tener una determinada fila y columna, que el personaje y CHLOE ya tengan un determianda posicion y que el tope_obstaculos y tope_herramientas no sea vacio
 * Post: Me devolvera TRUE si en esa posicion esa ocupada ya sea con el personaje, CHLOE, algun obstaculo o alguna herramienta y FALSE en caso contrario
 */
bool hay_algo_en_posicion(juego_t juego, coordenada_t posicion, char elemento){
    bool hay_algo_en_posicion = false;
	if ((elemento != juego.personaje.tipo) && (juego.personaje.posicion.fil == posicion.fil) && (juego.personaje.posicion.col == posicion.col)){
            hay_algo_en_posicion = true;
	} else if ( (juego.amiga_chloe.fil == posicion.fil) && (juego.amiga_chloe.col == posicion.col)){
        hay_algo_en_posicion = true;
	}
    for (int i = 0; i < juego.cantidad_obstaculos; i++){
        if ((posicion.fil == juego.obstaculos[i].posicion.fil) && (posicion.col == juego.obstaculos[i].posicion.col)){
            hay_algo_en_posicion = true;
		}
    }
	for (int i = 0; i < juego.cantidad_herramientas; i++){
        if ((juego.herramientas[i].posicion.fil == posicion.fil) && (juego.herramientas[i].posicion.col == posicion.col)){
            hay_algo_en_posicion = true;
		}
    }
    return hay_algo_en_posicion;
}

/*
 * Pre: -
 * Post: Devolvera una fila random en la columna 0 para el personaje
 */
void cargar_personaje(juego_t* juego){
	juego->personaje.posicion.fil = rand() % MAX_FILAS;
	juego->personaje.posicion.col = 0;
	
	while (hay_algo_en_posicion(*juego, juego->personaje.posicion, juego->personaje.tipo))
		juego->personaje.posicion.fil = rand() % MAX_FILAS;
}

/*
 * Pre: Necesito tanto que el primer_item este inicializado y que no sea >= al tope
 * Post: Devolvera la herramienta pasada por parametro, con los respectivos movimiento
 */
void cargar_mochila_inicio(personaje_t* personaje, char herramienta, int tope, int duracion_movimiento){
	for (int i = personaje->cantidad_elementos; i < tope; i++){
		personaje->mochila[i].tipo = herramienta;
		personaje->mochila[i].movimientos_restantes = duracion_movimiento;
		personaje->cantidad_elementos++;
	}
}

/*
 * Pre: Necesito que el personaje ya este inicializado con PARDO_OSO, POLAR_OSO o PANDA_OSO
 * Post: Me devolvera la mochila inicializada con las tres herramientas, con sus respectivos movimientos y la cantidad de elementos que tiene la mochila
 */
void inicializar_mochila(personaje_t* personaje){
	int cantidad_inicio_vela = 0;
	int cantidad_inicio_bengala = 0;

	if(personaje->tipo == PARDO_OSO)
		cargar_mochila_inicio(personaje, LINTERNA, CANTIDAD_LINTERNA_INICIO, DURACION_LINTERNA_PARDO);
	else
		cargar_mochila_inicio(personaje, LINTERNA, CANTIDAD_LINTERNA_INICIO, DURACION_LINTERNA);
	
	if(personaje->tipo == POLAR_OSO)
		cantidad_inicio_vela = CANTIDAD_VELAS_INICIO_POLAR;
	else
		cantidad_inicio_vela = CANTIDAD_VELAS_INICIO;
	cargar_mochila_inicio(personaje, VELA, (CANTIDAD_LINTERNA_INICIO + cantidad_inicio_vela), DURACION_VELA);
	
	if(personaje->tipo == PANDA_OSO)
		cantidad_inicio_bengala = CANTIDAD_BENGALAS_INICIO_PANDA;
	else
		cantidad_inicio_bengala = CANTIDAD_BENGALAS_INICIO;
	cargar_mochila_inicio(personaje, BENGALA, (CANTIDAD_LINTERNA_INICIO + cantidad_inicio_vela + cantidad_inicio_bengala), DURACION_BENGALA);
}

void generar_posicion_ramdom(coordenada_t* posicion){
	posicion->fil = rand() % MAX_FILAS;
	posicion->col = rand() % MAX_COLUMNAS;	
}

/*
 * Pre: Necesito que tener un tope inicializado y no vacio, que esl primer_item sea menor al tope y que el item este inicializado
 * Post: Me devolvera una posicion random para cada obstaculo
 */
void cargar_obstaculos(juego_t* juego, char obstaculo, int tope_obstaculo){	
	coordenada_t posicion;
	
	for (int i = juego->cantidad_obstaculos ; i < tope_obstaculo; i++){
		juego->obstaculos[i].tipo = obstaculo;
		generar_posicion_ramdom(&posicion);
		
		while (hay_algo_en_posicion(*juego, posicion, juego->obstaculos[i].tipo))
			generar_posicion_ramdom(&posicion);
		
		juego->obstaculos[i].posicion.fil = posicion.fil;
		juego->obstaculos[i].posicion.col = posicion.col;
		juego->obstaculos[i].visible = false;
		juego->cantidad_obstaculos++;
	}
}

/*
 * Pre: Necesito que tener un tope inicializado y no vacio, que el primer_item sea menor al tope y que el item este inicializado
 * Post: Me devolvera una posicion random para cada herramienta
 */
void cargar_herramientas(juego_t* juego, char herramienta, int tope_herramienta){
	coordenada_t posicion;
	
	for (int i = juego->cantidad_herramientas ; i < tope_herramienta; i++){
		juego->herramientas[i].tipo = herramienta;
		generar_posicion_ramdom(&posicion);
		
		while (hay_algo_en_posicion(*juego, posicion, juego->herramientas[i].tipo))
			generar_posicion_ramdom(&posicion);
		
		juego->herramientas[i].posicion.fil = posicion.fil;
		juego->herramientas[i].posicion.col = posicion.col;
		juego->herramientas[i].visible = false;
		juego->cantidad_herramientas++;
	}
}

/*
 * Pre: Necesito que tener personaje.cantidad_elementos inicializados y no vacios.
 * Post: Me va imprimir los elementos de la mochila con sus respectivos movientos.
 */
void imprimir_mochila(personaje_t personaje){	
	int contador_linternas = 0, contador_velas = 0, contador_bengalas = 0;
	int mov_linternas = 0, mov_velas = 0, mov_bengalas = 0;
	for (int i = 0; i < personaje.cantidad_elementos; i++){
		if (personaje.mochila[i].tipo == LINTERNA){
			contador_linternas++;
			mov_linternas = personaje.mochila[i].movimientos_restantes;
		} else if (personaje.mochila[i].tipo == VELA){
			contador_velas++;
			mov_velas = personaje.mochila[i].movimientos_restantes;
		} else if (personaje.mochila[i].tipo == BENGALA){
			contador_bengalas++;
			mov_bengalas = personaje.mochila[i].movimientos_restantes;
		}
	}
	printf("\t\t»Tenes %sx%i con %i movimientos cada una«\n",EMOJI_LINTERNA, contador_linternas, mov_linternas);
	printf("\t\t»Tenes %s x%i con %i movimientos cada una«\n",EMOJI_VELA ,contador_velas, mov_velas);
	printf("\t\t»Tenes %sx%i con %i movimientos cada una«\n",EMOJI_BENGALA ,contador_bengalas, mov_bengalas);
}

/*
 * Pre: -
 * Post: Mostrará las instrucciones de juego juego por pantalla.
 */
void mensaje_como_jugar(personaje_t personaje){
	double tiempo_perdido_arbol = 0;
	printf("Despues de un extenso analisis de las respuestas que nos dio llegamos que usted es: ");
	if(personaje.tipo == POLAR_OSO){
		printf("- Polar (%s) -\n", EMOJI_POLAR_OSO);
		tiempo_perdido_arbol = SUMAR_TIEMPO_NO_PARDO_ARBOL;
	} else if(personaje.tipo == PARDO_OSO){
		printf("- Pardo (%s) -\n", EMOJI_PARDO_OSO);
		tiempo_perdido_arbol = SUMAR_TIEMPO_PARDO_ARBOL;
	} else if(personaje.tipo == PANDA_OSO){
		printf("- Panda (%s) -\n", EMOJI_PANDA_OSO);
		tiempo_perdido_arbol = SUMAR_TIEMPO_NO_PARDO_ARBOL;
	}
	printf("\nEl objetivo del juego es encontrar a Chloe ( %s ) en menos de %i segundos\n", EMOJI_CHLOE, MAX_TIEMPO);
	printf("\nPara eso vas a tener una mochila con:\n");
	imprimir_mochila(personaje);
	printf("\nPero ojo porque si te encontras y te pones en la misma posicion que:\n");
	printf("\tUn arbol ( %s ) vas a perder %.1f segundos\n", EMOJI_ARBOL, tiempo_perdido_arbol);
	printf("\tUna piedra ( %s  ) vas a perder %.1f segundos\n", EMOJI_PIEDRA, SUMAR_TIEMPO_PIEDRA);	
	printf("\tEl Koala Nom Nom o sus sekoalaces ( %s ) vas a volver la la primera columna\n", EMOJI_KOALAS);
	printf("\n[Aguarde 15 segundos para que comience el juego de Los Escandalosos contra reloj©]\n");
	sleep(15);
	system("clear");
}

/*
 * Pre: -
 * Post: Inicializará el juego, cargando toda la información inicial y los datos del personaje. 
 */
void inicializar_juego(juego_t* juego, char tipo_personaje){
	juego->chloe_visible=false;

	juego->cantidad_obstaculos = 0;
	juego->cantidad_herramientas = 0;
	juego->personaje.tipo = tipo_personaje;
	juego->personaje.cantidad_elementos = 0;
	juego->personaje.elemento_en_uso = ELEMENTO_NO_USO;
	juego->personaje.ultimo_movimiento = SIN_MOVIMIENTO;

	cargar_chloe(&(juego->amiga_chloe));
	cargar_personaje(juego);

	inicializar_mochila(&juego->personaje);

	cargar_obstaculos(juego, ARBOL, MAX_ARBOLES);
	cargar_obstaculos(juego, PIEDRA, MAX_PIEDRAS + juego->cantidad_obstaculos);
	cargar_obstaculos(juego, KOALAS, MAX_KOALAS + juego->cantidad_obstaculos);

	cargar_herramientas(juego, VELA, MAX_VELAS);
	cargar_herramientas(juego, PILA, MAX_PILAS + juego->cantidad_herramientas);
	cargar_herramientas(juego, BENGALA, MAX_BENGALAS + juego->cantidad_herramientas);
	
	mensaje_como_jugar(juego->personaje);
}

/*
 * Pre: -
 * Post: Me va imprimir todas las jugadas validas para el usuario.
 */
void mensaje_tipo_de_jugadas(){
	printf("Jugadas validas:\n");
	printf("\tIngrese la tecla '%c': Si el personaje debe moverse para %s\n", ARRIBA, EMOJI_ARRIBA);
	printf("\tIngrese la tecla '%c': Si el personaje debe moverse para %s\n", ABAJO, EMOJI_ABAJO );
	printf("\tIngrese la tecla '%c': Si el personaje debe moverse para %s\n", IZQUIERDA, EMOJI_IZQUIERDA);
	printf("\tIngrese la tecla '%c': Si el personaje debe moverse para %s\n", DERECHA, EMOJI_DERECHA);
	printf("\tIngrese la tecla '%c': Si el personaje quiere encender la %s\n",  ENCENDER_APAGAR_LINTERNA, EMOJI_LINTERNA);
	printf("\tIngrese la tecla '%c': Si el personaje quiere encender una %s\n",  ENCENDER_APAGAR_VELA, EMOJI_VELA);
	printf("\tIngrese la tecla '%c': Si el personaje quiere encender una %s\n",  ENCENDER_APAGAR_BENGALA, EMOJI_BENGALA);
	printf("\tIngrese la tecla '%c': Si Si el personaje quiere ver el tiempo restante.\n",  TIEMPO_RESTANTE);
}

/*
 * Pre: -
 * Post: Me va imprimir todos los tipos de obstaculos y herramientas que hay para el usuario.
 */
void mensaje_obstaculos_herramientas(){
	printf("Obstaculos:");
	printf(" %s  (Arbol),", EMOJI_ARBOL);
	printf(" %s  ""(Piedra) y", EMOJI_PIEDRA);
	printf(" %s ""(Koala Nom Nom y sus sekoalaces)\n", EMOJI_KOALAS);
	printf("Herramietas:");
	printf(" %s ""(Pila, para la %s),", EMOJI_PILA, EMOJI_LINTERNA);
	printf(" %s (Vela) y", EMOJI_VELA);
	printf(" %s ""(Bengala)\n", EMOJI_BENGALA);
}

/*
 * Pre: -
 * Post: Me va a crear una matriz de MAX_FILASxMAX_COLUMNAS
 */
void crear_campo(char campo[MAX_FILAS][MAX_COLUMNAS]){
	for (int i = 0; i < MAX_FILAS; i++){
		for (int j = 0; j < MAX_COLUMNAS; j++){
			campo[i][j]= OSCURIDAD;	
		}
	}
}

/*
 * Pre: -
 * Post: Si CHLOE tiene visible igual a TRUE la va a mostrar en la matriz.
 */
void mostrar_chloe_matriz(juego_t juego, char campo[MAX_FILAS][MAX_COLUMNAS], bool chloe){
	if(chloe == true)
		campo[juego.amiga_chloe.fil][juego.amiga_chloe.col] = CHLOE;
}

/*
 * Pre: Necesito un tope inicializado y no nulo, que el primer_obstaculo sea menor que el tope.
 * Post: Si el obstaculo tiene visible igual a TRUE lo va a mostrar en la matriz.
 */
void mostrar_obstaculos_matriz(juego_t juego, char campo[MAX_FILAS][MAX_COLUMNAS]){
	for (int i = 0; i < juego.cantidad_obstaculos; i++){
		if(juego.obstaculos[i].visible == true)
			campo[juego.obstaculos[i].posicion.fil][juego.obstaculos[i].posicion.col] = juego.obstaculos[i].tipo;
	}
}

/*
 * Pre: Necesito un tope inicializado y no nulo, que el primer_herramientas sea menor que el tope.
 * Post: Si la herramienta tiene visible igual a TRUE la va a mostrar en la matriz.
 */
void mostrar_herramientas_matriz(juego_t juego, char campo[MAX_FILAS][MAX_COLUMNAS]){
	for (int i = 0; i < juego.cantidad_herramientas; i++){
		if(juego.herramientas[i].visible == true)
			campo[juego.herramientas[i].posicion.fil][juego.herramientas[i].posicion.col] = juego.herramientas[i].tipo;
	}
}

/*
 * Pre: Necesito que personaje sea PARDO_OSO, PANDA_OSO o POLAR_OSO
 * Post: Me va imprimir todos los elementos del campo con su respectivo emoji
 */
void imprimir_elementos_campo(char personaje, char campo[MAX_FILAS][MAX_COLUMNAS]){
	for (int i = 0; i < MAX_FILAS; i++){
		printf("\t»");
		for (int j = 0; j < MAX_COLUMNAS; j++){
			if (campo[i][j]==CHLOE){
				printf(" %s ", EMOJI_CHLOE);
			} else if (campo[i][j]== personaje){
				if(personaje == PANDA_OSO){
					printf(" %s ", EMOJI_PANDA_OSO);
				} else if(personaje == PARDO_OSO){
					printf(" %s ", EMOJI_PARDO_OSO);
				} else if(personaje == POLAR_OSO){
					printf(" %s ", EMOJI_POLAR_OSO);
				}
			} else if (campo[i][j]== ARBOL){
				printf(" %s ", EMOJI_ARBOL);
			} else if (campo[i][j]== PIEDRA){
				printf(" %s  ", EMOJI_PIEDRA);
			} else if (campo[i][j]== KOALAS){
				printf(" %s ", EMOJI_KOALAS);
			} else if (campo[i][j]== PILA){
				printf(" %s ", EMOJI_PILA);
			} else if (campo[i][j]== VELA){
				printf(" %s  ", EMOJI_VELA);
			} else if (campo[i][j]== BENGALA){
				printf(" %s ", EMOJI_BENGALA);
			} else if (campo[i][j]== OSCURIDAD){
				printf(" %s ", EMOJI_OSCURIDAD);
			}
		}
		printf("«\n");
	}
}

/*
 * Pre: -
 * Post: Me mostras un mensaje si alguna herramienta fue activada
 */
void mensaje_si_herramientas_activada(personaje_t personaje){
	if(personaje.mochila [personaje.elemento_en_uso].tipo == LINTERNA)
		printf("%s Activada con %i movimientos!!\n", EMOJI_LINTERNA, personaje.mochila [personaje.elemento_en_uso].movimientos_restantes);
	if(personaje.mochila [personaje.elemento_en_uso].tipo == VELA)
		printf("%s  Activada con %i movimientos!!\n", EMOJI_VELA, personaje.mochila [personaje.elemento_en_uso].movimientos_restantes);
	if(personaje.mochila [personaje.elemento_en_uso].tipo == BENGALA)
		printf("%s Activada con %i movimientos!!\n", EMOJI_BENGALA, personaje.mochila [personaje.elemento_en_uso].movimientos_restantes);
}

/*
 * Pre: -
 * Post: Me mostras un mensaje si CHLOE es visible
 */
void mensaje_si_chloe_visible(bool chloe_visible){
	if(chloe_visible == true)
		printf("%s visible!! Run Forest run»»»\n", EMOJI_CHLOE);
}

/*
 * Pre: -
 * Post: Mostrará el juego por pantalla.
 */
void mostrar_juego(juego_t juego){
	mensaje_tipo_de_jugadas();
	mensaje_obstaculos_herramientas();

	char campo[MAX_FILAS][MAX_COLUMNAS];

	crear_campo(campo);

	campo[juego.personaje.posicion.fil][juego.personaje.posicion.col]=juego.personaje.tipo;

	mostrar_chloe_matriz(juego, campo, juego.chloe_visible);

	mostrar_obstaculos_matriz(juego, campo);

	mostrar_herramientas_matriz(juego, campo);

	imprimir_elementos_campo(juego.personaje.tipo, campo);
	imprimir_mochila(juego.personaje);
	mensaje_si_herramientas_activada(juego.personaje);
	mensaje_si_chloe_visible(juego.chloe_visible);

	printf("Cantidad de elementos: %i\n", juego.personaje.cantidad_elementos);
	for(int i = 0; i < juego.personaje.cantidad_elementos; i++){
		printf("[%i] Herramienta: %c con %i mov\n", i, juego.personaje.mochila[i].tipo, juego.personaje.mochila[i].movimientos_restantes);
	}
}

/*
 * Pre: -
 * Post: Me va a devolver TRUE si jugada es ARRIBA, ABAJO, DERECHA, IZQUIERDA, ENCENDER_APAGAR_LINTERNA, ENCENDER_APAGAR_VELA, ENCENDER_APAGAR_BENGALA o TIEMPO_RESTANTE
 */
bool es_jugada_valida(char jugada){
	return (jugada == ARRIBA || jugada == ABAJO || jugada == DERECHA || jugada == IZQUIERDA || jugada == ENCENDER_APAGAR_LINTERNA || jugada == ENCENDER_APAGAR_VELA || jugada == ENCENDER_APAGAR_BENGALA || jugada == TIEMPO_RESTANTE);
}

/*
 * Pre: -
 * Post: Devolvera TRUE si el personaje ingreso por teclado (ARRIBA, ABAJO, DERECHA o IZQUIERDA).
 */
bool se_movio_personaje(char movimiento){
	return((movimiento == ARRIBA) || (movimiento == ABAJO) || (movimiento == DERECHA) || (movimiento == IZQUIERDA));
}

/*
 * Pre: -
 * Post: Si el personaje se encuentra en la misma posicion de que un determinado obstaculo perdera tiempo, dependiendo de que obstaculo es y dependiendo de que personaje es, pero si el obstaculo es KOALA volvera la columna 0 en una fila random 
 */
void encontrar_obstaculo(juego_t* juego){
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
		}
	}
}

/*
 * Pre: -
 * Post: Si el personaje se encuentra en la misma posicion de que una determianda herramienta esta ingresara a su mochila con su respectivo movimiento, dependiendo del tipo de personaje que sea y eliminado esta herramienta de la matriz.
 */
void encontrar_herramienta(juego_t* juego){
	for (int i = 0; i < juego->cantidad_herramientas; i++){
		if((juego->personaje.posicion.fil == juego->herramientas[i].posicion.fil) && (juego->personaje.posicion.col == juego->herramientas[i].posicion.col)){
			if(juego->herramientas[i].tipo == PILA){
				int j = 0;
				while(juego->personaje.mochila[j].tipo != LINTERNA){
					j++;
				}
				if(juego->personaje.tipo == PARDO_OSO){
					juego->personaje.mochila[j].movimientos_restantes += DURACION_LINTERNA_PARDO;
				} else {
					juego->personaje.mochila[j].movimientos_restantes += DURACION_LINTERNA;
				}
			} else if(juego->herramientas[i].tipo == VELA){
				juego->personaje.mochila[juego->personaje.cantidad_elementos].tipo = VELA;
				juego->personaje.mochila[juego->personaje.cantidad_elementos].movimientos_restantes = DURACION_VELA;
			} else if(juego->herramientas[i].tipo == BENGALA){
				juego->personaje.mochila[juego->personaje.cantidad_elementos].tipo = BENGALA;
				juego->personaje.mochila[juego->personaje.cantidad_elementos].movimientos_restantes = DURACION_BENGALA;
			}
			juego->personaje.cantidad_elementos++;
			juego->herramientas[i] = juego->herramientas[juego->cantidad_herramientas-1];
			juego->cantidad_herramientas--;
		}
	}
}

/*
 * Pre: Necesito que el tope_obstaculos y el tope_herramientas esten inicializados y no nulos
 * Post: Si un algun obstaculo, alguna herramienta o CHLOE tenia visible a TRUE me los va a cambiar a FALSE.
 */
void ocultar_elementos_del_campo(juego_t* juego){
	for (int i = 0; i < juego->cantidad_obstaculos; i++){
		if(juego->obstaculos[i].visible == true)
			juego->obstaculos[i].visible = false;
	}
	for (int i = 0; i < juego->cantidad_herramientas; i++){
		if(juego->herramientas[i].visible == true)
			juego->herramientas[i].visible = false;
	}
	if(juego->chloe_visible == true)
		juego->chloe_visible = false;
}

/*
 * Pre: -
 * Post: Va a devolver un mesaje que se quedo por pantalla que se quedo sin la herramienta deseada.
 */	
void mensaje_sin_herramienta_mochila(char herramienta[MAX_PALABRA]){
	 printf("Te quedastes sin %s\n", herramienta);
}

/*
 * Pre: -
 * Post: Si la posicion que le paso por parametro(posicion_fil, posicion_col) es igual a un obstaculo o herramienta o CHLOE me va a cambiar cada una visible a TRUE.
 */
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

/*
 * Pre: -
 * Post: Dependiendo de ultimo_movimiento va a cambiar visible a TRUE si hay algun obstaculo, alguna herramienta o CHLOE
 */
void iluminar_elementos_linterna(juego_t *juego, char ultimo_movimiento, coordenada_t posicion){
	if ((ultimo_movimiento == ARRIBA)){
		for (int j = 0; j < posicion.fil; j++){
			iluminar_campo(juego, j, posicion.col);
		}
	} else if ((ultimo_movimiento == ABAJO)){
		for (int j = posicion.fil+1; j < MAX_FILAS; j++){
			iluminar_campo(juego, j, posicion.col);
		}
	} else if ((ultimo_movimiento == DERECHA)){
		for (int j = posicion.col+1; j < MAX_COLUMNAS; j++){
			iluminar_campo(juego, posicion.fil, j);
		}
	} else if ((ultimo_movimiento == IZQUIERDA)){
		for (int j = 0; j < posicion.col; j++){
			iluminar_campo(juego, posicion.fil, j);
		}
	} else {
		for (int j = posicion.col + 1 ; j < MAX_COLUMNAS; j++){
			iluminar_campo(juego, posicion.fil, j);
		}
	}
}

void activar_linterna(juego_t* juego){
	int i = 0;

	while((juego->personaje.mochila[i].tipo != LINTERNA) && (i < juego->personaje.cantidad_elementos)){
		i++;
	}
	
	if(juego->personaje.mochila[i].tipo == LINTERNA){
		juego->personaje.elemento_en_uso = i;
		iluminar_elementos_linterna(juego, juego->personaje.ultimo_movimiento, juego->personaje.posicion);
		juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
	}
}

/*
 * Pre: -
 * Post: Si la herramienta esta en la mochila me va a contar la cantidad que tengo.
 */
int calcular_cantidad_herramienta(personaje_t personaje, char herramienta){
	int cantidad_herramienta = SIN_HERRAMIENTA;
	for (int i = 0; i < personaje.cantidad_elementos; i++){
		if(personaje.mochila[i].tipo == herramienta)
			cantidad_herramienta++;
	}
	return cantidad_herramienta;
}

/*
 * Pre: -
 * Post: Alrededor del personaje va a cambiar visible a TRUE si hay algun obstaculo, alguna herramienta o CHLOE 
 */	
void iluminar_elementos_vela(juego_t* juego, int fila, int columna){
	for (int i = fila-1; i <= fila+1; i++){
		if((i >= 0) && (i < MAX_FILAS)){
			for (int j = columna-1; j <= columna+1; j++){
				if((j >= 0 ) && (j < MAX_COLUMNAS)){
					if((i != juego->personaje.posicion.fil) || (j != juego->personaje.posicion.col)){
						iluminar_campo(juego, i, j);
					}
				}
			}
		}
	}
}

/*
 * Pre: -
 * Post: Con la posicion (fila,columna) y una distancia manhattam de DISTANCIA_MANHATTAM  va a cambiar visible a TRUE si hay algun obstaculo, alguna herramienta o CHLOE.
 */	
void iluminar_distacia_mahattam(juego_t* juego, coordenada_t posicion){
	for (int i = 0; i < MAX_FILAS; i++){
		for (int j = 0; j < MAX_COLUMNAS; j++){
			if((fabs (posicion.fil - i) + fabs (posicion.col - j)) <= DISTANCIA_MANHATTAM){
				iluminar_campo(juego, i, j);
			}
		}
	}
}

void activar_vela(juego_t* juego){
	int i = 0;

	while((juego->personaje.mochila[i].tipo != VELA) && (i < juego->personaje.cantidad_elementos))
		i++;

	if(juego->personaje.mochila[i].tipo == VELA){
		juego->personaje.elemento_en_uso = i;
		iluminar_elementos_vela(juego, juego->personaje.posicion.fil, juego->personaje.posicion.col);
		juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
	}
}

void activar_bengala(juego_t* juego){
	int i = 0;
	coordenada_t posicion_bengala;
	
	while((juego->personaje.mochila[i].tipo != BENGALA) && (i < juego->personaje.cantidad_elementos))
		i++;

	if(juego->personaje.mochila[i].tipo == BENGALA){
		juego->personaje.elemento_en_uso = i;
		generar_posicion_ramdom(&posicion_bengala);
		iluminar_distacia_mahattam(juego, posicion_bengala);
		juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
	}
}

/*
 * Pre: -
 * Post: Va a volver visible igual a TRUE si se topa con algun obstaculo, alguna herramienta o CHLOE de mientras el personaje se mueva con la linterna prendida
 */
void movimiento_linterna(juego_t *juego){
	ocultar_elementos_del_campo(juego);
	iluminar_elementos_linterna(juego, juego->personaje.ultimo_movimiento, juego->personaje.posicion);
}

/*
 * Pre: -
 * Post: Va a volver visible igual a TRUE si se topa con algun obstaculo, alguna herramienta o CHLOE de mientras el personaje se mueva con la vela prendida
 */
void movimiento_vela(juego_t *juego){
	ocultar_elementos_del_campo(juego);		
	iluminar_elementos_vela(juego, juego->personaje.posicion.fil, juego->personaje.posicion.col);
}

void movimiento_herramientas(juego_t* juego, char jugada){
	if(se_movio_personaje(jugada)){
		if(juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == LINTERNA)
			movimiento_linterna(juego);
		if(juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == VELA)
			movimiento_vela(juego);
	}
}

/*
 * Pre: Necesito que indice sea menor a la juego.personaje.cantidad_elementos
 * Post: Va a intercambiar la herrramienta deseada con la ultima.
 */	
void eliminar_elemento(juego_t* juego, int indice){
	for (int i = indice; i < juego->personaje.cantidad_elementos; i++){
		juego->personaje.mochila[i] = juego->personaje.mochila[i+1];
	}
	juego->personaje.cantidad_elementos--;
}

void eliminar_elementos_sin_mov(juego_t* juego){
	for (int i = 0; i < juego->personaje.cantidad_elementos; i++){
		if((juego->personaje.mochila[i].movimientos_restantes <= 0) && (juego->personaje.mochila[i].tipo != LINTERNA)){
			eliminar_elemento(juego, i);
		}
	}
}

/*
 * Pre: -
 * Post: Me va a cambiar CHLOE visible a TRUE si el personaje es PANDA_OSOS y perdio TIEMPO_PERDIDO_PANDA segundos
 */
void visibilizar_chloe(personaje_t personaje, bool* chloe_visible){
	if((personaje.tipo == PANDA_OSO) && (personaje.tiempo_perdido >= TIEMPO_PERDIDO_PANDA)){
		*chloe_visible = true;
	}
}

void realizar_jugada(juego_t* juego, char jugada){
	int cantidad_velas, cantidad_bengalas;

	if(se_movio_personaje(jugada))
		juego->personaje.ultimo_movimiento = jugada;

	switch (jugada){
	case ARRIBA:
		if(juego->personaje.posicion.fil > 0){
			juego->personaje.posicion.fil--;
			if(juego->personaje.elemento_en_uso != ELEMENTO_NO_USO)
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
			encontrar_obstaculo(juego);
			encontrar_herramienta(juego);
		}
		system("clear");
		break;
	case ABAJO:
		if(juego->personaje.posicion.fil < MAX_FILAS-1){
			juego->personaje.posicion.fil++;
			if(juego->personaje.elemento_en_uso != ELEMENTO_NO_USO)
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
			encontrar_obstaculo(juego);
			encontrar_herramienta(juego);
		}
		system("clear");
		break;
	case DERECHA:
		if(juego->personaje.posicion.col < MAX_COLUMNAS-1){
			juego->personaje.posicion.col++;
			if(juego->personaje.elemento_en_uso != ELEMENTO_NO_USO)
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
			encontrar_obstaculo(juego);
			encontrar_herramienta(juego);
		}
		system("clear");
		break;
	case IZQUIERDA:
		if(juego->personaje.posicion.col > 0){
			juego->personaje.posicion.col--;
			if(juego->personaje.elemento_en_uso != ELEMENTO_NO_USO)
				juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
			encontrar_obstaculo(juego);
			encontrar_herramienta(juego);
		}
		system("clear");
		break;
	case ENCENDER_APAGAR_LINTERNA:
		system("clear");
		int j = 0;
		
		while(juego->personaje.mochila[j].tipo != LINTERNA)
			j++;
		
		if(juego->personaje.mochila[j].movimientos_restantes <= 0){

			ocultar_elementos_del_campo(juego);
			mensaje_sin_herramienta_mochila(STR_PILAS);

		} else if(juego->personaje.mochila [juego->personaje.elemento_en_uso].tipo == BENGALA && juego->personaje.mochila [juego->personaje.elemento_en_uso].movimientos_restantes != SIN_MOVIMIENTO){
			
			printf("La bengala esta en uso esta en uso maestro!!!\n");
			juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;

		} else if(juego->personaje.mochila [juego->personaje.elemento_en_uso].tipo == LINTERNA){
			
			ocultar_elementos_del_campo(juego);
			juego->personaje.elemento_en_uso = ELEMENTO_NO_USO;
			
			printf("Linterna desactivada\n");
		} else {
			
			ocultar_elementos_del_campo(juego);
			cargar_obstaculos(juego, KOALAS, MAX_KOALAS + juego->cantidad_obstaculos);
			activar_linterna(juego);

		} 
		break;
	case ENCENDER_APAGAR_VELA:
		system("clear");
		cantidad_velas = calcular_cantidad_herramienta(juego->personaje, VELA);
		
		if(cantidad_velas <= SIN_HERRAMIENTA){
			
			mensaje_sin_herramienta_mochila(STR_VELAS);

		} else if(juego->personaje.mochila [juego->personaje.elemento_en_uso].tipo == BENGALA && juego->personaje.mochila [juego->personaje.elemento_en_uso].movimientos_restantes != SIN_MOVIMIENTO){
			
			printf("La bengala esta en uso esta en uso!!!\n");
			juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
		
		} else if(juego->personaje.mochila [juego->personaje.elemento_en_uso].tipo == VELA){
			
			ocultar_elementos_del_campo(juego);
			juego->personaje.elemento_en_uso = ELEMENTO_NO_USO;
			printf("Vela desactivada\n");
		
		} else {
			
			ocultar_elementos_del_campo(juego);
			activar_vela(juego);
		
		} 
		break;
	case ENCENDER_APAGAR_BENGALA:
		system("clear");
		cantidad_bengalas = calcular_cantidad_herramienta(juego->personaje, BENGALA);
		
		if(cantidad_bengalas <= SIN_HERRAMIENTA){
			
			mensaje_sin_herramienta_mochila(STR_BENGALAS);
		
		} else if(juego->personaje.mochila [juego->personaje.elemento_en_uso].tipo != BENGALA){
			
			ocultar_elementos_del_campo(juego);
			activar_bengala(juego);
		
		} else if(juego->personaje.mochila [juego->personaje.elemento_en_uso].tipo == BENGALA){
			
			printf("La bengala esta en uso esta en uso!!!\n");
			juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
		
		}
		break;
	case TIEMPO_RESTANTE:
		system("clear");
		printf("»Te quedan: %.1f segundos para encontrar a %s.«\n", MAX_TIEMPO - tiempo_actual(), EMOJI_CHLOE);
		break;
	}
	
	movimiento_herramientas(juego, jugada);

	if(juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes <= 0){
		ocultar_elementos_del_campo(juego);
		juego->personaje.elemento_en_uso = ELEMENTO_NO_USO;
	}

	eliminar_elementos_sin_mov(juego);
	visibilizar_chloe(juego->personaje, &juego->chloe_visible);
	mostrar_juego(*juego);
}

/*
 * Pre: -
 * Post: Devolvera TRUE si el personaje esta en la misma posicion de chloe
 */
bool encontraste_chloe(juego_t juego){
	return ((juego.amiga_chloe.fil == juego.personaje.posicion.fil) && (juego.amiga_chloe.col == juego.personaje.posicion.col));
}

int estado_juego(juego_t juego){
	int estado_juego = EN_JUEGO;
	if (encontraste_chloe(juego)){
		estado_juego = JUEGO_FINALIZADO;
	}
	return estado_juego;
}

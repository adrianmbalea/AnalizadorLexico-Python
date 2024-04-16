#ifndef ABB_H
#define ABB_H

#include "../includes/definiciones.h"

/**
 * Tipo de dato abstracto para arbol binario con clave de
 * ordenacion y elemento de celda.
 */

typedef struct celda * ABB;
		

//FUNCIONES DE CREACIÓN Y DESTRUCCIÓN DEL ARBOL
/**
 * Crea el arbol vacio.
 * @param A Puntero al arbol. Debe estar inicializado.
 */
void crearAbb(ABB *A);

/**
 * Destruye el arbol recursivamente
 * @param A El arbol que queremos destruir
 */
void destruirAbb(ABB *A);


//FUNCIONES DE INFORMACIÓN
/**
 * Comprueba si el arbol esta vacio
 * @param A El arbol binario
 */
unsigned esAbbVacio(ABB A);

void buscarNodoAbb(ABB A, char *lexema, componente* elemento);

void imprimirAbb(ABB A);





int insertarElementoAbb(ABB *A, char* lexema, int compLex);

void suprimirElementoAbb(ABB *A, char *lexema);



#endif	// ABB_H


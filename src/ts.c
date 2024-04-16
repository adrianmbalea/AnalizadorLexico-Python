#include <stdlib.h>
#include <stdio.h>
#include "../includes/ts.h"
#include "../includes/abb.h"
#include "definiciones.h"

/** Estructura para la tabla de simbolos => ABB **/
ABB ts;

/**
 * @brief Inicializa la TS con todas las palabras reservadas
 * 
 */
void inicializarTS(){
    int nPalabrasReservadas = sizeof(palabrasReservadas)/sizeof(palabrasReservadas[0]);
    for(int i=0; i<nPalabrasReservadas; i++){
        insertarLex(palabrasReservadas[i].lexema, palabrasReservadas[i].compLex);
    }
}

/**
 * @brief Libera la memoria de la TS
 * 
 */
void liberarTS(){
    destruirAbb(&ts);
}

/**
 * @brief Inserta un elemento en la TS, si no existe ya
 * 
 * @param lexema Lexema del elemento
 * @param compLex Componente lexico del elemento
 * @return int Componente lexico del elemento
 */
int insertarLex(char* lexema, int compLex){
    if(compLex){
        return insertarElementoAbb(&ts, lexema, compLex);
    } else{
        return -1;
    }
    
}

/**
 * @brief Elimina un elemento de la TS en caso de que exista
 * 
 * @param lexema Lexema del elemento a eliminar
 */
void eliminarLex(char* lexema){
    suprimirElementoAbb(&ts, lexema);
}

/**
 * @brief Imprime la TS siguiendo un recorrido inorden, lo cual implica que se impriman en orden alfabetico
 * 
 */
void imprimirTS(){
    printf("-------------------- TABLA DE SIMBOLOS --------------------\n");
    imprimirAbb(ts);
    printf("-----------------------------------------------------------\n");
}
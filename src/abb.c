#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/abb.h"

//ESTRUCTURAS DE DATOS

struct celda {
	componente info;
	struct celda *izq, *der;
};



void crearAbb(ABB *A) {
    *A = NULL;
}

void destruirAbb(ABB *A) {
    if (*A != NULL) {
        destruirAbb(&(*A)->izq);
        destruirAbb(&(*A)->der);
        if((*A)->info.lexema != NULL){
            free((*A)->info.lexema);
            (*A)->info.lexema=NULL;
        }
        free(*A);
        *A = NULL;
    }
}

int insertarElementoAbb(ABB *A, char* lexema, int compLex) {
    if (esAbbVacio(*A)) {
        *A = (ABB) malloc(sizeof (struct celda));
        (*A)->info.lexema = (char*)malloc(strlen(lexema)+1);
		strcpy((*A)->info.lexema, lexema);
		(*A)->info.lexema[strlen(lexema)] = '\0';
        (*A)->info.compLex=compLex;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return compLex;
    }
    int comp = strcmp(lexema, (*A)->info.lexema);

    if (comp == 0) { //lexema == (*A)->info.lexema
        return (*A)->info.compLex;
    }
    if (comp > 0) { //lexema > (*A)->info.lexema
        return insertarElementoAbb(&((*A)->der), lexema, compLex);
    }

    //lexema < ( *A)->info.lexema 
    return insertarElementoAbb(&((*A)->izq), lexema, compLex);
}

/* Funcion privada que devuelve mínimo de subárbol dcho */
componente _suprimir_min(ABB * A) {//Se devuelve el elemento más a la izquierda
    ABB aux;
    componente ele;
    if (esAbbVacio((*A)->izq)) {//Si izquierda vacía, se devuelve valor nodo actual A
        ele = (*A)->info;
        aux = *A;
        *A = (*A)->der;
        free(aux);
        return ele;
    } else {
        return _suprimir_min(&(*A)->izq); //se vuelve a buscar mínimo rama izquierda
    }
}

void suprimirElementoAbb(ABB *A, char *lexema) {
    ABB aux;
    if (esAbbVacio(*A)) {
        return;
    }

    int comp = strcmp(lexema, (*A)->info.lexema);
    if (comp < 0) { // lexema < (*A)->info.lexema
        suprimirElementoAbb(&(*A)->izq, lexema);
    } else if (comp > 0) { // lexema > (*A)->info.lexema
        suprimirElementoAbb(&(*A)->der, lexema);
    } else if (esAbbVacio((*A)->izq) && esAbbVacio((*A)->der)) {
        free((*A)->info.lexema);
        free(*A);
        *A = NULL;
    } else if (esAbbVacio((*A)->izq)) { // pero no es vacio derecha
        aux = *A;
        *A = (*A)->der;
        free(aux->info.lexema);
        free(aux);
    } else if (esAbbVacio((*A)->der)) { //pero no es vacio izquierda
        aux = *A;
        *A = (*A)->izq;
        free(aux->info.lexema);
        free(aux);
    } else { //ni derecha ni izquierda esta vacio, busco mínimo subárbol derecho
        free((*A)->info.lexema); //elimino la info pero no libero el nodo,
        //pues en su sitio voy a poner el mínimo del subárbol derecho
        (*A)->info = _suprimir_min(&(*A)->der);
    }
}


//OPERACIONES DE INFORMACIÓN

unsigned esAbbVacio(ABB A) {
    return A == NULL;
}

void buscarNodoAbb(ABB A, char *lexema, componente* componente) {
    if (esAbbVacio(A)) {
        return;
    }

    int comp = strcmp(lexema, A->info.lexema);
    if (comp == 0) { // lexema == A->info.lexema
        *componente = A->info;
        return;
    } else if (comp < 0) { // lexema < A->info.lexema
        buscarNodoAbb(A->izq, lexema, componente);
    } else { // lexema > A->info.lexema
        buscarNodoAbb(A->der, lexema, componente);
    }
}

void imprimirAbb(ABB A){
    if(!esAbbVacio(A)){
		imprimirAbb(A->izq);
        printf("<%d, \"%s\">\n", A->info.compLex, A->info.lexema);
		imprimirAbb(A->der);
    }
}
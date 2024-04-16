#ifndef ERRORES_H
#define ERRORES_H

#define ERROR_ARCHIVO 1
#define TAM_LEXEMA_EXCEDIDO 2
#define LEXEMA_MAL_FORMADO 3

/**
 * @brief Permite imprimir el error indicando la linea
 * 
 * @param codigo Codigo del error
 * @param linea Linea en la que se produjo el error
 */
void imprimirError(int codigo, int linea);

#endif	/* ERRORES_H */
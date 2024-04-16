#ifndef LEXICO_H
#define LEXICO_H

#include "../includes/definiciones.h"

/**
 * @brief Funcion llamada por el analizador sintactico para obtener el siguiente componente lexico
 * 
 * @return componente Componente lexico
 */
componente sigCompLexico();

/**
 * @brief Imprime el componente lexico con el formato <compLex, lexema> y despues libera su memoria
 * 
 * @param c Componente a imprimir
 */
void imprimirCompLexico(componente c);

#endif	/* LEXICO_H */
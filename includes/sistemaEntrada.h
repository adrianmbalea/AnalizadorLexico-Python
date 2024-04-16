#ifndef SISTEMAENTRADA_H
#define SISTEMAENTRADA_H

/**
 * @brief Inicializa el buffer de entrada, cargando el primer bloque
 * 
 * @param nombreArchivo Nombre del archivo a analizar
 */
void inicializarSistemaEntrada(const char *nombreArchivo);

/**
 * @brief Devuelve el siguiente caracter a procesar por el analizador lexico
 * 
 * @return char Caracter correspondiente al que apunta delantero
 */
char sigCaracter();

/**
 * @brief Retrocede nCaracteres el puntero delantero
 * 
 * @param nCaracteres numero de posiciones en las que se va a retroceder el puntero delantero
 */
void devolverCaracteres(int nCaracteres);

/**
 * @brief Llamada que hace el analizador lexico al sistema de entrada para reconocer el lexema que se encuentra entre inicio y delantero
 * 
 * @param errorTamLexemaExcedido Flag para indicar al analizador lexico que se ha excedido el tamano del lexema
 * @return char* Cadena correspondiente al lexema
 */
char* reconocerLexema(int* errorTamLexemaExcedido);

/**
 * @brief Libera la memoria del sistema de entrada
 * 
 */
void liberarSistemaEntrada();

#endif /* SISTEMAENTRADA_H */
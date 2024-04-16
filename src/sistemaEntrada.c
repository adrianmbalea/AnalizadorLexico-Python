#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/sistemaEntrada.h"
#include "../includes/errores.h"

// Tamano de cada bloque
#define N 31 // Asi conseguimos un centinela total de tamano 64

#define BLOQUE_A 0
#define BLOQUE_B 1

#define EOF_A N
#define EOF_B N*2+1
#define INICIO_A 0
#define INICIO_B N+1

FILE *archivo;

// Estructura para el sistema de entrada
typedef struct {
    char bloques[N * 2 + 2];    // Los dos bloques logicos estan en un unico array fisico
    char* delantero;            // Puntero delantero
    char* inicio;               // Puntero inicio
} Buffer;

Buffer buf; // Variable global para el buf de entrada
int yaCargado = 0; // Variable para evitar cargar un bloque dos veces al devolver caracteres
int tamLexemaReal = 0; // Variable que almacena el tamano real del lexema, aunque se sobreescriban bloques

// Funciones privadas

/**
 * @brief Busca el bloque en el que se encuentra un puntero, ya sea delantero o inicio
 * 
 * @param puntero Puntero del que se quiere buscar su posicion, puede ser buf.inicio o buf.delantero
 * @return int BLOQUE_A(0) si esta en el bloque A, BLOQUE_B(1) si esta en el bloque B
 */
int _buscarPuntero(char* puntero){
    if(puntero <= buf.bloques+EOF_A){
        return BLOQUE_A;
    } return BLOQUE_B;
}

/**
 * @brief Permite cargar un bloque del centinela leyendo del archivo
 * 
 * @param bloque Bloque que se quiere cargar. Puede ser BLOQUE_A(0) o BLOQUE_B(1)
 */
void _cargarBloque(int bloque){
    if(bloque==BLOQUE_A){
        // Si se sobreescribe el bloque donde esta el puntero inicio,
        // lo movemos y nos quedamos con la parte final del lexema
        if(_buscarPuntero(buf.inicio)==BLOQUE_A){
            buf.inicio=buf.bloques+INICIO_B;
        }

        // fread no lee el EOF, por lo que se coloca manualmente
        int bytesLeidos = fread(buf.bloques, sizeof(char), N, archivo);
        if(bytesLeidos<N){
            buf.bloques[INICIO_A+bytesLeidos] = EOF;
        }
    } else if(bloque==BLOQUE_B){
        // Si se sobreescribe el bloque donde esta el puntero inicio,
        // lo movemos y nos quedamos con la parte final del lexema
        if(_buscarPuntero(buf.inicio)==BLOQUE_B){
            buf.inicio=buf.bloques+INICIO_A;
        }

        // fread no lee el EOF, por lo que se coloca manualmente
        int bytesLeidos = fread(buf.bloques+INICIO_B, sizeof(char), N, archivo);
        if(bytesLeidos<N){
            buf.bloques[INICIO_B+bytesLeidos] = EOF;
        }
    }
}

/**
 * @brief Avanza la posicion del puntero delantero en uno
 * 
 */
void _avanzarDelantero(){
    buf.delantero++;
    if (buf.delantero == buf.bloques+EOF_A) { // Se ha llegado al EOF del bloque A
        if(!yaCargado){
            // Cargar bloque B
            _cargarBloque(BLOQUE_B);
        } else{
            yaCargado=0;
        }
        buf.delantero = buf.bloques+INICIO_B;
    } else if (buf.delantero == buf.bloques+EOF_B){ // Se ha llegado al EOF del bloque B
        if(!yaCargado){
            // Cargar bloque A
            _cargarBloque(BLOQUE_A);
        } else{
            yaCargado=0;
        }
        buf.delantero = buf.bloques+INICIO_A;
    }
}

/**
 * @brief Avanza la posicion del puntero inicio hasta la posicion de delantero
 * 
 */
void _avanzarInicio(){
    buf.inicio = buf.delantero;
}

/**
 * @brief Retrocede en una posicion el puntero delantero
 * 
 */
void _devolverCaracter(){
    if(buf.delantero==buf.bloques+INICIO_A){
        yaCargado=1;
        buf.delantero=buf.bloques+EOF_B-1;
    } else if(buf.delantero==buf.bloques+INICIO_B){
        yaCargado=1;
        buf.delantero=buf.bloques+EOF_A-1;
    } else{
        buf.delantero--;
    }
}


// Funciones publicas

/**
 * @brief Inicializa el buffer de entrada, cargando el primer bloque
 * 
 * @param nombreArchivo Nombre del archivo a analizar
 */
void inicializarSistemaEntrada(const char *nombreArchivo) {
    archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        imprimirError(ERROR_ARCHIVO, 0);
        exit(EXIT_FAILURE);
    }

    buf.bloques[EOF_A] = EOF; //EOF del bloque A
    buf.bloques[EOF_B] = EOF; // EOF del bloque B

    fread(buf.bloques, sizeof(char), N, archivo);
    buf.delantero = buf.bloques;
    buf.inicio = buf.bloques;
}

/**
 * @brief Devuelve el siguiente caracter a procesar por el analizador lexico
 * 
 * @return char Caracter correspondiente al que apunta delantero
 */
char sigCaracter() {
    char caracter = *(buf.delantero);

    tamLexemaReal++;

    _avanzarDelantero();

    return caracter;
}

/**
 * @brief Retrocede nCaracteres el puntero delantero
 * 
 * @param nCaracteres numero de posiciones en las que se va a retroceder el puntero delantero
 */
void devolverCaracteres(int nCaracteres){
    for(int i=0; i<nCaracteres; i++){
        _devolverCaracter();
    }
}

/**
 * @brief Llamada que hace el analizador lexico al sistema de entrada para reconocer el lexema que se encuentra entre inicio y delantero
 * 
 * @param errorTamLexemaExcedido Flag para indicar al analizador lexico que se ha excedido el tamano del lexema
 * @return char* Cadena correspondiente al lexema
 */
char* reconocerLexema(int* errorTamLexemaExcedido){
    int tamanoLexema;
    char* lexema;

    int bloqueInicio = _buscarPuntero(buf.inicio);
    int bloqueDelantero = _buscarPuntero(buf.delantero);

    if(bloqueInicio==bloqueDelantero){
        tamanoLexema = buf.delantero - buf.inicio;
        lexema = (char*)malloc((tamanoLexema+1)*sizeof(char));
        strncpy(lexema, buf.inicio, tamanoLexema*sizeof(char));
    } else if(bloqueInicio==BLOQUE_A){ // Si inicio esta en A y delantero en B
        tamanoLexema = buf.delantero - buf.inicio;
        lexema = (char*)malloc((tamanoLexema+1)*sizeof(char));
        // obtenemos primero la cadena desde inicio al fin del bloque A
        strncpy(lexema, buf.inicio, (buf.bloques+EOF_A - buf.inicio)*sizeof(char));
        // le anhadimos el '\0' para que la concatenacion funcione correctamente
        lexema[buf.bloques+EOF_A - buf.inicio] = '\0';
        strncat(lexema, buf.bloques+INICIO_B, (buf.delantero-(buf.bloques+INICIO_B))*sizeof(char));
    } else{ // Si inicio esta en B y delantero en A
        tamanoLexema = (buf.bloques + EOF_B - buf.inicio) + (buf.delantero-buf.bloques);
        lexema = (char*)malloc((tamanoLexema+1)*sizeof(char));
        strncpy(lexema, buf.inicio, (buf.bloques+EOF_B - buf.inicio)*sizeof(char));
        lexema[buf.bloques+EOF_B-buf.inicio] = '\0';
        strncat(lexema, buf.bloques, (buf.delantero-buf.bloques)*sizeof(char));
    }

    lexema[tamanoLexema] = '\0';

    _avanzarInicio();

    if(tamLexemaReal>N){
        *errorTamLexemaExcedido=1;
    } else{
        *errorTamLexemaExcedido=0;
    }

    tamLexemaReal=0;

    return lexema;
}

/**
 * @brief Libera la memoria del sistema de entrada
 * 
 */
void liberarSistemaEntrada() {
    fclose(archivo);
}



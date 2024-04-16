#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../includes/lexico.h"
#include "../includes/sistemaEntrada.h"
#include "../includes/ts.h"
#include "../includes/errores.h"


#define ESTADO_RECHAZO 0

int nLinea = 1;
int tamLexema = 0;

// Funciones privadas correspondientes a los automatas

/**
 * @brief Automata que reconoce cadenas alfanumericas
 * 
 * @param caracter Primer caracter de la cadena procesado por el analizador lexico
 * @return int Componente lexico
 */
int _alphaNumerico(char caracter){
    int estado=0;
    
    while(1){
        tamLexema++;
        switch (estado) {
            case 0:
                if(isalpha(caracter)){
                    estado=2;
                    break;
                } else if(caracter=='_'){
                    estado=1;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            case 1:
                if(isalnum(caracter)){
                    estado=2;
                    break;
                } else if(caracter=='_'){
                    estado=1;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // inicialmente el automata lo detecta como ID, pero si es palabra reservada se comprueba el analizador lexico
            case 2:
                if(!isalnum(caracter) && caracter != '_'){
                    devolverCaracteres(1);
                    return ID;
                } else{
                    break;
                }

            default:
                return ESTADO_RECHAZO;
        }

        caracter = sigCaracter();
    }
}

/**
 * @brief Automata que reconoce operadores y delimitadores
 * 
 * @param caracter Primer caracter de la cadena procesado por el analizador lexico
 * @return int Componente lexico
 */
int _operadoresDelimitadores(char caracter){
    int estado=0;
    char op=-1; // se emplea una variable aux con el fin de simplificar el numero de estados
    while(1){
        tamLexema++;
        switch(estado){
            case 0:
                if(caracter=='~' || caracter=='(' || caracter==')' || caracter=='[' || caracter==']' ||
                    caracter=='{' || caracter=='}' || caracter==',' || caracter=='.' || caracter==';'){
                        return caracter;
                    }

                else if(caracter=='+' || caracter=='%' || caracter=='@' || caracter=='&' ||
                    caracter=='|' || caracter=='^' || caracter==':' || caracter=='='){
                        op=caracter;
                        estado=1;
                        break;
                }

                else if(caracter=='-'){
                    estado=2;
                    break;
                }

                else if(caracter=='*' || caracter=='/' || caracter=='<' || caracter=='>'){
                    op=caracter;
                    estado=3;
                    break;
                }

                else if(caracter=='!'){
                    estado=4;
                    break;
                } 
                
                else{
                    return ESTADO_RECHAZO;
                }
            
            // +,%, @, &, |, ^, :, =, **, //, <<, >>
            case 1:
                if(caracter=='='){
                    switch(op){
                        case '+':
                            return MAS_IGUAL;
                        case '%':
                            return PORCENTAJE_IGUAL;
                        case '@':
                            return ARROBA_IGUAL;
                        case '&':
                            return AMPERSAND_IGUAL;
                        case '|':
                            return BARRA_VERTICAL_IGUAL;
                        case '^':
                            return CIRCUNFLEJO_IGUAL;
                        case ':':
                            return DOS_PUNTOS_IGUAL;
                        case '=':
                            return IGUAL_IGUAL;
                        case '*':
                            return ASTERISCO_ASTERISCO_IGUAL;
                        case '/':
                            return SLASH_SLASH_IGUAL;
                        case '<':
                            return MENOR_MENOR_IGUAL;
                        case '>':
                            return MAYOR_MAYOR_IGUAL;
                        default:
                            return ESTADO_RECHAZO;
                    }
                } else{
                    devolverCaracteres(1);
                    return op;
                }

            // -
            case 2:
                if(caracter=='='){
                    return MENOS_IGUAL;
                } else if(caracter=='>'){
                    return FLECHA;
                } else{
                    devolverCaracteres(1);
                    return '-';
                }

            // *, /, <, >
            case 3:
                // si se repite al caracter anterior
                // es decir, **, //, <<, >>
                if(caracter==op){
                    op=caracter;
                    estado=1;
                    break;
                } else if(caracter=='='){
                    switch (op){
                        case '*':
                            return POR_IGUAL;
                        case '/':
                            return SLASH_IGUAL;
                        case '<':
                            return MENOR_IGUAL;
                        case '>':
                            return MAYOR_IGUAL;
                        default:
                            return ESTADO_RECHAZO;
                    }
                } else{
                    devolverCaracteres(1);
                    return op;
                }

            // !
            case 4:
                if(caracter=='='){
                    return DISTINTO;
                } else{
                    return ESTADO_RECHAZO;
                }

            default:
                return ESTADO_RECHAZO;

        }
        caracter = sigCaracter();
    }
}

/**
 * @brief Automata que reconoce strings y bytes literales
 * 
 * @param caracter Primer caracter de la cadena procesado por el analizador lexico
 * @return int Componente lexico
 */
int _stringsBytesLiterales(char caracter){
    int estado=0;
    while(1){
        tamLexema++;
        switch(estado){
            case 0:
                if(caracter=='r' || caracter=='R'){
                    estado=1;
                    break;
                } else if(caracter=='f' || caracter=='F'){
                    estado=2;
                    break;
                } else if(caracter=='b' || caracter=='B'){
                    estado=3;
                    break;
                } else if(caracter=='u' || caracter=='U'){
                    estado=4;
                    break;
                } else if(caracter=='"'){
                    estado=5;
                    break;
                } else if(caracter=='\''){
                    estado=8;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // r, R
            case 1:
                if(caracter=='f' || caracter=='F'){
                    estado=4;
                    break;
                } else if(caracter=='b' || caracter=='B'){
                    estado=11;
                    break;
                } else if(caracter=='"'){
                    estado=5;
                    break;
                } else if(caracter=='\''){
                    estado=8;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }
            
            // f, F
            case 2:
                if(caracter=='r' || caracter=='R'){
                    estado=4;
                    break;
                } else if(caracter=='"'){
                    estado=5;
                    break;
                } else if(caracter=='\''){
                    estado=8;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // b, B
            case 3:
                if(caracter=='r' || caracter=='R'){
                    estado=11;
                    break;
                } else if(caracter=='"'){
                    estado=12;
                    break;
                } else if(caracter=='\''){
                    estado=15;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // [prefijos]
            case 4:
                if(caracter=='"'){
                    estado=5;
                    break;
                } else if(caracter=='\''){
                    estado=8;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // [prefijos] "
            case 5:
                if(caracter=='"'){
                    estado=6;
                    break;
                } else if(caracter!='\n' && caracter!=EOF){
                    estado=18;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // [prefijos] ""
            case 6:
                if(caracter=='"'){
                    estado=7;
                    break;
                } else{
                    devolverCaracteres(1);
                    return STRING_LITERAL;
                }

            // [prefijos] """caracter*
            case 7:
                if(caracter!='"'){
                    break;
                } else {
                    estado=22;
                    break;
                }

            // [prefijos] '
            case 8:
                if(caracter=='\''){
                    estado=9;
                    break;
                } else if(caracter!='\n' && caracter!=EOF){
                    estado=20;
                    break;  
                } else{
                    return ESTADO_RECHAZO;
                }

            // [prefijos] ''
            case 9:
                if(caracter=='\''){
                    estado=10;
                    break;
                } else{
                    devolverCaracteres(1);
                    return STRING_LITERAL;
                }

            // [prefijos] '''caracter*
            case 10:
                if(caracter!='\''){
                    break;
                } else{
                    estado=26;
                    break;
                }

            // prefijobinario [prefjioR]
            case 11:
                if(caracter=='"'){
                    estado=12;
                    break;
                } else if(caracter=='\''){
                    estado=15;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // prefijobinario [prefjioR] "
            case 12:
                if(caracter=='"'){
                    estado=13;
                    break;
                } else if(caracter!='\n' && caracter!=EOF){
                    estado=19;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // prefijobinario [prefijoR] ""
            case 13:
                if(caracter=='"'){
                    estado=14;
                    break;
                } else{
                    devolverCaracteres(1);
                    return BYTE_LITERAL;
                }

            // prefijobinario [prefijoR] """caracter*
            case 14:
                if(caracter!='"'){
                    break;
                } else{
                    estado=24;
                    break;
                }

            // prefijobinario [prefijoR] '
            case 15:
                if(caracter=='\''){
                    estado=16;
                    break;
                } else if(caracter!='\n' && caracter!=EOF){
                    estado=21;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // prefijobinario [prefijoR] ''
            case 16:
                if(caracter=='\''){
                    estado=17;
                    break;
                } else{
                    devolverCaracteres(1);
                    return BYTE_LITERAL;
                }

            // prefijobinario [prefijoR] '''caracter*
            case 17:
                if(caracter!='\''){
                    break;
                } else{
                    estado=28;
                    break;
                }

            // [prefijos] "[caracter]*
            case 18:
                if(caracter=='"'){
                    return STRING_LITERAL;
                } else if(caracter!='\n' && caracter!=EOF){
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // prefijobinario [prefijos] "[caracter]*
            case 19:
                if(caracter=='"'){
                    return BYTE_LITERAL;
                } else if(caracter!='\n' && caracter!=EOF){
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // [prefijos] '[caracter]*
            case 20:
                if(caracter=='\''){
                    return STRING_LITERAL;
                } else if(caracter!='\n' && caracter!=EOF){
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // prefijobinario [prefijos] '[caracter]*
            case 21:
                if(caracter=='\''){
                    return BYTE_LITERAL;
                } else if(caracter!='\n' && caracter!=EOF){
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // [prefijos] """[caracter]*"
            case 22:
                if(caracter=='"'){
                    estado=23;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // [prefijos] """[caracter]*""
            case 23:
                if(caracter=='"'){
                    return STRING_LITERAL;
                } else{
                    return ESTADO_RECHAZO;
                }

            // prefijobinario [prefijos] """[caracter]*"
            case 24:
                if(caracter=='"'){
                    estado=25;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // prefijobinario [prefijos] """[caracter]*""
            case 25:
                if(caracter=='"'){
                    return BYTE_LITERAL;
                } else{
                    return ESTADO_RECHAZO;
                }

            // [prefijos] '''[caracter]*'
            case 26:
                if(caracter=='\''){
                    estado=27;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // [prefijos] '''[caracter]*''
            case 27:
                if(caracter=='\''){
                    return STRING_LITERAL;
                } else{
                    return ESTADO_RECHAZO;
                }

            // prefijobinario [prefijos] '''[caracter]*'
            case 28:
                if(caracter=='\''){
                    estado=29;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // prefijobinario [prefijos] '''[caracter]*''
            case 29:
                if(caracter=='\''){
                    return BYTE_LITERAL;
                } else{
                    return ESTADO_RECHAZO;
                }
            default:
                return ESTADO_RECHAZO;
        }
        caracter=sigCaracter();
    }
}

/**
 * @brief Parte de automata que reconoce la parte exponencial de un numero despues de haber procesado ya la e
 * 
 * @return int Componente lexico
 */
int _exponencial(){
    int estado=0;
    while(1){
        char caracter = sigCaracter();
        tamLexema++;
        switch (estado){
            case 0:
                if(caracter=='+' || caracter=='-'){
                    estado=1;
                    break;
                } else if(isdigit(caracter)){
                    estado=2;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            case 1:
                if(isdigit(caracter)){
                    estado=2;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            case 2:
                if(isdigit(caracter)){
                    break;
                } else{
                    devolverCaracteres(1);
                    return FLOATING_POINT_LITERAL;
                }
            
            default:
                return ESTADO_RECHAZO;
        }
    }
}

/**
 * @brief Parte de automata que reconoce la parte flotante de un numero despues de haber procesado ya el '.'
 * 
 * @return int Componente lexico
 */
int _flotante(){
    int estado=0;
    while(1){
        char caracter=sigCaracter();
        tamLexema++;
        switch(estado){
            case 0:
                if(isdigit(caracter)){
                    estado=1;
                    break;
                } else if(caracter=='e' || caracter=='E'){
                    return _exponencial();
                } else{
                    devolverCaracteres(1);
                    return FLOATING_POINT_LITERAL;
                }

            case 1:
                if(isdigit(caracter)){
                    break;
                } else if(caracter=='_'){
                    estado=2;
                    break;
                } else if(caracter=='e' || caracter=='E'){
                    return _exponencial();
                } else{
                    devolverCaracteres(1);
                    return FLOATING_POINT_LITERAL;
                }

            case 2:
                if(isdigit(caracter)){
                    estado=1;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            default:
                return ESTADO_RECHAZO;
        }
    }
}

/**
 * @brief Automata que reconoce numeros
 * 
 * @param caracter Primer caracter de la cadena procesado por el analizador lexico
 * @return int Componente lexico
 */
int _numerico(char caracter){
    int estado=0;
    while(1){
        tamLexema++;
        switch (estado) {
            case 0:
                if(isdigit(caracter)){
                    if(caracter=='0'){
                        estado=3;
                        break;
                    } else{
                        estado=1;
                        break;
                    }
                } else if(caracter=='.'){
                    return _flotante();
                } else{
                    return ESTADO_RECHAZO;
                }

            case 1:
                if(isdigit(caracter)){
                    break;
                } else if(caracter=='_'){
                    estado=2;
                    break;
                } else if(caracter=='.'){
                    return _flotante();
                } else if(caracter=='e' || caracter=='E'){
                    return _exponencial();
                } else{
                    devolverCaracteres(1);
                    return INTEGER_LITERAL;
                }

            // nonzerodigit ([_] digit)*_
            case 2:
                if(isdigit(caracter)){
                    estado=1;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // 0
            case 3:
                if(caracter=='b' || caracter=='B'){
                    estado=4;
                    break;
                } else if(caracter=='o' || caracter=='O'){
                    estado=7;
                    break;
                } else if(caracter=='x' || caracter=='X'){
                    estado=10;
                    break;
                } else if(isdigit(caracter)){
                    estado=13;
                    break;
                } else if(caracter=='.'){
                    return _flotante();
                } else if(caracter=='e' || caracter=='E'){
                    return _exponencial();
                } else{
                    return ESTADO_RECHAZO;
                }

            // 0b, 0B
            case 4:
                if(caracter=='0' || caracter=='1'){
                    estado=6;
                    break;
                } else if(caracter=='_'){
                    estado=5;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // 0b([_] bindigit)*_
            case 5:
                if(caracter=='0' || caracter=='1'){
                    estado=6;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // Ob([_] bindigit)+
            case 6:
                if(caracter=='0' || caracter=='1'){
                    break;
                } else if(caracter=='_'){
                    estado=5;
                    break;
                } else{
                    devolverCaracteres(1);
                    return INTEGER_LITERAL;
                }

            // 0o, 0O
            case 7:
                if(caracter=='0' || caracter=='1' || caracter=='2' || caracter=='3' || 
                    caracter=='4' || caracter=='5' || caracter=='6' || caracter=='7'){
                        estado=9;
                        break;
                } else if(caracter=='_'){
                    estado=8;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // 0o([_] octdigit)*_
            case 8:
                if(caracter=='0' || caracter=='1' || caracter=='2' || caracter=='3' || 
                    caracter=='4' || caracter=='5' || caracter=='6' || caracter=='7'){
                        estado=9;
                        break;
                    }
                else{
                    return ESTADO_RECHAZO;
                }

            // Oo([_] octdigit)+
            case 9:
                if(caracter=='0' || caracter=='1' || caracter=='2' || caracter=='3' || 
                    caracter=='4' || caracter=='5' || caracter=='6' || caracter=='7'){
                        break;
                } else if(caracter=='_'){
                    estado=8;
                    break;
                } else{
                    devolverCaracteres(1);
                    return INTEGER_LITERAL;
                }

            // ox, 0X
            case 10:
                if(isxdigit(caracter)){
                    estado=12;
                    break;
                } else if(caracter=='_'){
                    estado=11;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // 0x([_] hexdigit)*_
            case 11:
                if(isxdigit(caracter)){
                    estado=12;
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }

            // Ox([_] hexdigit)+
            case 12:
                if(isxdigit(caracter)){
                    break;
                } else if(caracter=='_'){
                    estado=11;
                    break;
                } else{
                    devolverCaracteres(1);
                    return INTEGER_LITERAL;
                } 

            case 13:
                if(isdigit(caracter)){
                    break;
                } else if(caracter=='.'){
                    return _flotante();
                } else if(caracter=='e' || caracter=='E'){
                    return _exponencial();
                } else{
                    return ESTADO_RECHAZO;
                }

            default:
                return ESTADO_RECHAZO;
        }
        caracter = sigCaracter();
    }
}

/**
 * @brief Automata que reconoce comentarios o encoding declarations
 * 
 * @param caracter Primer caracter de la cadena procesado por el analizador lexico
 * @param linea Linea del archivo (las encoding declarations solo pueden estar en las lineas 1 o 2)
 * @return int Componente lexico
 */
int _comentario(char caracter, int linea){
    int estado=0;
    while(1){
        tamLexema++;
        switch (estado){
            case 0:
                if(caracter=='#'){
                    if(linea==1 || linea=='2'){
                        estado=1;
                    } else{
                        estado=20;
                    }
                    break;
                } else{
                    return ESTADO_RECHAZO;
                }
            case 1:
                if(caracter==' '){
                    estado=2;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 2:
                if(caracter=='-'){
                    estado=3;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 3:
                if(caracter=='*'){
                    estado=4;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 4:
                if(caracter=='-'){
                    estado=5;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 5:
                if(caracter==' '){
                    estado=6;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 6:
                if(caracter=='c'){
                    estado=7;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 7:
                if(caracter=='o'){
                    estado=8;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 8:
                if(caracter=='d'){
                    estado=9;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 9:
                if(caracter=='i'){
                    estado=10;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 10:
                if(caracter=='n'){
                    estado=11;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 11:
                if(caracter=='g'){
                    estado=12;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 12:
                if(caracter==':'){
                    estado=13;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 13:
                if(caracter==' '){
                    estado=14;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 14:
                if(isalpha(caracter)){
                    estado=15;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 15:
                if(caracter==' '){
                    estado=16;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    break;
                }
            case 16:
                if(caracter=='-'){
                    estado=17;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 17:
                if(caracter=='*'){
                    estado=18;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 18:
                if(caracter=='-'){
                    estado=19;
                    break;
                } else if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 19:
                if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return ENCODING_DECLARATION;
                } else if(caracter=='\n'){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    estado=20;
                    break;
                }
            case 20:
                if(caracter=='\n' || caracter==EOF){
                    devolverCaracteres(1);
                    return COMENTARIO;
                } else{
                    break;
                }
            
            default:
                return ESTADO_RECHAZO;
        }
        caracter=sigCaracter();
    }
}

/**
 * @brief Detecta si una cadena alfanumerica es una palabra reservada del lenguaje
 * 
 * @param lexema Lexema del elemento 
 * @return int Componente lexico de la palabra reservada, o -1 si no es una palabra reservada
 */
int _esPalabraReservada(char *lexema) {
    int nPalabrasReservadas = sizeof(palabrasReservadas)/sizeof(palabrasReservadas[0]);
    for (int i = 0; i < nPalabrasReservadas; i++) {
        if (strcmp(lexema, palabrasReservadas[i].lexema) == 0) {
            return palabrasReservadas[i].compLex;
        }
    }
    return -1; // No se encontró el lexema en las palabras reservadas
}

/**
 * @brief Imprime el componente lexico con el formato <compLex, lexema> y despues libera su memoria
 * 
 * @param c Componente a imprimir
 */
void imprimirCompLexico(componente c){
    if(c.compLex!=0){
        printf("< %d, %s >\n", c.compLex, c.lexema);
    }
    free(c.lexema);
}

/**
 * @brief Funcion llamada por el analizador sintactico para obtener el siguiente componente lexico
 * 
 * @return componente Componente lexico
 */
componente sigCompLexico(){
    tamLexema=0;
    int tamLexemaExcedido=0;
    componente c;
    char caracter = sigCaracter();
    if(caracter==EOF){
        c.compLex=EOF;
        c.lexema="EOF";
        return c;
    }

    while(caracter=='\n' || caracter=='\r' || caracter=='\t' || caracter==' '){
        if(caracter=='\n'){
            nLinea++;
        }
        free(reconocerLexema(&tamLexemaExcedido));
        caracter = sigCaracter();
    }

    if(isalpha(caracter) || caracter=='_'){
        if(caracter=='r' || caracter=='R' || caracter=='f' || caracter=='F' || caracter=='b' ||
            caracter=='B' || caracter=='u' || caracter=='U'){
                c.compLex=_stringsBytesLiterales(caracter);
                if(!c.compLex){ // si no es un string o un byte literal, es una cadena alfanumerica
                    devolverCaracteres(tamLexema);
                    caracter=sigCaracter();
                    c.compLex = _alphaNumerico(caracter);
                    c.lexema = reconocerLexema(&tamLexemaExcedido);
                    int palabraReservada = _esPalabraReservada(c.lexema);
                    if(palabraReservada!=-1){ // si es una palabra reservada
                        c.compLex=palabraReservada;
                    }
                    insertarLex(c.lexema, c.compLex);
                } else{ // si es un string o un byte literal
                    c.lexema = reconocerLexema(&tamLexemaExcedido);
                }
            } else{ // si no empieza por los prefijos de string/bytes es una cadena alfanumerica
                c.compLex = _alphaNumerico(caracter);
                c.lexema = reconocerLexema(&tamLexemaExcedido);
                int palabraReservada = _esPalabraReservada(c.lexema);
                if(palabraReservada!=-1){
                    c.compLex=palabraReservada;
                }
                insertarLex(c.lexema, c.compLex);
            }
        
    } else if(isdigit(caracter)){
        c.compLex = _numerico(caracter);
        if(caracter=='0' && !c.compLex){ // si empieza por 0 y obtenemos error
            devolverCaracteres(tamLexema);
            caracter=sigCaracter();
            c.compLex = '0';
        }
        c.lexema = reconocerLexema(&tamLexemaExcedido);
    } else if(caracter=='#'){
        c.compLex = _comentario(caracter, nLinea);
        c.lexema = reconocerLexema(&tamLexemaExcedido);
    } else if(caracter=='\'' || caracter=='"'){
        c.compLex = _stringsBytesLiterales(caracter);
        if(!c.compLex){ // si hay error al leer el string devuelve la comillas solo
            devolverCaracteres(tamLexema);
            caracter=sigCaracter();
            c.compLex=caracter;
        }
        c.lexema = reconocerLexema(&tamLexemaExcedido);
    } else if(caracter=='.'){
        c.compLex=_flotante();
        if(!c.compLex){
            devolverCaracteres(tamLexema);
            caracter=sigCaracter();
            c.compLex = _operadoresDelimitadores(caracter);
        }
        c.lexema = reconocerLexema(&tamLexemaExcedido);
    } else if(caracter=='\\'){
        c.compLex = caracter;
        c.lexema = reconocerLexema(&tamLexemaExcedido);
    } else{
        c.compLex = _operadoresDelimitadores(caracter);
        c.lexema = reconocerLexema(&tamLexemaExcedido);
    }

    if(tamLexemaExcedido){ // Si se ha excedido el tamano de lexema
        imprimirError(TAM_LEXEMA_EXCEDIDO, nLinea);
    }

    if(!c.compLex){ // Si no se ha detectado ningun componente lexico
        imprimirError(LEXEMA_MAL_FORMADO, nLinea);
    }

    return c;
}
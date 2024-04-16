#include <stdio.h>
#include <stdlib.h>
#include "../includes/sistemaEntrada.h"
#include "../includes/ts.h"
#include "../includes/sintactico.h"

int main(int argc, char** argv){
    char* nombreArchivo = "wilcoxon.py";
    if(argc>1){
        nombreArchivo=*(argv+1);
    }

    inicializarSistemaEntrada(nombreArchivo);

    inicializarTS();
    imprimirTS();

    iniciarSintactico();

    imprimirTS();
    
    liberarTS();

    liberarSistemaEntrada();

    exit(EXIT_SUCCESS);
    
}
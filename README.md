# Analizador léxico del lenguaje Python

## Introducción
Este analizador léxico ha sido realizado para la práctica 1 de la asignatura "Compiladores e Intérpretes" de 3ºGREI en el curso 2023/24. Para su realización se ha seguido la documentación correspondiente al análisis léxico de la propia página de Python, "https://docs.python.org/3/reference/lexical_analysis.html".

## Compilación y ejecución
Para compilar este programa se proporciona un Makefile, por lo que basta con ejecutar el comando 'make' en una terminal de Linux. Una vez compilado, su ejecución se realiza mediante el comando './analizadorLexicoPython [archivo_a_analizar]'. Se podrá proporcionar como parámetro la ruta al archivo a analizar. En caso de que no se haga, se ejecutará por defecto el análisis léxico del programa "wilcoxon.py". En caso de querer realizar un análisis a mayores del uso de memoria del programa, se podrá ejecutar el comando 'valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./analizadorLexicoPython [archivo_a_analizar]'
#include <stdio.h>
#include <bits/getopt_core.h>
#include "../include/comparation.h"
#include "../include/utils.h"
#include "../include/dataStructures.h"

int numThreads;
char *initDir;
char funcMode;

//* Logica principal del programa - diseño por definir
//! Función importante!!
//* @return undefined por los momentos indefinido
void *startSearchDuplicates()
{
    // se crean los hilos..
    printf("Hello world!!\n");
    return NULL;
}

//* Imprime el resultado del analisis del escritorio
void printFormat(void *data)
{
}

void getArguments(int argc, char *argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "t:d:m:")) != -1)
    {
        switch (opt)
        {
        case 't': // Argumento numero de threads
            numThreads = atoi(optarg);
            // printf("numThreads: %d\n", numThreads);
            break;
        case 'd': // Argumento directorio de inicio (absoluta o relativa)
            initDir = optarg;
            // printf("initDir: %s\n", initDir);
            break;
        case 'm': // Argumento modo
            funcMode = *optarg;
            // printf("funcMode: %c\n", funcMode);
            break;
        default:
            //
            break;
        }
    }
}

// ¿Sera que hoy se acaba venezuela dios mio? xd 
// Ok ya paso un dia desde el comentario de arriba, venezuela no se acabo
//! Estado: Destruido, demacrado,vuelto nada, papelón...
int main(int argc, char *argv[])
{
    getArguments(argc, argv);
    void *result = startSearchDuplicates();
    printFormat(result);
    return 0;
}

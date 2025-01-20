#include <stdio.h>
#include <stdlib.h>
#include <bits/getopt_core.h>
#include <pthread.h>
#include "../include/hashComparation.h"
#include "../include/utils.h"
#include "../include/dataStructures.h"
#include "../include/concurrentFile.h"

int numThreads;
char* initDir;
char funcMode;

// Logica principal del programa
// @return DirectoryData*
struct DirectoryData* startSearchDuplicates()
{
    if (numThreads < 1 || initDir == NULL || funcMode == '\0')
    {
        return NULL;
    }

    pthread_t pthreads[numThreads];
    struct DirectoryData* directoryData = initStructDirectoryData(funcMode, initDir);
    if (directoryData == NULL)
        return NULL;

    // Inicializa semaforos
    initSemFile();

    // Crea los hilos
    for (int i = 0; i < numThreads; i++)
    {
        pthread_create(&pthreads[i], NULL, searchFileDuplicates, directoryData);
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(pthreads[i], NULL);
    }
    // liberar hilos
    // free(pthreads);
    return directoryData;
}

// Obtiene los argumentos pasados al ejecutar y los guarda en numThreads, initDir, funcMode
// @param argc
// @param argv
void getArguments(int argc, char* argv[])
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
            funcMode =* optarg;
            // printf("funcMode: %c\n", funcMode);
            break;
        default:
            break;
        }
    }
}

int main(int argc, char* argv[])
{
    // Obtener argumentos
    getArguments(argc, argv);
    // Buscar duplicados
    struct DirectoryData* result = startSearchDuplicates();
    // Imprimir estadisticas
    printFormatFileDuplicates(result);
    // Liberar recursos
    freeDirectoryData(result);

    return 0;
}
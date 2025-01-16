#include <stdio.h>
#include <bits/getopt_core.h>
#include "../include/hashComparation.h"
#include "../include/utils.h"
#include "../include/dataStructures.h"
#include "../include/concurrentFileMd5.h"
#include <pthread.h>

int numThreads;
char *initDir;
char funcMode;

//* Logica principal del programa - diseño por definir
//! Función importante!!
//* @return undefined por los momentos indefinido
struct DirectoryData *startSearchDuplicates()
{

    if (numThreads < 1 || initDir == NULL || funcMode == '\0')
    {
        return NULL;
    }

    pthread_t *pthreads = (pthread_t *)malloc(sizeof(int) * numThreads);
    struct DirectoryData *directoryData = initStructDirectoryData(funcMode, initDir);
    if (directoryData == NULL)
        return NULL;

    // inicializa semaforos
    initSemFile();

    // se crean los hilos..
    for (int i = 0; i < numThreads; i++)
    {
        pthread_create(&pthreads[i], NULL, searchFileDuplicates, directoryData);
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(pthreads[i], NULL);
    }

    return directoryData;
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
    struct DirectoryData *result = startSearchDuplicates();
    if (result == NULL)
    {
        //* Mensaje de error temporal, no se si haya que ponerlos xd.
        printf("Argumentos inconrrectos\n");
        return 1;
    }

    printFormatFileDuplicates(result);
    return 0;
}

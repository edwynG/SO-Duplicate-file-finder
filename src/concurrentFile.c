#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include "../include/concurrentFile.h"
#include "../include/dataStructures.h"
#include "../include/utils.h"
#include <string.h>

// Implementación de concurrentFile.h para más detalle vea dicho archivo

sem_t mutex_advance;

struct DirectoryData *initStructDirectoryData(char funcMode, char *initDir)
{
    if (initDir == NULL || (funcMode != 'e' && funcMode != 'i'))
    {
        return NULL;
    }

    struct DirectoryData *directoryData = (struct DirectoryData *)malloc(sizeof(struct DirectoryData));
    directoryData->funcMode = funcMode;
    directoryData->toVisite = createList(); //* List (char*)
    directoryData->toVisite->addNode(directoryData->toVisite, initDir);
    directoryData->Visited = createList();              //* List (char*)
    directoryData->fileStatistics.numberDuplicate = 0;  // Numero de duplicados
    directoryData->fileStatistics.Files = createList(); // List ( FilesDuplicate* )

    return directoryData;
}

void printFormatFileDuplicates(struct DirectoryData *data)
{
    if (data == NULL)
    {
        return;
    }

    int numDuplicate = data->fileStatistics.numberDuplicate;
    // Extrae la lista de archivos duplicados
    struct List *files = data->fileStatistics.Files; //! Lista de FilesDuplicates

    // Extrae la cabecera de la lista de archivos duplicados
    struct Node *headFiles = (struct Node *)files->getHead(files); //! Extraemos el nodo
    printf("Se han encontrado %d archivos duplicados.\n\n", numDuplicate);

    while (headFiles != NULL)
    {
        // Realiza la converción de cada valor de la lista de archivos duplicados
        struct FilesDuplicates *headFileValue = (struct FilesDuplicates *)headFiles->value;
        char *nameFile = getFileName(headFileValue->file);

        // Toma la cabecera de la lista de duplicados del archivos
        struct Node *headDuplicates = (struct Node *)headFileValue->duplicates->getHead(headFileValue->duplicates);

        while (headDuplicates != NULL)
        {
            // Obtiene el valor del nodo de la lista de duplicados del archivo
            char *nameDuplicate = getFileName((char *)headDuplicates->value);
            printf("%s es duplicado de %s\n", nameDuplicate, nameFile);
            headDuplicates = headDuplicates->next;
        }
        headFiles = headFiles->next;
    }
}

void initSemFile()
{
    sem_init(&mutex_advance, 0, 1);
}

void *searchFileDuplicates(void *arg)
{
    struct DirectoryData *data = (struct DirectoryData *)arg;
    sem_wait(&mutex_advance);
    printf("Directorio actual: %s\n", (char *)data->toVisite->getHead(data->toVisite)->value);
    sem_post(&mutex_advance);
    pthread_exit(NULL);
}
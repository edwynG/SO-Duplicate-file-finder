#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include "../include/hashComparation.h"
#include "../include/concurrentFile.h"
#include "../include/dataStructures.h"
#include "../include/utils.h"
#include <string.h>

// Implementación de concurrentFile.h para más detalle vea dicho archivo

sem_t mutex_advance;

struct DirectoryData *initStructDirectoryData(char funcMode, char *initDir)
{
    if (initDir == NULL || (funcMode != 'e' && funcMode != 'l'))
    {
        return NULL;
    }

    // Ignora directorios de inicio "." ".."
    if (strcmp(initDir, ".") == 0 || strcmp(initDir, "..") == 0)
    {
        return NULL;
    }

    struct DirectoryData *directoryData = (struct DirectoryData *)malloc(sizeof(struct DirectoryData));
    directoryData->funcMode = funcMode;
    directoryData->toVisit = createList(); // List (char*)
    directoryData->toVisit->addNode(directoryData->toVisit, initDir);
    directoryData->Visited = createList();              // List (char*)
    directoryData->fileStatistics.numberDuplicates = 0; // Numero de duplicados
    directoryData->fileStatistics.Files = createList(); // List (FilesDuplicate*)

    return directoryData;
}

void freeDirectoryData(struct DirectoryData *data)
{
    if (data == NULL)
    {
        return;
    }

    data->toVisit->destructor(data->toVisit);
    data->Visited->destructor(data->Visited);
    struct List *files = data->fileStatistics.Files;
    struct Node *headDuplicateNode = files->getHead(files);
    
    // Libera todas las listas que estan en Files 
    while (headDuplicateNode != NULL)
    {
        struct FilesDuplicates *duplicates = (struct FilesDuplicates *)headDuplicateNode->value;
        duplicates->duplicates->destructor(duplicates->duplicates);
        headDuplicateNode = headDuplicateNode->next;
    }

    files->destructor(files);
    free(data);
}

void printFormatFileDuplicates(struct DirectoryData *data)
{
    if (data == NULL)
    {
        return;
    }

    int numDuplicate = data->fileStatistics.numberDuplicates;
    // Extrae la lista de archivos duplicados
    struct List *files = data->fileStatistics.Files; // Lista de FilesDuplicates

    // Extrae la cabecera de la lista de archivos duplicados
    struct Node *headFiles = (struct Node *)files->getHead(files); // Extraemos el nodo
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

    // Mientras que “a visitar” no este vacía
    while (1)
    {
        // Espera
        printf("SEARCHFILEDUPLICATES Esperar...\n");
        sem_wait(&mutex_advance);
        if (isEmpty(data->toVisit))
        {
            printf("SEARCHFILEDUPLICATES Liberar...\n\n");
            sem_post(&mutex_advance); // Libera
            break;                    // Si la lista está vacía, sale del bucle
        }

        printf("SEARCHFILEDUPLICATES Comenzar...\n");

        // Obtiene el siguiente nodo “a visitar”
        struct Node *toVisitNode = data->toVisit->getHead(data->toVisit);
        printf("SEARCHFILEDUPLICATES toVisitNode %s\n", (char *)toVisitNode->value);

        // Determina tipo
        struct stat info;
        if (lstat((char *)toVisitNode->value, &info) == 0)
        {

            if (getType(info.st_mode) == 'd')
            { // Si es un directorio
                printf("SEARCHFILEDUPLICATES Directorio\n");

                // Enumera los archivos que contiene y guarda registros acerca de ellos en la estructura de datos “a visitar”
                // TOFIX: enumerar?
                directoryTour((char *)toVisitNode->value, data->toVisit);
            }
            else
            {
                if (info.st_size != 0)
                { // Si es un archivo de datos no vacío
                    printf("SEARCHFILEDUPLICATES Archivo\n");

                    // Comprueba la igualdad contra los hashes de todos los archivos en la estructura de datos “visitados”
                    struct Node *toCompareNode = data->Visited->getHead(data->Visited);
                    struct FilesDuplicates *dataDuplicate = (struct FilesDuplicates *)malloc(sizeof(struct FilesDuplicates));
                    dataDuplicate->file = (char *)toVisitNode->value;
                    dataDuplicate->duplicates = createList();

                    while (toCompareNode != NULL)
                    {
                        printf("SEARCHFILEDUPLICATES toCompareNode %s\n", (char *)toCompareNode->value);

                        if (hashComparation(data->funcMode, toVisitNode->value, toCompareNode->value))
                        {
                            data->fileStatistics.numberDuplicates++;
                            dataDuplicate->duplicates->addNode(dataDuplicate->duplicates, (char *)toCompareNode->value);
                        }
                        toCompareNode = toCompareNode->next;
                    }

                    // Si no hay archivos duplicados evita registrar
                    if (isEmpty(dataDuplicate->duplicates))
                    {
                        dataDuplicate->duplicates->destructor(dataDuplicate->duplicates);
                        free(dataDuplicate);
                    }
                    else
                    {
                        data->fileStatistics.Files->addNode(data->fileStatistics.Files, dataDuplicate);
                    }

                    // Agrega el archivo que se acaba de verificar a “visitados”
                    data->Visited->addNode(data->Visited, toVisitNode->value);
                }
            }
        }

        // Remueve el archivo que se acaba de verificar de “a visitar”
        data->toVisit->removeNode(data->toVisit, toVisitNode);

        // Libera
        printf("SEARCHFILEDUPLICATES Liberar...\n\n");
        sem_post(&mutex_advance);
    }

    pthread_exit(NULL);
}
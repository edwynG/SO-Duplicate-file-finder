#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include "../include/concurrentFile.h"
#include "../include/dataStructures.h"
#include "../include/utils.h"
#include <string.h>
#include "../include/hashComparation.h"


// Implementación de concurrentFile.h para más detalle vea dicho archivo

sem_t mutexToVisit;
sem_t mutexVisited;

struct DirectoryData *initStructDirectoryData(char funcMode, char *initDir)
{
    // Valida argumentos ingresados
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
    addNode(directoryData->toVisit, initDir);
    setHashNode(directoryData->toVisit->tail, "init");
    directoryData->Visited = createList(); // List (char*)
    directoryData->fileStatistics = (struct FileStatistics *)malloc(sizeof(struct FileStatistics));
    directoryData->fileStatistics->numberDuplicates = 0; // Numero de duplicados
    directoryData->fileStatistics->Files = createList(); // List (FilesDuplicate*)

    return directoryData;
}

void freeDirectoryData(struct DirectoryData *data)
{
    if (data == NULL)
    {
        return;
    }

    destructor(data->toVisit);
    destructor(data->Visited);
    struct List *files = data->fileStatistics->Files;
    struct Node *headDuplicateNode = getHead(files);

    // Libera todas las listas que estan en Files
    while (headDuplicateNode != NULL)
    {
        struct FilesDuplicates *duplicates = (struct FilesDuplicates *)headDuplicateNode->value;
        destructor(duplicates->duplicates);
        headDuplicateNode = headDuplicateNode->next;
    }

    free(data->fileStatistics);
    destructor(files);
    free(data);
    sem_destroy(&mutexToVisit);
    sem_destroy(&mutexVisited);
}

void printFormatFileDuplicates(struct DirectoryData *data)
{
    if (data == NULL)
    {
        return;
    }

    int numDuplicate = data->fileStatistics->numberDuplicates;
    // Extrae la lista de archivos duplicados
    struct List *files = data->fileStatistics->Files; // Lista de FilesDuplicates

    // Extrae la cabecera de la lista de archivos duplicados
    struct Node *headFiles = (struct Node *)getHead(files); // Extraemos el nodo
    printf("Se han encontrado %d archivos duplicados.\n\n", numDuplicate);

    while (headFiles != NULL)
    {
        // Realiza la converción de cada valor de la lista de archivos duplicados
        struct FilesDuplicates *headFileValue = (struct FilesDuplicates *)headFiles->value;
        char *nameFile = getFileName(headFileValue->file);

        // Toma la cabecera de la lista de duplicados del archivos
        struct Node *headDuplicates = (struct Node *)getHead(headFileValue->duplicates);

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
    sem_init(&mutexToVisit, 0, 1);
    sem_init(&mutexVisited, 0, 1);
}

void *searchFileDuplicates(void *arg)
{
    struct DirectoryData *data = (struct DirectoryData *)arg;
    
    while (1)
    {
        // Espera
        sem_wait(&mutexToVisit);
        // Mientras que "a visitar" no este vacía
        if (isEmpty(data->toVisit))
        {
            sem_post(&mutexToVisit); // Libera
            break; // Si la lista está vacía, sale del bucle
        }

        // Obtiene el siguiente nodo “a visitar”
        struct Node *toVisitNode = getHead(data->toVisit);

        // Remueve el archivo que se acaba de verificar de "a visitar" (cambia el primero pero no libera el espacio del nodo)
        data->toVisit->head = toVisitNode->next;
        if (data->toVisit->head != NULL)
        {
            data->toVisit->head->before = NULL;
        }
        else
        {
            data->toVisit->tail = NULL; // Si la lista queda vacía
        }

        // Libera
        sem_post(&mutexToVisit);
             
        // Determina tipo
        struct stat info;
        if (lstat((char *)toVisitNode->value, &info) == 0)
        {
            // Espera
            sem_wait(&mutexVisited);

            // Estructura que contine la estadistica
            struct FileStatistics *fileStatistics = data->fileStatistics;

            // Lista de duplicados para la estadistica, cada nodo es una categoria o particion
            struct List *categoryList = fileStatistics->Files;

            // Comprueba la igualdad contra los hashes de todos los archivos en la estructura de datos "visitados"
           
            struct Node *toCompareNode = getHead(data->Visited);
         
            while (toCompareNode != NULL)
            {
                 
                // Se crea una categoria o partición del nodo visitado
                struct FilesDuplicates *dataCategory = (struct FilesDuplicates *)malloc(sizeof(struct FilesDuplicates));
                dataCategory->hash = (char *)malloc(strlen(toCompareNode->hash) + 1);
                strcpy(dataCategory->hash, toCompareNode->hash);
                dataCategory->file = (char *)toCompareNode->value;
                dataCategory->duplicates = createList();

                if (hashComparation(toVisitNode->hash, toCompareNode->hash))
                {
                    // Variable para almacenar la categoria o particion del archivo a guardar, si es que existe
                    struct FilesDuplicates *parentCategory = NULL;

                    // Verifica si el archivo pertenece a alguna de las categorias
                    if (isIncludedCategory(categoryList, toVisitNode->hash, &parentCategory))
                    {
                        // Agrega el nodo 'a visitar' a su categoria correspondiente
                        addNode(parentCategory->duplicates, (char *)toVisitNode->value);
                        fileStatistics->numberDuplicates++;
                        // Libera la categoria o particion sin uso
                        destructor(dataCategory->duplicates);
                        free(dataCategory);
                        // Detiene el bucle porque ya se encontro la categoria a la que pertenece el archivo
                        break;
                    }
                    // Crea una categoria nueva si el archivo no pertenece a ninguna
                    if (parentCategory == NULL)
                    {
                        addNode(dataCategory->duplicates, toVisitNode->value);
                        addNode(categoryList, dataCategory);
                        fileStatistics->numberDuplicates++;
                        break;
                    }
                }
                // Libera la categoria o particion si no tiene duplicados(significa que no tiene uso)
                if (isEmpty(dataCategory->duplicates))
                {
                    destructor(dataCategory->duplicates);
                    free(dataCategory);
                }
                toCompareNode = toCompareNode->next;
            }
            // Agrega el archivo que se acaba de verificar a "visitados"
            addNode(data->Visited, toVisitNode->value);
            setHashNode(data->Visited->tail,toVisitNode->hash);

            // Libera el espacio del nodo
            free(toVisitNode);

            // Libera
            sem_post(&mutexVisited);
            
        }
        else
        {
            // Libera
            sem_post(&mutexToVisit);
            break; 
        }
        
    }
    pthread_exit(NULL);
}
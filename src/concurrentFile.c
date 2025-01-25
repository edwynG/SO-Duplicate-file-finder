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
    directoryData->toVisit->addNode(directoryData->toVisit, initDir);
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

    data->toVisit->destructor(data->toVisit);
    data->Visited->destructor(data->Visited);
    struct List *files = data->fileStatistics->Files;
    struct Node *headDuplicateNode = files->getHead(files);

    // Libera todas las listas que estan en Files
    while (headDuplicateNode != NULL)
    {
        struct FilesDuplicates *duplicates = (struct FilesDuplicates *)headDuplicateNode->value;
        duplicates->duplicates->destructor(duplicates->duplicates);
        headDuplicateNode = headDuplicateNode->next;
    }

    free(data->fileStatistics);
    files->destructor(files);
    free(data);
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

    // Mientras que "a visitar" no este vacía
    while (1)
    {
        // Espera
        sem_wait(&mutex_advance);
        if (isEmpty(data->toVisit))
        {
            sem_post(&mutex_advance); // Libera
            break; // Si la lista está vacía, sale del bucle
        }

        // Obtiene el siguiente nodo “a visitar”
        struct Node *toVisitNode = data->toVisit->getHead(data->toVisit);

        // Determina tipo
        struct stat info;
        if (lstat((char *)toVisitNode->value, &info) == 0)
        {

            if (getType(info.st_mode) == 'd')
            { // Si es un directorio

                // Enumera los archivos que contiene y guarda registros acerca de ellos en la estructura de datos “a visitar”
                directoryTour((char *)toVisitNode->value, data->toVisit);
            }
            else
            {
                if (info.st_size != 0)
                { // Si es un archivo de datos no vacío

                    // Estructura que contine la estadistica
                    struct FileStatistics *fileStatistics = data->fileStatistics;

                    // Lista de duplicados para la estadistica, cada nodo es una categoria o particion
                    struct List *categoryList = fileStatistics->Files;

                    // Comprueba la igualdad contra los hashes de todos los archivos en la estructura de datos "visitados"
                    struct Node *toCompareNode = data->Visited->getHead(data->Visited);

                    while (toCompareNode != NULL)
                    {
                        // Se crea una categoria o partición del nodo visitado
                        struct FilesDuplicates *dataCategory = (struct FilesDuplicates *)malloc(sizeof(struct FilesDuplicates));
                        dataCategory->file = (char *)toCompareNode->value;
                        dataCategory->duplicates = createList();

                        if (hashComparation(data->funcMode, toVisitNode->value, toCompareNode->value))
                        {
                            // Variable para almacenar la categoria o particion del archivo a guardar, si es que existe
                            struct FilesDuplicates *parentCategory = NULL;

                            // Verifica si el archivo pertenece a alguna de las categorias
                            if (isIncludedCategory(categoryList, toVisitNode->value, data->funcMode, &parentCategory))
                            {

                                // Agrega el nodo 'a visitar' a su categoria correspondiente
                                parentCategory->duplicates->addNode(parentCategory->duplicates, (char *)toVisitNode->value);
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
                                dataCategory->duplicates->addNode(dataCategory->duplicates, toVisitNode->value);
                                categoryList->addNode(categoryList, dataCategory);
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
                    data->Visited->addNode(data->Visited, toVisitNode->value);
                }
            }

            // Remueve el archivo que se acaba de verificar de "a visitar"
            data->toVisit->removeNode(data->toVisit, toVisitNode);

            // Libera
            sem_post(&mutex_advance);
        }
    }

    pthread_exit(NULL);
}
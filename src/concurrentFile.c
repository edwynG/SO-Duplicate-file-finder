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

struct DirectoryData* initStructDirectoryData(char funcMode, char* initDir)
{
    if (initDir == NULL || (funcMode != 'e' && funcMode != 'l'))
    {
        return NULL;
    }

    struct DirectoryData* directoryData = (struct DirectoryData*)malloc(sizeof(struct DirectoryData));
    directoryData->funcMode = funcMode;
    directoryData->toVisit = createList(); // List (char*)
    directoryData->toVisit->addNode(directoryData->toVisit, initDir);
    directoryData->Visited = createList();              // List (char*)
    directoryData->fileStatistics.numberDuplicates = 0;  // Numero de duplicados
    directoryData->fileStatistics.Files = createList(); // List (FilesDuplicate*)

    return directoryData;
}

void printFormatFileDuplicates(struct DirectoryData* data)
{
    if (data == NULL)
    {
        return;
    }

    int numDuplicate = data->fileStatistics.numberDuplicates;
    // Extrae la lista de archivos duplicados
    struct List* files = data->fileStatistics.Files; // Lista de FilesDuplicates

    // Extrae la cabecera de la lista de archivos duplicados
    struct Node* headFiles = (struct Node*)files->getHead(files); // Extraemos el nodo
    printf("Se han encontrado %d archivos duplicados.\n\n", numDuplicate);

    while (headFiles != NULL)
    {
        // Realiza la converción de cada valor de la lista de archivos duplicados
        struct FilesDuplicates* headFileValue = (struct FilesDuplicates*)headFiles->value;
        char* nameFile = getFileName(headFileValue->file);

        // Toma la cabecera de la lista de duplicados del archivos
        struct Node* headDuplicates = (struct Node*)headFileValue->duplicates->getHead(headFileValue->duplicates);

        while (headDuplicates != NULL)
        {
            // Obtiene el valor del nodo de la lista de duplicados del archivo
            char* nameDuplicate = getFileName((char*)headDuplicates->value);
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

void* searchFileDuplicates(void* arg)
{
    struct DirectoryData* data = (struct DirectoryData*)arg;

    // Mientras que “a visitar” no este vacía
    while(!isEmpty(data->toVisit)){
        // Esperar
        printf("Esperar\n");
        sem_wait(&mutex_advance);
        printf("Comenzar\n");

        // Obtener el siguiente nodo “a visitar”
        struct Node* toVisitNode = data->toVisit->getHead(data->toVisit);
        printf("toVisitNode %s\n", (char*)toVisitNode->value);

        // Determinar tipo
        struct stat info;
        if(lstat((char*)toVisitNode->value, &info) == 0){ // TOFIX: path completo?
            if(getType(info.st_mode) == 'd'){ // Si es un directorio
                printf("Directorio\n");
                // TOFIX: enumerar?
                // Enumerar los archivos que contiene y guardar registros acerca de ellos en la estructura de datos “a visitar”
                directoryTour((char*)toVisitNode->value, data->toVisit);

            }else{
                if(info.st_size != 0){ // Si es un archivo de datos no vacío
                    printf("Archivo\n");
                    // Comprobar la igualdad contra los hashes de todos los archivos en la estructura de datos “visitados”
                    struct Node* currentNode = data->Visited->getHead(data->Visited);
                    while(currentNode != NULL){
                        if(hashComparation(data->funcMode, toVisitNode->value, currentNode->value)){
                            printf("Copia\n");
                            // TOFIX: estadisticas
                            // data->fileStatistics.numberDuplicates++; 
                            // data->fileStatistics.Files->addNode(data->fileStatistics.Files, toVisitNode->value);
                        }
                        currentNode = currentNode->next;
                    }
                    // Agregar el archivo que se acaba de verificar a “visitados” 
                    data->Visited->addNode(data->Visited, toVisitNode->value);
                }
            }
        }
        
        // Remover el archivo que se acaba de verificar de “a visitar”
        data->toVisit->removeNode(data->toVisit, toVisitNode);

        // Liberar
        sem_post(&mutex_advance);
    }
        
    pthread_exit(NULL);
}
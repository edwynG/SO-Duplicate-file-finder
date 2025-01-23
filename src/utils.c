#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../include/utils.h"
#include "../include/dataStructures.h"
#include "../include/hashComparation.h"

// Implementación de utils.h para más detalle vea dicho archivo

char *getFileName(char *path)
{
    // Busca la última aparición de '/' o '\' en la ruta
    const char *lastSlash = strrchr(path, '/');
    const char *lastBackslash = strrchr(path, '\\');

    // Determina cuál de los dos es el último
    if (lastSlash == NULL && lastBackslash == NULL)
    {
        return NULL; // No hay barras, devuelve NULL
    }
    else if (lastSlash == NULL)
    {
        return (char *)(lastBackslash + 1); // Solo hay backslash
    }
    else if (lastBackslash == NULL)
    {
        return (char *)(lastSlash + 1); // Solo hay slash
    }
    else
    {
        // Compara qué resultado tiene más caracteres y lo retorna
        return (char *)(lastSlash > lastBackslash ? lastSlash + 1 : lastBackslash + 1);
    }
}

char getType(unsigned int mode)
{
    if (S_ISDIR(mode))
    {
        return 'd';
    }
    else if (S_ISLNK(mode))
    {
        return 'l';
    }
    else
    {
        return 'f';
    }
}

void directoryTour(char *DirectoryName, struct List *toVisit)
{
    if (strcmp(DirectoryName, ".") == 0 || strcmp(DirectoryName, "..") == 0)
    {
        return;
    }

    DIR *directory = opendir(DirectoryName);
    // Verifica que sea un directorio
    if (directory == NULL)
    {
        return; // Si no lo es termina
    }

    struct dirent *input;
    while ((input = readdir(directory)) != NULL)
    {
        // Ignora los directorios "." y ".."
        if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0)
        {
            char fullPath[1024];
            snprintf(fullPath, sizeof(fullPath), "%s/%s", DirectoryName, input->d_name);

            // Usa lstat para obtener información sobre el archivo
            struct stat info;
            if (lstat(fullPath, &info) == 0)
            {
                // Si es un enlace simbólico se ignora
                if (getType(info.st_mode) != 'l')
                {
                    // Genera copia para no sobreescribir atributo value del Node
                    char *fullPathCopy = (char *)malloc(strlen(fullPath) + 1);
                    strcpy(fullPathCopy, fullPath);
                    printf("DIRECTORYTOUR fullPath %s\n", fullPathCopy);
                    // Agrega el nodo a "a visitar"
                    toVisit->addNode(toVisit, fullPathCopy);
                }
            }
        }
    }

    closedir(directory);
}

int isIncludedCategory(struct List *listCategory, char *value, char type, struct FilesDuplicates **parentNode)
{
    if (listCategory == NULL || value == NULL)
    {
        return 0;
    }

    struct Node *current = listCategory->head;
    while (current != NULL)
    {
        struct FilesDuplicates *currentData = (struct FilesDuplicates *)current->value;
        if (hashComparation(type, currentData->file, value))
        {
            (*parentNode) = currentData;
            return 1; // El valor está en la lista
        }
        current = current->next;
    }
    (*parentNode) = NULL;
    return 0; // El valor no está en la lista
}
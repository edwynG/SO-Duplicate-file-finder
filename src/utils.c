#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../include/utils.h"
#include "../include/dataStructures.h"

// Implementación de utils.h para más detalle vea dicho archivo

char* getFileName(char* path)
{
    // Busca la última aparición de '/' o '\' en la ruta
    const char* lastSlash = strrchr(path, '/');
    const char* lastBackslash = strrchr(path, '\\');

    // Determina cuál de los dos es el último
    if (lastSlash == NULL && lastBackslash == NULL)
    {
        return NULL; // No hay barras, devuelve NULL
    }
    else if (lastSlash == NULL)
    {
        return (char*)(lastBackslash + 1); // Solo hay backslash
    }
    else if (lastBackslash == NULL)
    {
        return (char*)(lastSlash + 1); // Solo hay slash
    }
    else
    {
        // Comapara que resultado tiene más caracteres y lo retorna
        return (char*)(lastSlash > lastBackslash ? lastSlash + 1 : lastBackslash + 1);
    }
}

struct List* directoryTour(char* DirectoryName)
{
    if (strcmp(DirectoryName, ".") || strcmp(DirectoryName, ".."))
        return NULL;

    struct dirent* input;
    DIR* directory = opendir(DirectoryName);
    // Verifica que se aun directorio
    if (directory == NULL)
        return NULL; // Si no lo es retorna NULL

    struct List* toVisite = createList();
    while ((input = readdir(directory)) != NULL)
    {
        // Ignorar los directorys "." y ".."
        if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0)
        {
            char pathComplet[1024];
            snprintf(pathComplet, sizeof(pathComplet), "%s/%s", DirectoryName, input->d_name);

            struct stat info;
            // Usar lstat para obtener información sobre el archivo
            if (lstat(pathComplet, &info) == 0)
            {
                // Ignorar enlaces simbólicos
                if (!S_ISLNK(info.st_mode))
                {
                    toVisite->addNode(toVisite, pathComplet);
                    // Si es un directory, llamar recursivamente
                    // Implementación antigua..
                    // if (S_ISDIR(info.st_mode))
                    // {
                    //     directoryTour(pathComplet);
                    // }
                }
            }
        }
    }

    closedir(directory);
    return toVisite;
}
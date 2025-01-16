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

char getType(unsigned int mode){
    if (S_ISDIR(mode)) {
        return 'd';
    } else if (S_ISLNK(mode)) {
        return 'l';
    } else {
        return 'f';
    }
}

void directoryTour(char* DirectoryName, struct List* toVisit){
    if (!strcmp(DirectoryName, ".") || !strcmp(DirectoryName, "..")){
        return;
    }

    struct dirent* input;
    DIR* directory = opendir(DirectoryName);
    // Verifica que sea un directorio
    if (directory == NULL){
        return; // Si no lo es termina
    }
    
    while ((input = readdir(directory)) != NULL)
    {
        // Ignorar los directorios "." y ".."
        if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0)
        {
            char fullPath[1024];
            snprintf(fullPath, sizeof(fullPath), "%s/%s", DirectoryName, input->d_name);

            struct stat info;
            // Usar lstat para obtener información sobre el archivo
            if (lstat(fullPath, &info) == 0)
            {
                // Si es un enlace simbólico: se ignora
                if (getType(info.st_mode) != 'l')
                {
                    printf("fullPath %s\n", fullPath);
                    toVisit->addNode(toVisit, fullPath);
                }
            }
        }
    }
    closedir(directory);
}
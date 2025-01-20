// Evita que utils.h se cargue más de una vez durante el preprocesamiento
#ifndef UTILS
#define UTILS

#include "dataStructures.h"

// Devuelve el nombre de un archivo o directorio
// @param path ruta del arhivo o directorio
// @return (char*)
char* getFileName(char* path);

// Devuelve el tipo: directorio, enlace simbólico o archivo
// @param mode
// @return (char) 'd', 'l', 'f'
char getType(unsigned int mode);

// Agrega a una lista todas los archivos y carpetas de un directorio
// @param DirectoryName ruta del directorio
// @param toVisit lista a usar
void directoryTour(char* DirectoryName, struct List* toVisit);

#endif
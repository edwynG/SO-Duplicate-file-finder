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

// Lista todas los archivos y carpetas de un directorio
// @param DirectoryName ruta del directorio
// @return List* (char*)
struct List* directoryTour(char* DirectoryName);

#endif
// Evita que utils.h se cargue mas de una vez durante el preprocesamiento
#ifndef UTILS
#define UTILS

#include "dataStructures.h"

#define UTILS // TOFIX: por que repetir?

// Devuelve el nombre de un archivo o directorio
// @param path ruta del arhivo o directorio
// @return (char*)
char *getFileName(char *path);

// Lista todas los archivos y carpetas de un directorio
// @param DirectoryName ruta del directorio
// @return List* (char*)
struct List *directoryTour(char *DirectoryName);

#endif
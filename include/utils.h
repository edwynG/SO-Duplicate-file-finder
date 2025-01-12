// Evita que utils.h se cargue mas de una ves durante el preprocesamiento
#ifdef UTILS
#include "dataStructures.h"

#define UTILS
//* Devuelve el nombre de un archivo o directorio
//* @param path ruta del arhivo o directorio
//* @return  (char*)
char *getFileName(const char *path);

//* Lista todas los archivos y carpetas de un directorio
//* @param DirectoryName ruta del directorio
//* @return List* (char*) 
struct List* directoryTour(char *DirectoryName);

#endif
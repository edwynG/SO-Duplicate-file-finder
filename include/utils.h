#include "dataStructures.h"
// Evita que utils.h se cargue más de una vez durante el preprocesamiento
#ifndef UTILS
#define UTILS

// Devuelve el nombre de un archivo o directorio
// @param path ruta del arhivo o directorio
// @return (char*)
char *getFileName(char *path);

// Devuelve el tipo: directorio, enlace simbólico o archivo
// @param mode
// @return (char) 'd', 'l', 'f'
char getType(unsigned int mode);

// Agrega a una lista todas los archivos y carpetas de un directorio
// @param DirectoryName ruta del directorio
// @param toVisit lista a usar
// @param Modo de calculo de hash
void directoryTour(char *DirectoryName, struct List *toVisit, char funcMode);

// Verifica si un archivo pertence a una categoría o partición. Si pertenece, indica la categoria mediante un parametro por referencia
// @param list Un puntero a la estructura List
// @param value Una cadena que representa el valor a verificar
// @param parentNode Un apuntador a un puntero de una estructura FilesDuplicates que representará la categoria a la que pertenezca el archivo
// @return (int) que indica si la categoría está incluida (1) o no (0)
int isIncludedCategory(struct List *listCategory, char *hashCompare, struct FilesDuplicates **parentNode);
#endif
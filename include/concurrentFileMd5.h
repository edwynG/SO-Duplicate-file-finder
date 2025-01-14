#ifndef CONCURRENT_FILE_MD5
#define CONCURRENT_FILE_MD5
#include "dataStructures.h"

// Estructura principal para trabajar la infomación manjeda entre hilos
struct DirectoryData
{
    char funcMode;
    struct List *toVisite; // List (char*)
    struct List *Visited;  // List (char* )
    struct FileStatistics fileStatistics;
};

// Función para definir la estructura de datos sobre donde trabajaran los hilos
struct DirectoryData *initStructDirectoryData(char funcMode, char *initDir);

//* Función para buscar los archivos duplicados
// Encuentra los archivos duplicados en un directorio dado atra ves de 
// una estructura (struct DirectoryData).Esta función es especial para
// trabajar con hilos
void *searchFileDuplicates(void *arg);

//* Imprime el resultado del analisis del escritorio
void printFormatFileDuplicates(struct DirectoryData *data);

void initSemFile();

#endif
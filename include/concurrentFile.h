#ifndef CONCURRENT_FILE
#define CONCURRENT_FILE
#include "dataStructures.h"

// Estructura principal para trabajar la infomación manejada entre hilos
struct DirectoryData
{
    char funcMode;
    struct List *toVisite; // List (char*)
    struct List *Visited;  // List (char*)
    struct FileStatistics fileStatistics;
};

// Metodos

// Define la estructura de datos sobre donde trabajaran los hilos
struct DirectoryData *initStructDirectoryData(char funcMode, char *initDir);

// Encuentra los archivos duplicados en un directorio dado atra vez de 
// una estructura (struct DirectoryData).Esta función es especial para
// trabajar con hilos
void *searchFileDuplicates(void *arg);

// Imprime el resultado del analisis del escritorio
void printFormatFileDuplicates(struct DirectoryData *data);

void initSemFile();

#endif
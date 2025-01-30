// Evita que hashComparation.h se cargue más de una vez durante el preprocesamiento
#ifndef HASH_COMPARATION
#define HASH_COMPARATION

// Calcula el hash MD5 de los contenidos del archivo
// @param filename nombre del archivo al cual se le calculará el hash md5
// @param hashValue arreglo de tipo char donde se almacenara el valor del hash md5 del archivo filename
// @return (int)
int MDFile(char* filename, char hashValue[33]);

// Calcula usando el modo ejecutable/modo libreria el hash de un archivo
// @param funcMode 'e'/'l'
// @param file1 nombre del archivo al cual se le calculará el hash md5
void hashCalculation(char funcMode, char *file1, char hash1[]);

// Compara el hash de dos archivos
// @param hash1 del primer archivo
// @param hash2 del segundo archivo
// @return (int) 1 si son iguales 0 de lo contrario
int hashComparation(char hash1[], char hash2[]);

#endif
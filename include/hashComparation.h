// Evita que hashComparation.h se cargue más de una vez durante el preprocesamiento
#ifndef HASH_COMPARATION
#define HASH_COMPARATION

// Calcula el hash MD5 de los contenidos del archivo
// @param filename nombre del archivo al cual se le calculará el hash md5
// @param hashValue arreglo de tipo char donde se almacenara el valor del hash md5 del archivo filename
// @return (int)
int MDFile(char* filename, char hashValue[33]);

// Calcula usando el modo ejecutable/modo libreria el hash de dos archivos y los compara
// @param funcMode 'e'/'l'
// @param file1 nombre del archivo al cual se le calculará el hash md5
// @param file2 nombre del archivo al cual se le calculará el hash md5 
// @return (int) 1 si son iguales 0 de lo contrario
int hashComparation(char funcMode, char* file1, char* file2);

#endif
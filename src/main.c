#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <bits/getopt_core.h>
#include "../include/structure.h"
#include "../include/comparation.h"

int numThreads;
char* initDir;
char funcMode;

void directoryTour(char *DirectoryName) {
    struct dirent *input;
    DIR *directory = opendir(DirectoryName);

    if (directory == NULL) {
        perror("No se puede abrir el directory");
        return;
    }

    while ((input = readdir(directory)) != NULL) {
        // Ignorar los directorys "." y ".."
        if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0) {
            char pathComplet[1024];
            snprintf(pathComplet, sizeof(pathComplet), "%s/%s", DirectoryName, input->d_name);
            
            struct stat info;
            // Usar lstat para obtener información sobre el archivo
            if (lstat(pathComplet, &info) == 0) {
                // Ignorar enlaces simbólicos
                if (!S_ISLNK(info.st_mode)) {
                    printf("%s\n", pathComplet);
                    // Si es un directory, llamar recursivamente
                    if (S_ISDIR(info.st_mode)) {
                        directoryTour(pathComplet);
                    }
                }
            }
        }
    }

    closedir(directory);
}

void getArguments(int argc, char *argv[]){
    int opt;

    while((opt = getopt(argc, argv, "t:d:m:")) != -1) {
        switch (opt) {
            case 't': // Argumento numero de threads
                numThreads = atoi(optarg);
                // printf("numThreads: %d\n", numThreads);
                break;
            case 'd': // Argumento directorio de inicio (absoluta o relativa)
                initDir = optarg;
                // printf("initDir: %s\n", initDir);
                break;
            case 'm': // Argumento modo 
                funcMode = *optarg;
                // printf("funcMode: %c\n", funcMode);
                break;
            default:
                //
                break;
        }
    }
}

int main(int argc, char *argv[]){
    getArguments(argc, argv);
    // hashComparation(funcMode, "archivo1.txt", "archivo2.txt");
    return 0;
}

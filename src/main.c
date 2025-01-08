#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include "../include/structure.h"


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

int main(int argc, char const *argv[])
{
    printf("Hello world\n");
    return 0;
}

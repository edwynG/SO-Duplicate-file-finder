#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../include/hashComparation.h"
#include "../include/md5ArrayBinary.h"
// Implementación de hashComparation.h para más detalle vea dicho archivo

int hashComparation(char funcMode, char *file1, char *file2)
{
    int isEqual = 0;
    if (funcMode == 'e')
    { // Modo ejecutable (md5-app)
        //Ubicacion del archivo temporal
        char *dirMd5 = "/tmp/md5"; // Nombre único
        int pipe1[2]; // 0 extremo lectura, 1 extremo escritura
        int pipe2[2]; // 0 extremo lectura, 1 extremo escritura
        if (pipe(pipe1) == -1)
        {
            perror("Error pipe");
            exit(1);
        }
        // Crear un archivo temporal
        FILE *fp = fopen(dirMd5, "wb");
        if (fp == NULL)
        {
            perror("Error al crear el archivo temporal");
            exit(1);
        }
        // Escribir el contenido del ejecutable en el archivo temporal
        fwrite(md5, sizeof(unsigned char), md5_len, fp);
        fclose(fp);

        // Cambiar los permisos del archivo para hacerlo ejecutable
        chmod(dirMd5, 0755);

        // Crea child1
        int child1 = fork();
        if (child1 < 0)
        {
            perror("Error fork");
            exit(1);
        }

        if (child1 == 0)
        { // Proceso child1
            close(pipe1[0]);
            dup2(pipe1[1], STDOUT_FILENO);// Redirige consola al pipe
            execlp(dirMd5, dirMd5, file1, (char *)NULL); // Reemplaza el proceso por el código MD5 y le pasa como argumento file1

            perror("Error execlp");
            exit(1);
        }
        else
        { // Proceso parent
            // Crea pipe2
            if (pipe(pipe2) == -1)
            {
                perror("Error pipe");
                exit(1);
            }

            // Crea child2
            int child2 = fork();
            if (child2 < 0)
            {
                perror("Error fork");
                exit(1);
            }

            if (child2 == 0)
            { // Proceso child2
                close(pipe2[0]);
                dup2(pipe2[1], STDOUT_FILENO); // Redirige consola al pipe
                execlp(dirMd5, dirMd5, file2, (char *)NULL); // Reemplaza el proceso por el código MD5 y le pasa como argumento file2
                perror("Error execlp");
                exit(1);
            }
            else
            { // Proceso parent
                close(pipe1[1]);
                close(pipe2[1]);
                // Lee el hash del primer archivo
                char hash1[33];
                read(pipe1[0], hash1, 32);
                hash1[32] = '\0';
                // Lee el hash del segundo archivo
                char hash2[33];
                read(pipe2[0], hash2, 32);
                hash2[32] = '\0';

                if (strcmp(hash1, hash2) == 0)
                { // Compara hashes
                    isEqual = 1;
                }
            }
        }
        // Elimina el archivo temporal
        unlink(dirMd5);
 
    }
    else if (funcMode == 'l')
    { // Modo biblioteca (md5-lib)
        char hash1[33];
        char hash2[33];
        if (MDFile(file1, hash1) && MDFile(file2, hash2))
        {
            if (strcmp(hash1, hash2) == 0)
            { // Compara hashes
                isEqual = 1;
            }
        }
        else
        {
            printf("Error en MDFile\n");
        }
    }
    return isEqual;
}
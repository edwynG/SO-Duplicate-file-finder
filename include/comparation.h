#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Calcular el hash MD5 de los contenidos del archivo

int MDFile(char* filename, char hashValue[33]);

int hashComparation(char funcMode, char* file1, char* file2){
    int equal = 0;
    
    if(funcMode == 'e'){ // Modo ejecutable (md5-app)
        // 1) Invocar el comando make y obtener el ejecutable
        // 2) Escribir en la linea de comandos ./md5 <nombrearchivo> para imprimir el hash MD5 del archivo <nombrearchivo>
        // 3) Crear tubería entre una invocación con fork del código MD5 y su programa
        // 4) Leer el hash de la tubería
        
        // Crear pipe1
        int pipe1[2]; // 0 extremo lectura, 1 extremo escritura
        int pipe2[2]; // 0 extremo lectura, 1 extremo escritura
        if(pipe(pipe1) == -1){
            perror("pipe");
            exit(1);
        }

        // Crear child1
        int child1 = fork();
        if(child1 < 0){
            perror("fork");
            exit(1);
        }

        if(child1 == 0){ // Proceso child1
            close(pipe1[0]); 
            dup2(pipe1[1], STDOUT_FILENO); // Redirigir consola al pipe
            execlp("resources/md5-app/md5", "resources/md5-app/md5", file1, (char *)NULL);
            
            perror("execlp");
            exit(1);
        }else{ // Proceso parent
            // Crear pipe2
            if(pipe(pipe2) == -1){
                perror("pipe");
                exit(1);
            }

            // Crear child2
            int child2 = fork();
            if(child2 < 0){
                perror("fork");
                exit(1);
            }

            if(child2 == 0){ // Proceso child2
                close(pipe2[0]); 
                dup2(pipe2[1], STDOUT_FILENO); // Redirigir consola al pipe
                execlp("resources/md5-app/md5", "resources/md5-app/md5", file2, (char *)NULL);
                
                perror("execlp");
                exit(1);
            }else{ // Proceso parent
                close(pipe1[1]);
                close(pipe2[1]);

                // Leer el hash del primer archivo
                char hash1[33];
                read(pipe1[0], hash1, 32);
                hash1[32] = '\0';

                // Leer el hash del segundo archivo
                char hash2[33];
                read(pipe2[0], hash2, 32);
                hash2[32] = '\0';

                printf("hash1 %s\n", hash1);
                printf("hash2 %s\n", hash2);
                if(strcmp(hash1, hash2) == 0){ // Comparar hashes
                    equal = 1;
                }
            }
        }
    }else if(funcMode == 'l'){ // Modo biblioteca (md5-lib)
        // 1) Invocar el comando make y obtener el archivo de biblioteca estática (libmd5.a)
        // 2) Enlazar la librería estática (archivo de extensión .a) al proyecto
        // 2) Usar la función int MDFile(char* filename, char hashValue[33});
        // filename nombre del archivo
        // hashValue almacena el hash
        // Valor retorno: 1 o 0 si hubo un error
        char hash1[33];
        char hash2[33];
        if(MDFile(file1, hash1) && MDFile(file2, hash2)){
            printf("hash1 %s\n", hash1);
            printf("hash2 %s\n", hash2);
            if(strcmp(hash1, hash2) == 0){ // Comparar hashes
                equal = 1;
            }
        }
    }
    printf("equal %d\n", equal);
    return equal;
}
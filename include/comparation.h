#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../resources/md5-lib/libmd5.a"
//  Calcular el hash MD5 de los contenidos del archivo

int comparation(char funcMode, char* file1, char* file2){
    int equal = 0;
    if(funcMode == 'e'){ // Modo ejecutable (md5-app)
        // 1) Invocar el comando make y obtener el ejecutable
        // 2) Escribir en la linea de comandos ./md5 <nombrearchivo> para imprimir el hash MD5 del archivo <nombrearchivo>
        // 3) Crear tubería entre una invocación con fork del código MD5 y su programa
        // 4) Leer el hash de la tubería
        
        // Crear pipe
        int pipefd[2]; // 0 extremo lectura, 1 extremo escritura
        if(pipe(pipefd) == -1){
            perror("pipe");
            exit(1);
        }

        // Crear child
        int pid = fork();
        if(pid < 0){
            perror("fork");
            exit(1);
        }

        if(pid == 0){ // Child
            close(pipefd[0]); 

            dup2(pipefd[1], STDOUT_FILENO); // Redirigir consola al pipe
            execlp("md5", "md5", file1, (char *)NULL);
            perror("execlp");

            exit(1);
        }else{ // Parent
            close(pipefd[1]);
            char hash1[33], hash2[33];

            read(pipefd[0], hash1, 32);
            hash1[32] = '\0';

            read(pipefd[0], hash2, 32);
            hash2[32] = '\0';

            if(strcmp(hash1, hash2) == 0){ // Comparar hashes
                equal = 1;
            }
        }
    }else if(funcMode == 'l'){ // Modo biblioteca (md5-lib)
        // 1) Invocar el comando make y obtener el archivo de biblioteca estática (libmd5.a)
        // 2) Enlazar la librería estática (archivo de extensión .a) al proyecto
        // 2) Usar la función int MDFile(char* filename, char hashValue[33);
        // filename nombre del archivo
        // hashValue almacena el hash
        // Valor retorno: 1 o 0 si hubo un error
        char hash1[33], hash2[33];
        if(MDFile(file1, hash1) && MDFile(file2, hash2)){
            if(strcmp(hash1, hash2) == 0){ // Comparar hashes
                equal = 1;
            }
        }else{
            return equal;
        }
    }
    return equal;
}
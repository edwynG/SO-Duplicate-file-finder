# Buscador de archivos duplicados

Este programa, desarrollado en **C**, busca y detecta archivos duplicados en un sistema de archivos. Emplea algoritmos eficientes para comparar el contenido de los archivos y así identificando duplicados mediante el cálculo de hashes (como MD5). Esto permite a los usuarios liberar espacio en disco al eliminar archivos redundantes.

El proyecto está diseñado para ser modular y fácil de mantener, con una estructura clara que separa el código fuente, los encabezados, los recursos y los archivos de configuración.

## Estructura del Proyecto

La estructura del proyecto es la siguiente:

```{html}
mi_proyecto/
├── resources/          # Recursos adicionales (bibliotecas, etc.)
│   └── md5-lib/        # Biblioteca para calcular hashes MD5
│   └── md5-app/        # Ejecutable para calcular hashes MD5
├── src/                # Código fuente
│   ├── main.c          # Punto de entrada del programa
│   └── ...             # Otros módulos
├── include/            # Archivos de cabecera
│   └── ...             # Declaraciones de funciones y estructuras
├── tests/              # Archivos de pruebas
│   └── ...             # Pruebas unitarias
├── obj/                # Archivos objeto(Se genera en el build)
├── bin/                # Ejecutables(Build)
├── Makefile            # Archivo Makefile
└── README.md           # Este archivo
```

## Colaboradores

- **Edwyn Guzmán**
- **Samantha Ramirez**

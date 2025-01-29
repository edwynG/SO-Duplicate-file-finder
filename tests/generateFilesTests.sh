#!/bin/bash

# Nombre de la carpeta, por defecto 'tests'
folder_name=${1:-test}

rm -rf "$folder_name"  # Eliminar el directorio de pruebas si ya existe
# Crear el directorio principal de pruebas
mkdir -p "$folder_name"

# Contadores
total_archivos=0
total_enlaces=0
duplicados=0

# Función para crear archivos, duplicados y enlaces simbólicos
create_files() {
    local current_dir="$1"
    local num_files=$((RANDOM % 7 + 2))  # Número aleatorio de archivos (2 a 7)

    for _ in $(seq 1 $num_files); do
        # Crear un archivo con contenido único
        local filename="file_${total_archivos}.txt"
        echo "Contenido del archivo $filename" > "$current_dir/$filename"
        total_archivos=$((total_archivos + 1))

        # Decidir aleatoriamente cuántos duplicados crear
        local num_duplicates=$((RANDOM % 5))  # 0 a 4 duplicados
        for d in $(seq 1 $num_duplicates); do
            cp "$current_dir/$filename" "$current_dir/${filename%.txt}_dup$d.txt"
            duplicados=$((duplicados + 1))
        done

        # Decidir aleatoriamente si crear un enlace simbólico
        if (( RANDOM % 2 == 0 )); then
            local link_name="${filename%.txt}_link.txt"
            ln -s "$current_dir/$filename" "$current_dir/$link_name"
            total_enlaces=$((total_enlaces + 1))

            # Crear duplicados del enlace simbólico
            local num_link_duplicates=$((RANDOM % 3 + 1))  # 1 a 3 duplicados
            for d in $(seq 1 $num_link_duplicates); do
                # Copiar el enlace simbólico
                cp -P "$current_dir/$link_name" "$current_dir/${link_name%.txt}_dup$d.txt"
                total_enlaces=$((total_enlaces + 1))
            done
        fi
    done
}

# Crear archivos en el directorio especificado
create_files "$folder_name"


# Mostrar el número total de archivos y duplicados creados
total_creados=$((total_archivos + duplicados + total_enlaces))
echo "Se han creado un total de $total_creados archivos (enlaces simbólicos y incluyendo duplicados)."
echo "Se han creado un total de $duplicados duplicados."

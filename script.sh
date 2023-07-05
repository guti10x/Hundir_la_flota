#!/bin/bash

tablero1() {
    Pedir el número de filas
    echo "Ingrese el número de filas:"
    read filas

    # Pedir el número de columnas
    echo "Ingrese el número de columnas:"
    read columnas

    # Crear una matriz vacía
    matriz=()

    # Rellenar la matriz con 0 o 1
    for ((i=0; i<filas; i++))
    do
        for ((j=0; j<columnas; j++))
        do
            echo "Ingrese el valor para la posición [$i][$j] (0 agua 1 barco):"
            read valor
            matriz[$i,$j]=$valor
        done
    done

    # Guardar la matriz en un archivo de texto
    archivo="tablero_jugador1.txt"
    rm -f "$archivo"  # Eliminar el archivo si ya existe

    for ((i=0; i<filas; i++))
    do
        for ((j=0; j<columnas; j++))
        do
            echo -n "${matriz[$i,$j]} " >> "$archivo"
        done
        echo >> "$archivo"
    done
}

tablero2() {
    Pedir el número de filas
    echo "Ingrese el número de filas:"
    read filas

    # Pedir el número de columnas
    echo "Ingrese el número de columnas:"
    read columnas

    # Crear una matriz vacía
    matriz=()

    # Rellenar la matriz con 0 o 1
    for ((i=0; i<filas; i++))
    do
        for ((j=0; j<columnas; j++))
        do
            echo "Ingrese el valor para la posición [$i][$j] (0 agua 1 barco):"
            read valor
            matriz[$i,$j]=$valor
        done
    done

    # Guardar la matriz en un archivo de texto
    archivo="tablero_jugador2.txt"
    rm -f "$archivo"  # Eliminar el archivo si ya existe

    for ((i=0; i<filas; i++))
    do
        for ((j=0; j<columnas; j++))
        do
            echo -n "${matriz[$i,$j]} " >> "$archivo"
        done
        echo >> "$archivo"
    done
}

# Menú de opciones
while true; do
    echo "---- MENU ----"
    echo "1. configirar tablero jugador 1"
    echo "2. configirar tablero jugador 2"
    echo "0. Salir"
    echo "---------------"
    read -p "Ingrese una opción: " opcion

    case $opcion in
        1)
            tablero1
            ;;
        2)
            tablero2
            ;;
        0)
            echo "Saliendo..."
            break
            ;;
        *)
            echo "Opción inválida. Por favor, seleccione una opción válida."
            ;;
    esac
done

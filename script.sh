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

# Función para comprobar archivos necesarios para ejecución y compilar
compilar() {
    if [ -f "tablero_jugador1.txt" ] && [ -f "tablero_jugador2.txt" ]&& [ -f "main.cpp" ]; then
        echo "Todos los archivos necesarios existen y estan disponibles para la ejecución del codigo."

        #compilar
        local archivo_cpp="main.cpp"
        echo "Compilando el archivo $archivo_cpp..."
        g++ -o ejecutable $archivo_cpp
        echo "Compilación completada. Se generó el archivo ejecutable 'ejecutable'."
    
    else
        echo "Faltan archivos para la correcta ejecución del juego."
    fi

}

# Menú de opciones
while true; do
    echo " -----------------------------------------------------------"
    echo "|                     HUNDIR LA FLOTA                        |"
    echo " -----------------------------------------------------------"
    echo "                                                            "

    echo "1. Configirar tablero jugador 1"
    echo "2. Configirar tablero jugador 2"
    echo "3. Compilar"
    echo "4. Jugar"


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
        3)
            compilar
            ;;
        4)
            compilar
            ;;
        5)
            compilar
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

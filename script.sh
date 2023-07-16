#!/bin/bash

tablero1() {
    while true; do
    echo "Ingrese el número de filas (mínimo 5):"
    read filas

    echo "Ingrese el número de columnas (mínimo 5):"
    read columnas

    if (( filas >= 5 && columnas >= 5 )); then
       break
    fi
    done

    # Crear una matriz vacía
    matriz=()

    # Rellenar la matriz con 0 o 1
    for ((i=0; i<5; i++))
    do
        for ((j=0; j<5; j++))
        do
            echo "Ingrese el valor para la posición [$i][$j] (0 agua 1 barco):"
            read valor
            matriz[$i,$j]=$valor
        done
    done

    # Guardar la matriz en un archivo de texto
    archivo="tablero_jugador1.txt"
    rm -f "$archivo"  # Eliminar el archivo si ya existe

    for ((i=0; i<5; i++))
    do
        for ((j=0; j<5; j++))
        do
            echo -n "${matriz[$i,$j]} " >> "$archivo"
        done
        echo >> "$archivo"
    done
}

tablero2() {
    while true; do
    echo "Ingrese el número de filas (mínimo 5):"
    read filas

    echo "Ingrese el número de columnas (mínimo 5):"
    read columnas

    if (( filas >= 5 && columnas >= 5)); then
        break
    fi
    done

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

lanzar_programa() {
    #compilar
        local archivo_cpp="main.cpp"
        echo "Compilando el archivo $archivo_cpp..."
        g++ -o ejecutable $archivo_cpp
        echo "Compilación completada."
    #ejecutar programa

        if [ -f "ejecutable" ]; then
            echo "Ejecutando el programa...``"
            #gnome-terminal -- ./ejecutable
            #xterm -e ./ejecutable & 
            ./ejecutable 
        
        fi
    #verificar que existen todos los archivos finales correctamente

        # Obtener el directorio base del script actual
        directorio=$(dirname "$0")


        # Archivo a buscar
        archivo="intercambio_disparos.txt"
        archivo2=$(find "$directorio" -type f -name "batalla*.txt")

        # Verificar si el archivo existe
        if [ -f "$directorio/$archivo" ]; then
            echo "El archivo $archivo se ha creado correctamente."
        else
            echo "El archivo $archivo se ha creado correctamente."
        fi
        
        if [ -n "$archivo2" ]; then
            echo "El archivo $archivo2 se ha creado correctamente."
        else
            echo "El archivo $archivo2 se ha creado correctamente."
        fi
        
    }

monitorizar_programa() {
    echo "Al ejecutar el juego se ira mostrando los disparos realizados escritos en el archiv de intercambio de disparos"
    # Ruta del archivo a monitorear
    archivo="$(dirname "$0")/intercambio_disparos.txt"

    # Variable para almacenar el número de líneas del archivo
    num_lineas_anterior=$(wc -l < "$archivo")

    cat "$archivo"

    # Bucle infinito para monitorear el archivo
    while true; do
        # Obtener el número actual de líneas del archivo
        num_lineas_actual=$(wc -l < "$archivo")

        # Verificar si se ha añadido una nueva línea al archivo
        if [ "$num_lineas_actual" -gt "$num_lineas_anterior" ]; then
            # Mostrar la nueva línea por consola
            nueva_linea=$(tail -n 1 "$archivo")
            echo $nueva_linea
        fi

        # Actualizar el número de líneas anterior
        num_lineas_anterior="$num_lineas_actual"

        # Esperar 0.5 segundos antes de volver a verificar
        sleep 0.5
    done
}

# Exportar la función para que sea accesible desde el nuevo terminal
export -f monitorizar_programa

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
    echo "5. Monitorizar la partida (con el archivo de disparos)"

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
            lanzar_programa  
            ;;
        5)
            gnome-terminal -- bash -c "monitorizar_programa; exec bash"
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
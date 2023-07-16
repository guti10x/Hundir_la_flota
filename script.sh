#!/bin/bash

function solicitar_orientacion() {
    local orientacion
    read -p "Introduce la orientación (N/S/E/O): " orientacion
    while ! [[ "$orientacion" =~ ^[NSOEW]$ ]]; do
        read -p "Orientación inválida. Introduce la orientación (N/S/E/O) nuevamente: " orientacion
    done
    echo "$orientacion"
}

function solicitar_coordenada() {
    local coordenada
    read -p "Introduce la coordenada $1: " coordenada
    while ! [[ "$coordenada" =~ ^[0-9]+$ ]]; do
        read -p "Coordenada inválida. Introduce la coordenada $1 nuevamente: " coordenada
    done
    echo "$coordenada"
}

tablero1() {
    while true; do
        # Pedir al usuario el número de filas y columnas
        read -p "Introduce el número de filas (mínimo 5) del tablero: " filas
        read -p "Introduce el número de columnas (mínimo 5) del tablero: " columnas

        # Comprobar si las filas y columnas son mayores o iguales a 5
        if [ "$filas" -ge 5 ] && [ "$columnas" -ge 5 ]; then
            break
        else
            echo "Error: El número de filas y columnas debe ser mayor o igual a 5."
        fi
    done

    # Crear una matriz inicializada a 0
    declare -A matriz
    for ((i=0; i<filas; i++)); do
        for ((j=0; j<columnas; j++)); do
            matriz[$i,$j]=0
        done
    done

    # Mostrar la matriz por consola
    echo "Tablero generado:"
    for ((i=0; i<filas; i++)); do
        for ((j=0; j<columnas; j++)); do
            echo -n "${matriz[$i,$j]} "
        done
        echo
    done
    
    # Solicitar posicion y orientacion bombardeo
    local x_inicio=$(solicitar_coordenada "x de la bombardero dentro del tablero")
    local y_inicio=$(solicitar_coordenada "Y de la bombardero dentro del tablero")
    local orientacion=$(solicitar_orientacion "de la bbombardero dentro del tablero")
    
    # Verificar límites de las coordenadas
    if ((x_inicio < 0 || x_inicio >= filas)) || ((y_inicio < 0 || y_inicio >= columnas)); then
        echo "Error: Las coordenadas están fuera de rango del tablero."
        exit 1
    fi

    # Verificar que ambas posiciones sean 0 antes de establecerlas a 1
    if [ "${matriz[$x_inicio,$y_inicio]}" -eq 0 ]; then
        matriz["$x_inicio,$y_inicio"]=1

        # Establecer la posición contigua según la orientación
        case "$orientacion" in
            N)
                ((x_inicio--))
                ;;
            S)
                ((x_inicio++))
                ;;
            E)
                ((y_inicio++))
                ;;
            O)
                ((y_inicio--))
                ;;
        esac

        # Verificar límites de las coordenadas contiguas
        if ((x_inicio >= 0 && x_inicio < filas)) && ((y_inicio >= 0 && y_inicio < columnas)) &&
        [ "${matriz[$x_inicio,$y_inicio]}" -eq 0 ]; then
            matriz["$x_inicio,$y_inicio"]=1
        else
            echo "Error: La posición contigua está fuera de rango del tablero o no es 0."
            matriz["$x_inicio,$y_inicio"]=0  # Ajustar a 0 si es necesario
            exit 1
        fi
    else
        echo "Error: La posición original no es 0."
        exit 1
    fi

    # Llenar la matriz con valores ingresados por el usuario
    #echo "Coloca los barcos en el tablero"
    #echo "Barcos disponibles: portaviones (4 uds), bombarderos (3 uds) y fragatas (2 uds)"
    #for ((i=0; i<filas; i++)); do
    #    for ((j=0; j<columnas; j++)); do
    #        valor=""
    #        while [[ "$valor" != "0" && "$valor" != "1" ]]; do
    #            read -p "Introduce un valor (0 (agua) o 1(barco)) para la posición [$i,$j]: " valor
    #            if [[ "$valor" != "0" && "$valor" != "1" ]]; then
    #                echo "Error: Solo puedes introducir 0 o 1."
    #            fi
    #        done
    #        matriz[$i,$j]=$valor
    #    done
    #done
    
    # Mostrar la matriz por consola
    echo "Barcos colocados:"
    for ((i=0; i<filas; i++)); do
        for ((j=0; j<columnas; j++)); do
            echo -n "${matriz[$i,$j]} "
        done
        echo
    done

    # Guardar la matriz en un archivo de texto
    archivo_salida="tablero_jugador1.txt"

    # Eliminar el contenido del archivo
    > $archivo_salida

    # Agregar la matriz al archivo
    for ((i=0; i<filas; i++)); do
        for ((j=0; j<columnas; j++)); do
            echo -n "${matriz[$i,$j]} " >> $archivo_salida
        done
        echo >> $archivo_salida
    done

    echo "Matriz guardada en $archivo_salida"
}

tablero2() {
   while true; do
        # Pedir al usuario el número de filas y columnas
        read -p "Introduce el número de filas (mínimo 5) del tablero: " filas
        read -p "Introduce el número de columnas (mínimo 5) del tablero: " columnas

        # Comprobar si las filas y columnas son mayores o iguales a 5
        if [ "$filas" -ge 5 ] && [ "$columnas" -ge 5 ]; then
            break
        else
            echo "Error: El número de filas y columnas debe ser mayor o igual a 5."
        fi
    done

    # Crear una matriz inicializada a 0
    declare -A matriz
    for ((i=0; i<filas; i++)); do
        for ((j=0; j<columnas; j++)); do
            matriz[$i,$j]=0
        done
    done

    # Mostrar la matriz por consola
    echo "Tablero generado:"
    for ((i=0; i<filas; i++)); do
        for ((j=0; j<columnas; j++)); do
            echo -n "${matriz[$i,$j]} "
        done
        echo
    done
    
    # Llenar la matriz con valores ingresados por el usuario
    echo "Coloca los barcos en el tablero"
    echo "Barcos disponibles: portaviones (4 uds), bombarderos (3 uds) y fragatas (2 uds)"
    for ((i=0; i<filas; i++)); do
        for ((j=0; j<columnas; j++)); do
            valor=""
            while [[ "$valor" != "0" && "$valor" != "1" ]]; do
                read -p "Introduce un valor (0 (agua) o 1(barco)) para la posición [$i,$j]: " valor
                if [[ "$valor" != "0" && "$valor" != "1" ]]; then
                    echo "Error: Solo puedes introducir 0 o 1."
                fi
            done
            matriz[$i,$j]=$valor
        done
    done

    # Mostrar la matriz por consola
    echo "Barcos colocados:"
    for ((i=0; i<filas; i++)); do
        for ((j=0; j<columnas; j++)); do
            echo -n "${matriz[$i,$j]} "
        done
        echo
    done

    # Guardar la matriz en un archivo de texto
    archivo_salida="tablero_jugador2.txt"

    # Eliminar el contenido del archivo
    > $archivo_salida

    # Agregar la matriz al archivo
    for ((i=0; i<filas; i++)); do
        for ((j=0; j<columnas; j++)); do
            echo -n "${matriz[$i,$j]} " >> $archivo_salida
        done
        echo >> $archivo_salida
    done

    echo "Matriz guardada en $archivo_salida"
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
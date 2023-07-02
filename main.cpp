#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <random>

void cargarTablero(const std::string& archivo, std::vector<std::vector<int>>& tablero) {
    std::ifstream archivoTablero(archivo);

    if (!archivoTablero) {
        std::cout << "No se pudo abrir el archivo del tablero: " << archivo << std::endl;
        exit(1);
    }

    int valor;
    while (archivoTablero >> valor) {
        tablero.push_back(std::vector<int>());
        tablero.back().push_back(valor);
        while (archivoTablero.peek() == ' ') {
            archivoTablero.ignore();
            archivoTablero >> valor;
            tablero.back().push_back(valor);
        }
    }

    archivoTablero.close();
}

void imprimirTablero(const std::vector<std::vector<int>>& tablero) {
    for (const auto& fila : tablero) {
        
        for (int valor : fila) {
            std::cout << valor << " ";
        }
        std::cout<< std::endl;
    }
}


int main() {
    std::vector<std::vector<int>> tableroJugador1;
    std::vector<std::vector<int>> tableroJugador2;

    // Cargar tablero del jugador 1
    cargarTablero("tablero_jugador1.txt", tableroJugador1);

    // Cargar tablero del jugador 2
    cargarTablero("tablero_jugador2.txt", tableroJugador2);

    // Imprimir tableros
    std::cout << "Tablero del Jugador 1:" << std::endl;
    imprimirTablero(tableroJugador1);

    std::cout << std::endl;

    std::cout << "Tablero del Jugador 2:" << std::endl;
    imprimirTablero(tableroJugador2);

        pid_t pid1, pid2;

    // Crear el primer proceso hijo
    pid1 = fork();
    if (pid1 < 0) {
        // Error al crear el proceso hijo
        std::cerr << "Error al crear el primer proceso hijo." << std::endl;
        return 1;
    } else if (pid1 == 0) {
        // Código para el primer proceso hijo (jugador 1)
        std::cout << "Proceso hijo 1 (jugador 1)" << std::endl;

        // Generar fila y columna disparo aleatorio
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> rowDist(0, tableroJugador2.size() - 1);
        std::uniform_int_distribution<> colDist(0, tableroJugador2[0].size() - 1);

        while (true) {
            // Generar timepo de disparo aleatorio
            std::uniform_int_distribution<> waitDist(0, 10);
            int waitTime = waitDist(gen);
            sleep(waitTime);

            // Realizar disparo aleatorio
            int row = rowDist(gen);
            int col = colDist(gen);
            int target = tableroJugador2[row][col];

            // Procesar el resultado del disparo
            if (target == 1) {
                // El disparo acierta en un barco
                tableroJugador2[row][col] = 3;  // Marcar como barco hundido
                // Realizar disparos en la dirección del barco hundido (derecha, abajo, izquierda, arriba)
                // ...

            } else if (target == 0) {
                // El disparo da en agua
                // ...

            } else {
                // El disparo ya se había realizado en esa posición
                // ...
            }

            // Verificar si se hundieron todos los barcos del oponente
            bool allShipsSunk = true;
            for (const auto& row : tableroJugador2) {
                for (int cell : row) {
                    if (cell == 1) {
                        allShipsSunk = false;
                        break;
                    }
                }
                if (!allShipsSunk) {
                    break;
                }
            }

            if (allShipsSunk) {
                std::cout << "Jugador 1 ha ganado!" << std::endl;
                break;
            }
        }

        exit(0);
    }

    // Crear el segundo proceso hijo
    pid2 = fork();
    if (pid2 < 0) {
        // Error al crear el proceso hijo
        std::cerr << "Error al crear el segundo proceso hijo." << std::endl;
        return 1;
    } else if (pid2 == 0) {
        // Código para el segundo proceso hijo (jugador 2)
        std::cout << "Proceso hijo 2 (jugador 2)" << std::endl;

        // Generar fila y columna disparo
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> rowDist(0, tableroJugador1.size() - 1);
        std::uniform_int_distribution<> colDist(0, tableroJugador1[0].size() - 1);

        while (true) {
            // Generar timepo de disparo aleatorio
            std::uniform_int_distribution<> waitDist(0, 10);
            int waitTime = waitDist(gen);
            sleep(waitTime);

            // Realizar disparo aleatorio
            int row = rowDist(gen);
            int col = colDist(gen);
            int target = tableroJugador1[row][col];

            // Procesar el resultado del disparo
            if (target == 1) {
                // El disparo acierta en un barco
                tableroJugador1[row][col] = 3;  // Marcar como barco hundido
                // Realizar disparos en la dirección del barco hundido (derecha, abajo, izquierda, arriba)
                // ...

            } else if (target == 0) {
                // El disparo da en agua
                // ...

            } else {
                // El disparo ya se había realizado en esa posición
                // ...
            }

            // Verificar si se hundieron todos los barcos del oponente
            bool allShipsSunk = true;
            for (const auto& row : tableroJugador1) {
                for (int cell : row) {
                    if (cell == 1) {
                        allShipsSunk = false;
                        break;
                    }
                }
                if (!allShipsSunk) {
                    break;
                }
            }

            if (allShipsSunk) {
                std::cout << "Jugador 2 ha ganado!" << std::endl;
                break;
            }
        }

        exit(0);
    }

    // Código para el proceso padre
    // ...

    // Esperar a que ambos procesos hijos terminen
    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    return 0;
}
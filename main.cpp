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

bool todosBarcosHundidos(const std::vector<std::vector<int>>& tablero) {
    for (const auto& fila : tablero) {
        for (int celda : fila) {
            if (celda == 1) {
                return false;
            }
        }
    }
    return true;
}

int cambioRow(int direction,int lastHitRow) {
         if (direction == 1) {
                // Disparo hacia la derecha
                return lastHitRow; 
            } else if (direction == 2) {
                // Disparo hacia abajo
                return lastHitRow + 1;
                
            } else if (direction == 3) {
                // Disparo hacia la izquierda
                return lastHitRow;
                
            } else if (direction == 4) {
                // Disparo hacia arriba
                return lastHitRow - 1;
            }
}
int cambioCol (int direction,int lastHitCol) {
         if (direction == 1) {
                // Disparo hacia la derecha
                return lastHitCol + 1;
            } else if (direction == 2) {
                // Disparo hacia abajo
                return lastHitCol;
            } else if (direction == 3) {
                // Disparo hacia la izquierda
                return lastHitCol - 1;
            } else if (direction == 4) {
                // Disparo hacia arriba
                return lastHitCol;
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

        // Variables para almacenar la dirección del siguiente disparo y la posición del último disparo certero
        int direction = 0;       // Dirección del siguiente disparo (0: aleatorio, 1: derecha, 2: abajo, 3: izquierda, 4: arriba)
        int lastHitRow = -1;     // Fila del último disparo certero
        int lastHitCol = -1;     // Columna del último disparo certero
        
        while (true) {
            // Generar timepo de disparo aleatorio
            std::uniform_int_distribution<> waitDist(0, 10);
            int waitTime = waitDist(gen);
            sleep(waitTime);

            // Coordenadas del siguiente disparo
            int nextRow, nextCol;

            if (direction == 0) {
                // Disparo aleatorio
                nextRow = rowDist(gen);
                nextCol = colDist(gen);
            } else if (direction == 1) {
                // Disparo hacia la derecha
                nextRow = lastHitRow;
                nextCol = lastHitCol + 1;
                int contador= 4;
                while (tableroJugador1[nextRow][nextCol] != 1 && tableroJugador1[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Error fatal" << std::endl;
                        break;
                    }
                }  
    
            } else if (direction == 2) {
                // Disparo hacia abajo
                nextRow = lastHitRow + 1;
                nextCol = lastHitCol;
                int contador= 4;
                while (tableroJugador1[nextRow][nextCol] != 1 && tableroJugador1[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Error fatal" << std::endl;
                        break;
                    }
                } 
            } else if (direction == 3) {
                // Disparo hacia la izquierda
                nextRow = lastHitRow;
                nextCol = lastHitCol - 1;
                int contador= 4;
                while (tableroJugador1[nextRow][nextCol] != 1 && tableroJugador1[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Error fatal" << std::endl;
                        break;
                    }
                } 
            } else if (direction == 4) {
                // Disparo hacia arriba
                nextRow = lastHitRow - 1;
                nextCol = lastHitCol;
                int contador= 4;
                while (tableroJugador1[nextRow][nextCol] != 1 && tableroJugador1[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Error fatal" << std::endl;
                        break;
                    }
                } 
            }
            // Realizar disparo 
            int target = tableroJugador2[nextRow][nextCol];

            // Procesar el resultado del disparo
            if (target == 1) {
                // disparo al barco --> disparo
                tableroJugador2[nextRow][nextCol] = 3;  // Marcar como barco hundido
                
                lastHitRow = nextRow;
                lastHitCol = nextCol;











                

                // Actualizar la dirección del siguiente disparo
                direction = (direction + 1) % 4;  // Incrementar en sentido de las agujas del reloj (derecha, abajo, izquierda, arriba)

                // Verificar si se hundieron todos los barcos del oponente
                if (todosBarcosHundidos(tableroJugador2)) {
                    std::cout << "Jugador 1 ha ganado!" << std::endl;
                    break;
                }
            } else if (target == 0) {
                //  disparo al agua --> disparo aleatorio
                direction = 0;

            } else {
                // El disparo ya se había realizado en esa posición
                // ...
            }
        }
    }

    // Crear el segundo proceso hijo
    pid2 = fork();
    if (pid2 < 0) {
        // Error al crear el proceso hijo
        std::cerr << "Error al crear el segundo proceso hijo." << std::endl;
        return 1;
    } else if (pid2 == 0) {
        // Código para el primer proceso hijo (jugador 1)
        std::cout << "Proceso hijo 1 (jugador 1)" << std::endl;

        // Generar fila y columna disparo aleatorio
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> rowDist(0, tableroJugador2.size() - 1);
        std::uniform_int_distribution<> colDist(0, tableroJugador2[0].size() - 1);

        // Variables para almacenar la dirección del siguiente disparo y la posición del último disparo certero
        int direction = 0;       // Dirección del siguiente disparo (0: aleatorio, 1: derecha, 2: abajo, 3: izquierda, 4: arriba)
        int lastHitRow = -1;     // Fila del último disparo certero
        int lastHitCol = -1;     // Columna del último disparo certero
        
        while (true) {
            // Generar timepo de disparo aleatorio
            std::uniform_int_distribution<> waitDist(0, 10);
            int waitTime = waitDist(gen);
            sleep(waitTime);

            // Coordenadas del siguiente disparo
            int nextRow, nextCol;

            if (direction == 0) {
                // Disparo aleatorio
                nextRow = rowDist(gen);
                nextCol = colDist(gen);
            } else if (direction == 1) {
                // Disparo hacia la derecha
                nextRow = lastHitRow;
                nextCol = lastHitCol + 1;
                int contador= 4;
                while (tableroJugador2[nextRow][nextCol] != 1 && tableroJugador2[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Error fatal" << std::endl;
                        break;
                    }
                }  
    
            } else if (direction == 2) {
                // Disparo hacia abajo
                nextRow = lastHitRow + 1;
                nextCol = lastHitCol;
                int contador= 4;
                while (tableroJugador2[nextRow][nextCol] != 1 && tableroJugador2[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Error fatal" << std::endl;
                        break;
                    }
                } 
            } else if (direction == 3) {
                // Disparo hacia la izquierda
                nextRow = lastHitRow;
                nextCol = lastHitCol - 1;
                int contador= 4;
                while (tableroJugador2[nextRow][nextCol] != 1 && tableroJugador2[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Error fatal" << std::endl;
                        break;
                    }
                } 
            } else if (direction == 4) {
                // Disparo hacia arriba
                nextRow = lastHitRow - 1;
                nextCol = lastHitCol;
                int contador= 4;
                while (tableroJugador2[nextRow][nextCol] != 1 && tableroJugador2[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Error fatal" << std::endl;
                        break;
                    }
                } 
            }
            // Realizar disparo 
            int target = tableroJugador2[nextRow][nextCol];

            // Procesar el resultado del disparo
            if (target == 1) {
                // disparo al barco --> disparo
                tableroJugador2[nextRow][nextCol] = 3;  // Marcar como barco hundido
                
                lastHitRow = nextRow;
                lastHitCol = nextCol;











                

                // Actualizar la dirección del siguiente disparo
                direction = (direction + 1) % 4;  // Incrementar en sentido de las agujas del reloj (derecha, abajo, izquierda, arriba)

                // Verificar si se hundieron todos los barcos del oponente
                if (todosBarcosHundidos(tableroJugador2)) {
                    std::cout << "Jugador 1 ha ganado!" << std::endl;
                    break;
                }
            } else if (target == 0) {
                //  disparo al agua --> disparo aleatorio
                direction = 0;

            } else {
                // El disparo ya se había realizado en esa posición
                // ...
            }
        }
    }












    // Código para el proceso padre
    // ...

    // Esperar a que ambos procesos hijos terminen
    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    return 0;
}
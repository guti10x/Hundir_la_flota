#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <random>
#include <thread>
#include <ctime>
#include <iomanip>
#include <string>

const int ROWS = 8;
const int COLS = 8;

void cargarTablero(const std::string& archivo, int tablero[ROWS][COLS]) {
    std::ifstream archivoTablero(archivo);

    if (!archivoTablero) {
        std::cout << "No se pudo abrir el archivo del tablero: " << archivo << std::endl;
        exit(1);
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            char c;
            archivoTablero >> c;
            tablero[i][j] = c - '0';
        }
    }

    archivoTablero.close();
}

void imprimirTablero(const int tablero[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            std::cout << tablero[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

bool todosBarcosHundidos(const int tablero[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (tablero[i][j] == 1) {
                return false;
            }
        }
    }
    return true;
}

std::string fechaActual() {
    // Obtener la fecha y hora actual
    std::time_t now = std::time(nullptr);

    // Convertir la fecha y hora en una estructura tm
    std::tm* timeinfo = std::localtime(&now);

    // Obtener los componentes de la fecha y hora
    int year = timeinfo->tm_year + 1900;   // Año actual
    int month = timeinfo->tm_mon + 1;      // Mes actual
    int day = timeinfo->tm_mday;           // Día actual
    int hour = timeinfo->tm_hour;          // Hora actual
    int minute = timeinfo->tm_min;         // Minuto actual

    // Construir el string de la fecha y hora en el formato deseado
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << year << "-"
       << std::setw(2) << std::setfill('0') << month << "-"
       << std::setw(2) << std::setfill('0') << day << " "
       << std::setw(2) << std::setfill('0') << hour << ":"
       << std::setw(2) << std::setfill('0') << minute;

    // Devolver el string de la fecha y hora
    return ss.str();
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

 void jugador1(){
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Esperar antes de empezar a ejecutarse para comprobaciones del main

    int tableroJugador2[ROWS][COLS];

    // Cargar tablero del jugador 2
    cargarTablero("tablero_jugador2.txt", tableroJugador2);

    // Generar fila y columna disparo aleatorio
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> rowDist(0, 8);
        std::uniform_int_distribution<> colDist(0, 8);

        // Variables para almacenar la dirección del siguiente disparo y la posición del último disparo certero
        int direction = 0;       // Dirección del siguiente disparo (0: aleatorio, 1: derecha, 2: abajo, 3: izquierda, 4: arriba)
        int lastHitRow = -1;     // Fila del último disparo certero
        int lastHitCol = -1;     // Columna del último disparo certero
        
        bool estadoJuego = true;
        while (estadoJuego= true ){
            // Generar timepo de disparo aleatorio
            std::uniform_int_distribution<> waitDist(0, 10);
            int waitTime = waitDist(gen);
            std::cout << "Jugador1 - tiempo para sigueinte disparo: " <<waitTime<< std::endl;
            sleep(waitTime);
            
            // Coordenadas del siguiente disparo
            int nextRow, nextCol;

            if (direction == 0) {
                // Disparo aleatorio
                nextRow = rowDist(gen);
                nextCol = colDist(gen);
                std::cout << "Jugador1 - disparo aleatorio con direción [" <<nextRow<<" "<<nextCol<<"]"<<std::endl;
                
            } else if (direction == 1) {
                // Disparo hacia la derecha
                nextRow = lastHitRow;
                nextCol = lastHitCol + 1;
                int contador= 4;
                while (tableroJugador2[nextRow][nextCol] != 1 && tableroJugador2[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Jugador1 - error fatal" << std::endl;
                        break;
                    }
                }  
                std::cout << "Jugador1 - disparo con direción [" <<nextRow<<" "<<nextCol<<"]"<<std::endl;
    
            } else if (direction == 2) {
                // Disparo hacia abajo
                nextRow = lastHitRow + 1;
                nextCol = lastHitCol;
                int contador= 4;
                while (tableroJugador2[nextRow][nextCol] != 1 && tableroJugador2[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Jugador1 - error fatal" << std::endl;
                        break;
                    }
                } 
                std::cout << "Jugador1 - disparo con direción [" <<nextRow<<" "<<nextCol<<"]"<<std::endl;
            } else if (direction == 3) {
                // Disparo hacia la izquierda
                nextRow = lastHitRow;
                nextCol = lastHitCol - 1;
                int contador= 4;
                while (tableroJugador2[nextRow][nextCol] != 1 && tableroJugador2[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Jugador1 - error fatal" << std::endl;
                        break;
                    }
                }
                std::cout << "Jugador1 - disparo con direción [" <<nextRow<<" "<<nextCol<<"]"<<std::endl;
            } else if (direction == 4) {
                // Disparo hacia arriba
                nextRow = lastHitRow - 1;
                nextCol = lastHitCol;
                int contador= 4;
                while (tableroJugador2[nextRow][nextCol] != 1 && tableroJugador2[nextRow][nextCol] != 0){  
                   nextRow=cambioCol(direction,nextRow);
                   nextCol=cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Jugador1 - error fatal" << std::endl;
                        break;
                    }
                } 
                std::cout << "Jugador1 - disparo con direción [" <<nextRow<<" "<<nextCol<<"]"<<std::endl;
            }

                 // Realizar disparo                       
            int target = tableroJugador2[nextRow][nextCol];
            
            std::cout << "Jugador1 - disparando a [" << nextRow << ", " << nextCol << "]" << std::endl;

            // Procesar el resultado del disparo
            if (target == 1) {
                // Disparo al barco --> disparo
                tableroJugador2[nextRow][nextCol] = 3;  // Marcar como barco hundido
                std::cout << "Jugador1 - ¡¡¡¡¡IMPACTO A BARCO ENEMIGO!!!!!!! [" << nextRow << ", " << nextCol << "]" << std::endl;

                //reajustamos variables ultimo disparo para hacer el siguiente disparo
                lastHitRow = nextRow;
                lastHitCol = nextCol;

                // Obtener el PID del hilo
                pid_t pid = getpid();

                std::ostringstream oss;
                oss << pid << ":" << nextRow << "," << nextCol << ":";
                std::string texto = oss.str();

                std::ofstream archivo("intercambio_disparos.txt");
                
                if (archivo.is_open()) {
                    //archivo << texto;
                    archivo.close();
                    std::cout << "Jugador1 - registro de impacto guardado" << std::endl;
                } else {
                    std::cout << "Jugador1 - registro de impacto NO guardado al no poder abrir el archivo" << std::endl;
                }
                
                // Actualizar la dirección del siguiente disparo
                direction = (direction + 1) % 4;  // Incrementar en sentido de las agujas del reloj (derecha, abajo, izquierda, arriba)

                // Verificar si se hundieron todos los barcos del oponente
                if (todosBarcosHundidos(tableroJugador2)) {
                    std::cout << "Jugador 1 ha ganado!" << std::endl;
                    estadoJuego = false;
                }
            } else if (target == 0) {
                //  Disparo al agua --> disparo aleatorio
                direction = 0;
                std::cout << "Jugador1 - disparo al agua!" << std::endl;
            } else if(target == 3) {
                //  Disparo al agua --> disparo aleatorio
                direction = 0;
                std::cout << "Jugador2 - disparo al agua!" << std::endl;
            }   
        }
 }

 void jugador2() {
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Esperar antes de empezar a ejecutarse para comprobaciones del main

    int tableroJugador1[ROWS][COLS];

    // Cargar tablero del jugador 2
    cargarTablero("tablero_jugador1.txt", tableroJugador1);

    // Generar fila y columna disparo aleatorio
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> rowDist(0, 8);
        std::uniform_int_distribution<> colDist(0, 8);

        // Variables para almacenar la dirección del siguiente disparo y la posición del último disparo certero
        int direction = 0;       // Dirección del siguiente disparo (0: aleatorio, 1: derecha, 2: abajo, 3: izquierda, 4: arriba)
        int lastHitRow = -1;     // Fila del último disparo certero
        int lastHitCol = -1;     // Columna del último disparo certero
        
        bool estadoJuego = true;
        while (estadoJuego= true ){
            // Generar timepo de disparo aleatorio
            std::uniform_int_distribution<> waitDist(0, 10);
            int waitTime = waitDist(gen);
            std::cout << "Jugador2 - tiempo para sigueinte disparo: " <<waitTime<< std::endl;
            sleep(waitTime);
            
            // Coordenadas del siguiente disparo
            int nextRow, nextCol;

            if (direction == 0) {
                // Disparo aleatorio
                nextRow = rowDist(gen);
                nextCol = colDist(gen);
                std::cout << "Jugador2 - disparo aleatorio con direción [" <<nextRow<<" "<<nextCol<<"]"<<std::endl;
                
            } else if (direction == 1) {
                // Disparo hacia la derecha
                nextRow = lastHitRow;
                nextCol = lastHitCol + 1;
                int contador= 4;
                while (tableroJugador1[nextRow][nextCol] != 1 && tableroJugador1[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Jugador2 - error fatal" << std::endl;
                        break;
                    }
                }  
                std::cout << "Jugador2 - disparo con direción [" <<nextRow<<" "<<nextCol<<"]"<<std::endl;
    
            } else if (direction == 2) {
                // Disparo hacia abajo
                nextRow = lastHitRow + 1;
                nextCol = lastHitCol;
                int contador= 4;
                while (tableroJugador1[nextRow][nextCol] != 1 && tableroJugador1[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Jugador2 - error fatal" << std::endl;
                        break;
                    }
                } 
                std::cout << "Jugador2 - disparo con direción [" <<nextRow<<" "<<nextCol<<"]"<<std::endl;
            } else if (direction == 3) {
                // Disparo hacia la izquierda
                nextRow = lastHitRow;
                nextCol = lastHitCol - 1;
                int contador= 4;
                while (tableroJugador1[nextRow][nextCol] != 1 && tableroJugador1[nextRow][nextCol] != 0){  
                   cambioCol(direction,nextRow);
                   cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Jugador2 - error fatal" << std::endl;
                        break;
                    }
                }
                std::cout << "Jugador2 - disparo con direción [" <<nextRow<<" "<<nextCol<<"]"<<std::endl;
            } else if (direction == 4) {
                // Disparo hacia arriba
                nextRow = lastHitRow - 1;
                nextCol = lastHitCol;
                int contador= 4;
                while (tableroJugador1[nextRow][nextCol] != 1 && tableroJugador1[nextRow][nextCol] != 0){  
                   nextRow=cambioCol(direction,nextRow);
                   nextCol=cambioCol(direction,nextCol);
                    if (contador>4){
                        std::cout << "Jugador2 - error fatal" << std::endl;
                        break;
                    }
                } 
                std::cout << "Jugador2 - disparo con direción [" <<nextRow<<" "<<nextCol<<"]"<<std::endl;
            }

            // Realizar disparo                       
            int target = tableroJugador1[nextRow][nextCol];
            
            std::cout << "Jugador2 - disparando a [" << nextRow << ", " << nextCol << "]" << std::endl;

            // Procesar el resultado del disparo
            if (target == 1) {
                // Disparo al barco --> disparo
                tableroJugador1[nextRow][nextCol] = 3;  // Marcar como barco hundido
                std::cout << "Jugador2 - ¡¡¡¡¡IMPACTO A BARCO ENEMIGO!!!!! [" << nextRow << ", " << nextCol << "]" << std::endl;

                //reajustamos variables ultimo disparo para hacer el siguiente disparo
                lastHitRow = nextRow;
                lastHitCol = nextCol;

                // Obtener el PID del hilo
                pid_t pid = getpid();

                std::ostringstream oss;
                oss << pid << ":" << nextRow << "," << nextCol << ":";
                std::string texto = oss.str();

                std::ofstream archivo("intercambio_disparos.txt");
                if (archivo.is_open()) {
                    archivo << texto;
                    archivo.close();
                    std::cout << "Jugador2 - registro de impacto guardado" << std::endl;
                } else {
                    std::cout << "Jugador2 - registro de impacto NO guardado al no poder abrir el archivo" << std::endl;
                }

                // Actualizar la dirección del siguiente disparo
                direction = (direction + 1) % 4;  // Incrementar en sentido de las agujas del reloj (derecha, abajo, izquierda, arriba)

                // Verificar si se hundieron todos los barcos del oponente
                if (todosBarcosHundidos(tableroJugador1)) {
                    std::cout << "Jugador 2 ha ganado!" << std::endl;
                    estadoJuego = false;
                }
            } else if(target == 0) {
                //  Disparo al agua --> disparo aleatorio
                direction = 0;
                std::cout << "Jugador2 - disparo al agua!" << std::endl;
            } else if(target == 3) {
                //  Disparo al agua --> disparo aleatorio
                direction = 0;
                std::cout << "Jugador2 - disparo al agua!" << std::endl;
            }   
        }
 }
int main() {
    std::cout << " -------------------------------------------"<<std::endl;
    std::cout << "|             HUNDIR LA FLOTA              |"<<std::endl;
    std::cout << " -------------------------------------------"<<std::endl;

    int tableroJugador1[ROWS][COLS];
    int tableroJugador2[ROWS][COLS];

    // Cargar tablero del jugador 1
    cargarTablero("tablero_jugador1.txt", tableroJugador1);

    // Cargar tablero del jugador 2
    cargarTablero("tablero_jugador2.txt", tableroJugador2);

    // Mostrar tablero del jugador 1
    std::cout << "Tablero del jugador 1:" << std::endl;
    std::cout << std::endl;
    imprimirTablero(tableroJugador1);
    std::cout<< std::endl;
    std::cout << std::endl;

    // Mostrar tablero del jugador 2
    std::cout << "Tablero del jugador 2:" << std::endl;
    std::cout << std::endl;
    imprimirTablero(tableroJugador2);
    std::cout<< std::endl;
    std::cout << std::endl;

    std::cout << " -------------------------------------------"<<std::endl;
    std::cout << "|         Que comience el juego             |"<<std::endl;
    std::cout << " -------------------------------------------"<<std::endl;

    // Crear los hilos y lanzar las funciones
    std::thread hilo1(jugador1);
    std::thread hilo2(jugador2);

    // Obtener el PID del hilo
    std::thread::id threadId1 = hilo1.get_id();
    std::thread::id threadId2 = hilo2.get_id();

    // Verificar si los hilos se han creado correctamente
    if (hilo1.joinable()) {
        std::cout << "Hilo 1 con PID " << threadId1<<" creado correctamente." << std::endl;
    } else {
        std::cout << "Error al crear el hilo 1." << std::endl;
    }
    if (hilo2.joinable()) {
        std::cout << "Hilo 2 con PID " << threadId1<<" creado correctamente." << std::endl;
    } else {
        std::cout << "Error al crear el hilo 2." << std::endl;
    }

    std::string fecha = fechaActual();
    std::cout << "Fecha y hora actual: " << fecha << std::endl;
    
    // Esperar a que los hilos terminen su ejecución
    hilo1.join();
    hilo2.join();

    // Finalizar el proceso padre
    std::cout << "Fin del programa" << std::endl;

    return 0;

    /* LANZAR HIJOS CON PROCESOS
    pid_t pid1, pid2;

    // Crear el primer proceso hijo
    pid1 = fork();
    if (pid1 < 0) {
        // Error al crear el proceso hijo
        std::cerr << "Error al crear el primer proceso hijo" << std::endl;
        return 1;
    } else if (pid1 == 0) {
        // Código para el primer proceso hijo (jugador 1)
        jugador1();
        return 0;
    }

   // Crear el segundo proceso hijo
    pid2 = fork();
    if (pid2 < 0) {
        // Error al crear el segundo proceso hijo
        std::cerr << "Error al crear el segundo proceso hijo" << std::endl;
        return 1;
    } else if (pid2 == 0) {
        // Código para el segundo proceso hijo (jugador 2)
        jugador2();
        return 0;
    }

    // Esperar a que los procesos hijos terminen
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
    */
}
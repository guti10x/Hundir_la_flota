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
#include <mutex>
#include <condition_variable>
#include <vector>

int contarFilas() {
    std::ifstream archivo("tablero_jugador1.txt");
    std::string linea;
    int numFilas = 0;

    while (std::getline(archivo, linea)) {
        numFilas++;
    }

    archivo.close();

    return numFilas;
}

int contarColumnas() {
    std::ifstream archivo("tablero_jugador1.txt");
    std::string linea;
    int numColumnas = 0;

    if (std::getline(archivo, linea)) {
        numColumnas = linea.length();
    }

    archivo.close();

    return numColumnas;
}

std::vector<std::vector<int>> tableroJugador1;
std::vector<std::vector<int>> tableroJugador2;

int tableroJugadorA1[5][5];
int tableroJugadorA2[5][5];

void inicializarMatriz() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            tableroJugadorA1[i][j] = tableroJugador1[i][j];
            tableroJugadorA2[i][j] = tableroJugador2[i][j];
        }
    }
}
std::string rutaTablero1="tablero_jugador1.txt";
std::string rutaTablero2="tablero_jugador2.txt";
std::string rutaLogIntercambioDisparos="intercambio_disparos.txt";

std::mutex smf;
std::condition_variable cv;
bool isWriting = false;
// Declaración del semáforo
std::mutex semaphore; 

void cargarTablero1() {
    std::ifstream archivo(rutaTablero1);
    if (archivo.is_open()) {
        std::string linea;
        while (getline(archivo, linea)) {
            std::vector<int> fila;
            for (char c : linea) {
                if (c == '0' || c == '1') {
                    fila.push_back(c - '0');
                }
            }
            tableroJugador1.push_back(fila);
        }
        archivo.close();
    } else {
        std::cout << "No se pudo abrir el archivo de tableroJugador1." << std::endl;
    }
}

void imprimirTablero1() {
    for (const auto& fila : tableroJugadorA1) {
        for (int elemento : fila) {
            std::cout << elemento << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void cargarTablero2() {
    std::ifstream archivo(rutaTablero2);
    if (archivo.is_open()) {
        std::string linea;
        while (getline(archivo, linea)) {
            std::vector<int> fila;
            for (char c : linea) {
                if (c == '0' || c == '1') {
                    fila.push_back(c - '0');
                }
            }
            tableroJugador2.push_back(fila);
        }
        archivo.close();
    } else {
        std::cout << "No se pudo abrir el archivo de tableroJugador2." << std::endl;
    }
}

void imprimirTablero2() {
    for (const auto& fila : tableroJugadorA2) {
        for (int elemento : fila) {
            std::cout << elemento << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool todosBarcosHundidos1() {
    for (const auto& fila : tableroJugadorA2) {
        for (int elemento : fila) {
            if (elemento == 1) {
                return false;
            }
        }
    }
    return true;
}

bool todosBarcosHundidos2() {
    for (const auto& fila : tableroJugadorA2) {
        for (int elemento : fila) {
            if (elemento == 1) {
                return false;
            }
        }
    }
    return true;
}

void comprobarWin() {
    std::fstream archivo(rutaLogIntercambioDisparos, std::ios::in | std::ios::out);
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo." << std::endl;
        return;
    }

    std::string linea;
    std::streampos posicion;

    while (std::getline(archivo, linea)) {
        if (linea.find("OVER") != std::string::npos) {
            posicion = archivo.tellg();
        }
    }

    // Obtener el PID del hilo
    std::thread::id thread_id = std::this_thread::get_id();

    // Adquirir el cerrojo del mutex
    std::unique_lock<std::mutex> lock(smf);  

    // Esperar hasta que no haya escritura en curso
    cv.wait(lock, [] { return !isWriting; });

    isWriting = true;  // Indicar que se está realizando una escritura
             
    // Realizar la escritura en el archivo
    std::ostringstream oss;
    oss << thread_id << " :WIN";
    std::string texto = oss.str();

    std::ofstream archivo2("intercambio_disparos.txt", std::ios::app); // Abrir en modo de anexado (append)

    if (archivo2.is_open()) {
        archivo2 << texto << "\n"; // Agregar nueva línea al final
        archivo2.close();
        std::cout << "Jugador1 - registro de fin de juego guardado" << std::endl;
    } else {
        std::cout << "Jugador1 - registro de fin de juego NO guardado al no poder abrir el archivo" << std::endl;
    }

    // Indicar que la escritura ha finalizado
    isWriting = false;  

    // Notificar a todas las hebras que la escritura ha terminado
    cv.notify_all();

    // Forzar finalizaci'on del hilo
    std::terminate();
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

void copiarArchivo(const std::string& archivoOriginal,  const std::thread::id& pidHijo1,  const std::thread::id& pidHijo2) {
    std::ifstream input(archivoOriginal);
    if (!input) {
        std::cerr << "No se pudo abrir el archivo original." << std::endl;
        return;
    }

    std::stringstream ss;
    ss << "batalla " << pidHijo1 << " vs " << pidHijo2 << "." << fechaActual() << ".txt";
    std::string archivoCopia = ss.str();

    std::ofstream output(archivoCopia);
    if (!output) {
        std::cerr << "No se pudo crear el archivo de copia." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(input, linea)) {
        output << linea << std::endl;
    }

    input.close();
    output.close();

    std::cout << "Se ha creado una copia del archivo con el nombre: " << archivoCopia << std::endl;
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
    return 0;
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
    return 0;
}

 void jugador1(){
    std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Esperar antes de empezar a ejecutarse para comprobaciones del main

    // Obtener el número de filas del vector
    int rows=tableroJugador1.size();
    // Obtener el número de columnas del vector (asumiendo que todas las filas tienen la misma cantidad de columnas)
    int cols= tableroJugador1[0].size(); 

    // Generar fila y columna disparo aleatorio
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDist(0, rows);
    std::uniform_int_distribution<> colDist(0, cols);

    // Variables para almacenar la dirección del siguiente disparo y la posición del último disparo certero
    int direction = 0;       // Dirección del siguiente disparo (0: aleatorio, 1: derecha, 2: abajo, 3: izquierda, 4: arriba)
    int lastHitRow = -1;     // Fila del último disparo certero
    int lastHitCol = -1;     // Columna del último disparo certero
        
    bool estadoJuego = true;
    std::string tipoImpacto = "";
    while (estadoJuego= true ){
        // Generar timepo de disparo aleatorio
        std::uniform_int_distribution<> waitDist(0,10);
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
            while (rows < nextRow &&  cols < nextCol){ 
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
            while (rows < nextRow &&  cols < nextCol){   
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
            while (rows < nextRow &&  cols < nextCol){   
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
            while (rows < nextRow &&  cols < nextCol){ 
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
        //std::cout << nextRow<<nextCol<<" "<<rows<<cols<<std::endl; 
        //imprimirTablero2();              
        int target = tableroJugadorA2[nextRow][nextCol];
        //std::cout << "contenido-  "<<target << std::endl; 
            
        std::cout << "Jugador1 - disparando a [" << nextRow << ", " << nextCol << "]" << std::endl;
        // Procesar el resultado del disparo
        if (target == 1) {
            // Disparo al barco --> disparo
              if (tableroJugadorA1[nextRow][nextCol+1]!=1 && tableroJugadorA1[nextRow][nextCol-1]!=1 
                    && tableroJugadorA1[nextRow+1][nextCol]!=1 && tableroJugadorA1[nextRow-1][nextCol]!=1){

                    /* Para barcos en diagonal
                    && tableroJugadorA1[nextRow+1][nextCol+1]!=1 && tableroJugadorA1[nextRow+1][nextCol-1]!=1
                    && tableroJugadorA1[nextRow-1][nextCol+1]!=1 && tableroJugadorA1[nextRow-1][nextCol-1]!=1)
                    */
                    tipoImpacto= "HUNDIDO";
                    std::cout << "Jugador1 - ¡¡¡¡¡ BARCO ENEMIGO HUNDIDO!!!!! [" << nextRow << ", " << nextCol << "]" << std::endl;
            }else{
                    tipoImpacto= "TOCADO";
                    std::cout << "Jugador1 - ¡¡¡¡¡IMPACTO A BARCO ENEMIGO!!!!! [" << nextRow << ", " << nextCol << "]" << std::endl;
            }
            tableroJugador2[nextRow][nextCol] = 3;  // Marcar como barco hundido
            imprimirTablero2();

             //reajustamos variables ultimo disparo para hacer el siguiente disparo
            lastHitRow = nextRow;
            lastHitCol = nextCol;

            // Obtener el PID del hilo
            std::thread::id thread_id = std::this_thread::get_id();

            // Adquirir el cerrojo del mutex
            std::unique_lock<std::mutex> lock(smf);  

            // Esperar hasta que no haya escritura en curso
            cv.wait(lock, [] { return !isWriting; });

            isWriting = true;  // Indicar que se está realizando una escritura

            // Realizar la escritura en el archivo
            std::ostringstream oss;
            oss << thread_id << ":" << nextRow << "," << nextCol<< ":"<<tipoImpacto;
            std::string texto = oss.str();

            std::ofstream archivo("intercambio_disparos.txt", std::ios::app); // Abrir en modo de anexado (append)

            if (archivo.is_open()) {
                archivo << texto << "\n"; // Agregar nueva línea al final
                archivo.close();
                std::cout << "Jugador1 - registro de impacto guardado" << std::endl;
            } else {
                std::cout << "Jugador1 - registro de impacto NO guardado al no poder abrir el archivo" << std::endl;
            }

            // Indicar que la escritura ha finalizado
            isWriting = false;  

            // Notificar a todas las hebras que la escritura ha terminado
            cv.notify_all(); 
            
            // Actualizar la dirección del siguiente disparo
            direction = (direction + 1) % 4;  // Incrementar en sentido de las agujas del reloj (derecha, abajo, izquierda, arriba)

            // Verificar si se hundieron todos los barcos del oponente
            if (todosBarcosHundidos1()) {
                std::cout << "!!!!!Jugador 1 ha hundido todos los barcos del enemigo!!!!!" << std::endl;
                estadoJuego = false;

                // Obtener el PID del hilo
                std::thread::id thread_id = std::this_thread::get_id();

                // Adquirir el cerrojo del mutex
                std::unique_lock<std::mutex> lock(smf);  

                // Esperar hasta que no haya escritura en curso
                cv.wait(lock, [] { return !isWriting; });

                isWriting = true;  // Indicar que se está realizando una escritura
             
                // Realizar la escritura en el archivo
                std::ostringstream oss;
                oss << thread_id << " :GAME OVER";
                std::string texto = oss.str();

                std::ofstream archivo("intercambio_disparos.txt", std::ios::app); // Abrir en modo de anexado (append)

                if (archivo.is_open()) {
                    archivo << texto << "\n"; // Agregar nueva línea al final
                    archivo.close();
                    std::cout << "Jugador1 - registro de fin de juego guardado" << std::endl;
                } else {
                    std::cout << "Jugador1 - registro de fin de juego NO guardado al no poder abrir el archivo" << std::endl;
                }
                // Indicar que la escritura ha finalizado
                isWriting = false;  

                // Notificar a todas las hebras que la escritura ha terminado
                cv.notify_all();

                // Forzar finalizaci'on del hilo
                std::terminate();
            }else{
                // Espera 2 segundos antes del siguiente disparo
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
                
        } else if(target == 0) {
            // Disparo al agua --> disparo aleatorio
            direction = 0;
            tipoImpacto= "HUNDIDO";
            std::cout << "Jugador1 - disparo al agua!" << std::endl;
        } else if(target == 3) {
            // Disparo al agua --> disparo aleatorio
            direction = 0;
            std::cout << "Jugador2 - disparo al agua!" << std::endl;
        }   
    }
}

 void jugador2() {
    std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Esperar antes de empezar a ejecutarse para comprobaciones del main

    // Obtener el número de filas del vector
    int rows=tableroJugador1.size();
    // Obtener el número de columnas del vector (asumiendo que todas las filas tienen la misma cantidad de columnas)
    int cols= tableroJugador1[0].size(); 

    // Generar fila y columna disparo aleatorio
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> rowDist(0, rows);
        std::uniform_int_distribution<> colDist(0, cols);

        // Variables para almacenar la dirección del siguiente disparo y la posición del último disparo certero
        int direction = 0;       // Dirección del siguiente disparo (0: aleatorio, 1: derecha, 2: abajo, 3: izquierda, 4: arriba)
        int lastHitRow = -1;     // Fila del último disparo certero
        int lastHitCol = -1;     // Columna del último disparo certero
        
        bool estadoJuego = true;
        std::string tipoImpacto = "";
        while (estadoJuego= true ){
            // Generar timepo de disparo aleatorio
            std::uniform_int_distribution<> waitDist(0,10);
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
                while (rows < nextRow &&  cols < nextCol){ 
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
                while (rows < nextRow &&  cols < nextCol){ 
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
                while (rows < nextRow &&  cols < nextCol){  
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
                while (rows < nextRow &&  cols < nextCol){ 
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
            //std::cout << nextRow<<nextCol<<" "<<rows<<cols<<std::endl;  
            //imprimirTablero1();                
            int target = tableroJugadorA1[nextRow][nextCol];
            //std::cout << "contenido-  "<<target << std::endl; 
            
            std::cout << "Jugador2 - disparando a [" << nextRow << ", " << nextCol << "]" << std::endl;

            // Procesar el resultado del disparo
            if (target == 1) {
                // Disparo al barco --> disparo

                std::cout <<tableroJugadorA1[nextRow][nextCol+1]<< std::endl;
                
                if (tableroJugadorA1[nextRow][nextCol+1]!=1 && tableroJugadorA1[nextRow][nextCol-1]!=1 
                    && tableroJugadorA1[nextRow+1][nextCol]!=1 && tableroJugadorA1[nextRow-1][nextCol]!=1){

                    /* Para barcos en diagonal
                    && tableroJugadorA1[nextRow+1][nextCol+1]!=1 && tableroJugadorA1[nextRow+1][nextCol-1]!=1
                    && tableroJugadorA1[nextRow-1][nextCol+1]!=1 && tableroJugadorA1[nextRow-1][nextCol-1]!=1)
                    */
                
                    tipoImpacto= "HUNDIDO";
                    std::cout << "Jugador2 - ¡¡¡¡¡ BARCO ENEMIGO HUNDIDO!!!!! [" << nextRow << ", " << nextCol << "]" << std::endl;
                }else{
                    tipoImpacto= "TOCADO";
                    std::cout << "Jugador2 - ¡¡¡¡¡IMPACTO A BARCO ENEMIGO!!!!! [" << nextRow << ", " << nextCol << "]" << std::endl;
                }
                    
                tableroJugadorA1[nextRow][nextCol] = 3;  // Marcar como barco hundido
                imprimirTablero1();

                //reajustamos variables ultimo disparo para hacer el siguiente disparo
                lastHitRow = nextRow;
                lastHitCol = nextCol;

                // Obtener el PID del hilo
                std::thread::id thread_id = std::this_thread::get_id();

                // Adquirir el cerrojo del mutex
                std::unique_lock<std::mutex> lock(smf);  

                // Esperar hasta que no haya escritura en curso
                cv.wait(lock, [] { return !isWriting; });

                isWriting = true;  // Indicar que se está realizando una escritura
             
                // Realizar la escritura en el archivo
                std::ostringstream oss;
                oss << thread_id << ":" << nextRow << "," << nextCol << ":"<<tipoImpacto;
                std::string texto = oss.str();

                std::ofstream archivo("intercambio_disparos.txt", std::ios::app); // Abrir en modo de anexado (append)

                if (archivo.is_open()) {
                    archivo << texto << "\n"; // Agregar nueva línea al final
                    archivo.close();
                    std::cout << "Jugador1 - registro de impacto guardado" << std::endl;
                } else {
                    std::cout << "Jugador1 - registro de impacto NO guardado al no poder abrir el archivo" << std::endl;
                }

                // Indicar que la escritura ha finalizado
                isWriting = false;  

                // Notificar a todas las hebras que la escritura ha terminado
                cv.notify_all();  

                // Actualizar la dirección del siguiente disparo
                direction = (direction + 1) % 4;  // Incrementar en sentido de las agujas del reloj (derecha, abajo, izquierda, arriba)

                // Verificar si se hundieron todos los barcos del oponente
                if (todosBarcosHundidos2()) {
                    std::cout << "!!!!!Jugador 2 ha hundido todos los barcos del enemigo!!!!!" << std::endl;
                    estadoJuego = false;
                    
                    // Obtener el PID del hilo
                    std::thread::id thread_id = std::this_thread::get_id();

                    // Adquirir el cerrojo del mutex
                    //std::unique_lock<std::mutex> lock(smf);  

                    // Esperar hasta que no haya escritura en curso
                    //cv.wait(lock, [] { return !isWriting; });
                    
                    isWriting = true;  // Indicar que se está realizando una escritura
               
                    // Realizar la escritura en el archivo
                    std::ostringstream oss;
                    oss << thread_id << " : GAME OVER";
                    std::string texto = oss.str();

                    std::ofstream archivo("intercambio_disparos.txt", std::ios::app); // Abrir en modo de anexado (append)

                    if (archivo.is_open()) {
                        archivo << texto << "\n"; // Agregar nueva línea al final
                        archivo.close();
                        std::cout << "Jugador1 - registro de fin de juego guardado" << std::endl;
                    } else {
                        std::cout << "Jugador1 - registro de fin de juego NO guardado al no poder abrir el archivo" << std::endl;
                    }

                    // Indicar que la escritura ha finalizado
                    isWriting = false;  

                    // Notificar a todas las hebras que la escritura ha terminado
                    cv.notify_all();  

                    // Forzar finalizaci'on del hilo
                    std::terminate();
                }else{
                    // Espera 2 segundos antes del siguiente disparo
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                }
        
            } else if(target == 0) {
                // Disparo al agua --> disparo aleatorio
                direction = 0;
                tipoImpacto= "AGUA"; 
                std::cout << "Jugador2 - disparo al agua!" << std::endl;
            } else if(target == 3) {
                // Disparo al agua --> disparo aleatorio
                direction = 0;
                std::cout << "Jugador2 - disparo al agua!" << std::endl;
            }   
        }
 }


int main() {
    std::cout << " -----------------------------------------------------------"<<std::endl;
    std::cout << "|                     HUNDIR LA FLOTA                        |"<<std::endl;
    std::cout << " -----------------------------------------------------------"<<std::endl;
    std::cout << std::endl;
   
    int cols=contarColumnas();
    int rows=contarFilas();
    //std::cout << cols<<std::endl;
    //std::cout << rows<<std::endl;

    // Cargar tablero del jugador 1
    cargarTablero1();

    // Cargar tablero del jugador 2
    cargarTablero2();

    inicializarMatriz();

    // Mostrar tablero del jugador 1
    std::cout << "Tablero del jugador 1:" << std::endl;
    std::cout << std::endl;
    imprimirTablero1();
    std::cout<< std::endl;
    std::cout << std::endl;

    // Mostrar tablero del jugador 2
    std::cout << "Tablero del jugador 2:" << std::endl;
    std::cout << std::endl;
    imprimirTablero2();
    std::cout<< std::endl;
    std::cout << std::endl;

    std::cout << " -----------------------------------------------------------"<<std::endl;
    std::cout << "|                  Que comience el juego                    |"<<std::endl;
    std::cout << " -----------------------------------------------------------"<<std::endl;
    std::cout << std::endl;

    // Creamos archivo donde se almacenarán los disparos exitosos de ambos jugadores realizados
    std::remove("intercambio_disparos.txt");
    std::ofstream archivo("intercambio_disparos.txt"); // Crear el archivo
    std::string log_intercambio_disparos = "intercambio_disparos.txt";

    // Crear los hilos y lanzar las funciones
    std::thread hilo1(jugador1);
    std::thread hilo2(jugador2);

    // Obtener el PID del hilo
    std::thread::id threadId1 = hilo1.get_id();
    std::thread::id threadId2 = hilo2.get_id();

    // Verificar si los hilos se han creado correctamente
    if (hilo1.joinable()) {
        std::cout << "Hilo 1 con PID " << threadId1<<"c reado correctamente." << std::endl;
    } else {
        std::cout << "Error al crear el hilo 1." << std::endl;
    }
    if (hilo2.joinable()) {
        std::cout << "Hilo 2 con PID " << threadId2 <<" creado correctamente." << std::endl;
    } else {
        std::cout << "Error al crear el hilo 2." << std::endl;
    }

    // Esperar a que los hilos terminen su ejecución
    hilo1.join();
    hilo2.join();

    //Se realiza copia del archivo de los impactos realizados por ambos jugador
    copiarArchivo(log_intercambio_disparos, threadId2, threadId2);

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
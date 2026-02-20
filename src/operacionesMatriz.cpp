#include "operacionesMatriz.h"

vector<vector<char>> Visitados(filas, vector<char>(columnas, false));

void mostrar(const vector<vector<char>>& tablero) {
    for (const auto& fila : tablero) {
        for (const auto& celda : fila) {
            cout << celda;
        }
        cout << endl;
    }
}

void cargarTablero(vector<vector<char>> &tablero){
    map<int, string> Tableros;
    vector<string> salaDeEspera; // Se utiliza para ordenar el nombre de los ficheros antes de ponerlo en el mapa
    string rutaSalidas = "../Salidas";
    if(fs::exists(rutaSalidas) && fs::is_directory(rutaSalidas)){
        
        for(fs::directory_entry archivo: fs::directory_iterator(rutaSalidas)){ // directory_entry no pilla los directorios en orden, por eso metemos las ruta en un vector
            salaDeEspera.push_back(archivo.path());
        }
        sort(salaDeEspera.begin(), salaDeEspera.end());

        int i = 1;
        for(string ruta: salaDeEspera){
            Tableros.insert({i, ruta});
            i++;

        }
    }else{
        cout << "Error al intentar acceder a la ruta (revisar código) \n" <<endl;
    }


    bool opcionValida = false;
    int opcion;

    while(!opcionValida){
        cout << "Seleccione un tablero para jugar (1-"<< int(Tableros.size()) <<"): ";
        cin >> opcion;
        if (Tableros.find(opcion) != Tableros.end()) {
            string nombreArchivo = Tableros[opcion];
            ifstream archivo(nombreArchivo);
            if (archivo.is_open()) {
                string linea;
                int j = 0;
                while (getline(archivo, linea)) {   
                    int i = 0;
                    while(i < int(linea.size())){
                        tablero[j][i] = (linea[i]); 
                        i++;
                    }
                    j++;
                }
                archivo.close();
                opcionValida = true;
            } else {
                cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
                break;
            }
        } else {
            cout << "Opción no válida. Por favor, seleccione un número entre 1 y "<< int(Tableros.size()) <<"." << endl;
        }
    }
}

void encontrarX(vector<vector<char>> &tablero, int &posX, int &posY){
    bool encontrado = false;
    for (int i = 0; i < filas && !encontrado; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            if(tablero[i][j] == 'X'){
                posX = i;
                posY = j;
                encontrado = true;
                break;
            }
        }
    }
}

bool dentroLimites(int x, int y){
    if(x >= filas || y >= columnas || x < 0 || y < 0){
        return false;
    }
    return true;
}

void delimitarVehiculo(vector<vector<char>> tablero, int x, int y, vector<pair<int,int>> &V, char vehiculo){
    if(dentroLimites(x,y) && tablero[x][y] == vehiculo && !Visitados[x][y]){
        pair<int,int> coord (x,y);
        V.push_back(coord);
        Visitados[x][y] = true;
        delimitarVehiculo(tablero, x, y+2, V, vehiculo);
        delimitarVehiculo(tablero, x+1, y, V, vehiculo); //sumo 2 al desplazarme a izq o dere para evitar las comas
        delimitarVehiculo(tablero, x, y-2, V, vehiculo);
        delimitarVehiculo(tablero, x-1, y, V, vehiculo);
        Visitados[x][y] = false;
    }
}

bool puedeMoverse(vector<vector<char>> tablero, int coordenadaDeX, vector<pair<int,int>> V, char mov){
    bool puede = true;
    for (int i = 0; i < V.size() && puede; i++)
    {
        switch (mov)
        {
        case 'u':
        case 'd':
            if(V[i].second != coordenadaDeX){
                puede = false;
            }
            break;

        case 'r':
        case 'l':
            if(V[i].first != coordenadaDeX){
                puede = false;
            }
            break;
        
        default:
            break;
        }
    }
    return puede;
}

void movimiento(vector<vector<char>> &tablero, char movimiento){
    int posX, posY;
    encontrarX(tablero, posX, posY);
    char vehiculo;
    bool movimientoValido = false;
    vector<pair<int,int>> V;
    switch (movimiento)
    {
    case 'u':
    if(dentroLimites(posX+1, posY)){
        vehiculo = tablero[posX+1][posY];
        delimitarVehiculo(tablero, posX+1, posY, V, vehiculo);
        if(V.size() == 1){
            tablero[posX+1][posY] = 'X';
            tablero[posX][posY] = vehiculo;
            movimientoValido = true;
        } else if(puedeMoverse(tablero, posY, V, movimiento)){
            if(V.size() == 2){
                tablero[posX+2][posY] = 'X';
                tablero[posX][posY] = vehiculo;
                movimientoValido = true;
            } else{
                tablero[posX+3][posY] = 'X';
                tablero[posX][posY] = vehiculo;
                movimientoValido = true;
            }
        } 
        if(movimientoValido){
            cout << "Has movido el vehículo " << vehiculo << " hacia arriba" << endl;
        } else{
            cout << "No se ha podido mover el vehículo " << vehiculo << " hacia arriba" << endl;
        }
    } else{
        cout << "No se ha podido mover el vehículo hacia arriba porque se encuentra en el límite del tablero" << endl;
    }
        break;
    case 'd':
    if(dentroLimites(posX-1, posY)){
        vehiculo = tablero[posX-1][posY];
        delimitarVehiculo(tablero, posX-1, posY, V, vehiculo);
        if(V.size() == 1){
            tablero[posX-1][posY] = 'X';
            tablero[posX][posY] = vehiculo;
            movimientoValido = true;
        } else if(puedeMoverse(tablero, posY, V, movimiento)){
            if(V.size() == 2){
                tablero[posX-2][posY] = 'X';
                tablero[posX][posY] = vehiculo;
                movimientoValido = true;
            } else{
                tablero[posX-3][posY] = 'X';
                tablero[posX][posY] = vehiculo;
                movimientoValido = true;
            }
        } 
        if(movimientoValido){
            cout << "Has movido el vehículo " << vehiculo << " hacia abajo" << endl;
        } else{
            cout << "No se ha podido mover el vehículo " << vehiculo << " hacia abajo" << endl;
        }
    } else{
        cout << "No se ha podido mover el vehículo hacia abajo porque se encuentra en el límite del tablero" << endl;
    }
    break;

    case 'r':
    if(dentroLimites(posX, posY-1)){
        vehiculo = tablero[posX][posY-2];
        delimitarVehiculo(tablero, posX, posY-2, V, vehiculo);
        if(V.size() == 1){
            tablero[posX][posY-2] = 'X';
            tablero[posX][posY] = vehiculo;
            movimientoValido = true;
        } else if(puedeMoverse(tablero, posX, V, movimiento)){
            if(V.size() == 2){
                tablero[posX][posY-4] = 'X';
                tablero[posX][posY] = vehiculo;
                movimientoValido = true;
            } else{
                tablero[posX][posY-6] = 'X';
                tablero[posX][posY] = vehiculo;
                movimientoValido = true;
            }
        } 
        if(movimientoValido){
            cout << "Has movido el vehículo " << vehiculo << " hacia la derecha" << endl;
        } else{
            cout << "No se ha podido mover el vehículo " << vehiculo << " hacia la derecha" << endl;
        }
    } else{
        cout << "No se ha podido mover el vehículo hacia la derecha porque se encuentra en el límite del tablero" << endl;
    }
    break;

    case 'l':
    if(dentroLimites(posX, posY+1)){
        vehiculo = tablero[posX][posY+2];
        delimitarVehiculo(tablero, posX, posY+2, V, vehiculo);
        if(V.size() == 1){
            tablero[posX][posY+2] = 'X';
            tablero[posX][posY] = vehiculo;
            movimientoValido = true;
        } else if(puedeMoverse(tablero, posX, V, movimiento)){
            if(V.size() == 2){
                tablero[posX][posY+4] = 'X';
                tablero[posX][posY] = vehiculo;
                movimientoValido = true;
            } else{
                tablero[posX][posY+6] = 'X';
                tablero[posX][posY] = vehiculo;
                movimientoValido = true;
            }
        } 
        if(movimientoValido){
            cout << "Has movido el vehículo " << vehiculo << " hacia la izquierda" << endl;
        } else{
            cout << "No se ha podido mover el vehículo " << vehiculo << " hacia la izquierda" << endl;
        }
    } else{
        cout << "No se ha podido mover el vehículo hacia la izquierda porque se encuentra en el límite del tablero" << endl;
    }
    break;

    default:
        cout << "Movimiento no válido, escoja de nuevo" << endl;
        break;
    }
}
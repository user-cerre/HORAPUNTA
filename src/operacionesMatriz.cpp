#include "operacionesMatriz.h"



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
    for (int i = 0; i < filas; i++)
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

//si a su alrededor no hay ninguna otra letra igual, es un coche.
bool esCoche (vector<vector<char>> &tablero, int x, int y, char vehiculo){
    if(tablero[x+1][y] != vehiculo || tablero[x][y+1] != vehiculo || tablero[x-1[y] != vehiculo] || 
            tablero[x][y-1] != vehiculo){
                return true;
            }
    return false;
}

void movimiento(vector<vector<char>> &tablero, char movimiento, int posX, int posY){
    char vehiculo;
    bool puedeMover = true;
    switch (movimiento)
    {
    case 'w':
        vehiculo = tablero[posX+1][posY];
        if(esCoche(tablero, posX+1, posY, vehiculo)){
            tablero[posX+1][posY] = 'X';
            tablero[posX][posY] = vehiculo;
        } else{

        }
        
        break;
    
    default:
        break;
    }
}
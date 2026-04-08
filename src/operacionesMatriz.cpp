#include "operacionesMatriz.h"

using namespace std::filesystem;

vector<vector<bool>> Visitados(filas, vector<bool>(columnas, false));

void mostrar( Tablero &tablero)
{
    for (const auto &fila : tablero.getTablero())
    {
        for (const auto &celda : fila)
        {
            cout << celda << " ";
        }
        cout << endl;
    }
}

bool cargarTablero(Tablero &tablero)
{
    map<int, string> Tableros;
    vector<string> salaDeEspera; // Se utiliza para ordenar el nombre de los ficheros antes de ponerlo en el mapa
    string rutaEntradas = "Entradas";
    vector<vector<char>> tableroAux (filas, vector<char>(columnas));
    if (filesystem::exists(rutaEntradas) && filesystem::is_directory(rutaEntradas))
    {

        for (directory_entry archivo : directory_iterator(rutaEntradas))
        { // directory_entry no pilla los directorios en orden, por eso metemos las ruta en un vector
            salaDeEspera.push_back(archivo.path());
        }
        // sort(salaDeEspera.begin(), salaDeEspera.end());
        sort(salaDeEspera.begin(), salaDeEspera.end(), [](const string &a, const string &b) {
        auto extraerNumero = [](const string &ruta) {
            size_t inicio = ruta.find("HORAPUNTA") + 9;
            size_t fin = ruta.find(".txt");
            return stoi(ruta.substr(inicio, fin - inicio));
        };

        return extraerNumero(a) < extraerNumero(b);
    });

        int i = 1;
        for (string ruta : salaDeEspera)
        {
            Tableros.insert({i, ruta});
            i++;
        }
    }
    else
    {
        cout << "Error al intentar acceder a la ruta (revisar código)" << endl;
    }

    bool opcionValida = false;
    int opcion;

    while (!opcionValida)
    {
        cout << "Seleccione un tablero para jugar (1-" << Tableros.size() << ") o 0 para salir: ";
        if (!(cin >> opcion)){ // Si la lectura falla (devuelve false)
    
        cerr << "Error: Debes introducir un numero." << endl;
        
        cin.clear(); //Limpiar el estado de error de cin
        
        //Descarta lo que haya en el buffer hasta el siguiente salto de línea
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        continue; // Salta al inicio del bucle para volver a preguntar
        }
        if (opcion == 0)
        {
            cout << "Saliendo del programa. ¡Hasta luego!" << endl;
            return false;
        }
        if (Tableros.find(opcion) != Tableros.end())
        {
            string nombreArchivo = Tableros[opcion];
            ifstream archivo(nombreArchivo);
            if (archivo.is_open())
            {
                string linea;
                int i = 0;
                while (getline(archivo, linea))
                {
                    int j = 0;
                    for (char c : linea)
                    {
                        if (c != ',')
                        {
                            tableroAux[i][j] = (c);
                            j++;
                        }
                    }
                    i++;
                }
                archivo.close();
                opcionValida = true;
                tablero.setTablero(tableroAux);
            }
            else
            {
                cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
                break;
            }
        }
        else
        {
            cout << "Opción no válida. Por favor, seleccione un número entre 1 y " << int(Tableros.size()) << "." << endl;
        }
    }
    return true;
}

void mostrarSolucionTablero(vector<char> solucion, int heuristica){
    cout << "Secuencia de la solución: ";
    if (solucion.empty() || heuristica != 0) {
        cout << "No se ha hallado la solución." << endl;
        return;
    }
    if(solucion.at(0) == ' '){
        cout << "El tablero ya está resuelto, no es necesario realizar ningún movimiento.";
    } else {
        for (long unsigned int i = 0; i < solucion.size(); i++) {
            cout << solucion[i];
            if (i + 1 < solucion.size()) {
                cout << ", ";
            }
        }
    }
    cout << endl;
}

void encontrarX(Tablero &tablero, int &posX, int &posY)
{
    bool encontrado = false;
    for (int i = 0; i < filas && !encontrado; i++)
    {
        for (int j = 0; j < columnas; j++) // Antes era +2 pero como hemos quitado comas, pues +1
        {

            if (tablero.getTablero()[i][j] == 'X')
            {
                posX = i;
                posY = j;
                encontrado = true;
                break;
            }
        }
    }
}
void encontrarZ(Tablero &tablero, int &posX, int &posY)
{
    bool encontrado = false;
    for (int i = 0; i < filas && !encontrado; i++)
    {
        for (int j = 0; j < columnas; j++) // Antes era +2 pero como hemos quitado comas, pues +1
        {
            if (tablero.getTablero()[i][j] == 'Z')
            {
                posX = i;
                posY = j;
                encontrado = true;
                break;
            }
        }
    }
}

bool dentroLimites(int x, int y)
{
    if (x >= filas || y >= columnas || x < 0 || y < 0)
    {
        return false;
    }
    return true;
}

void delimitarVehiculo(Tablero &tablero, int x, int y, vector<pair<int, int>> &V, char vehiculo)
{
    if (dentroLimites(x, y) && tablero.getTablero()[x][y] == vehiculo && !Visitados[x][y])
    {
        pair<int, int> coord(x, y);
        V.push_back(coord);
        Visitados[x][y] = true;
        delimitarVehiculo(tablero, x, y + 1, V, vehiculo);
        delimitarVehiculo(tablero, x + 1, y, V, vehiculo);
        delimitarVehiculo(tablero, x, y - 1, V, vehiculo);
        delimitarVehiculo(tablero, x - 1, y, V, vehiculo);
        Visitados[x][y] = false;
    }
}

bool puedeMoverse(int coordenadaDeX, vector<pair<int, int>> V, char mov)
{
    if (V.empty())
    {
        return false;
    }

    if (mov == 'u' || mov == 'd')
    {
        for (const auto &coord : V)
        {
            if (coord.second != coordenadaDeX)
            {
                return false;
            }
        }
        return true;
    }

    if (mov == 'l' || mov == 'r')
    {
        for (const auto &coord : V)
        {
            if (coord.first != coordenadaDeX)
            {
                return false;
            }
        }
        return true;
    }

    return false;
}

char moverVehiculo(Tablero &tablero, char movimiento, bool mostrarMensajes)
{
    int posX, posY;
    vector<vector<char>> tableroTemp = tablero.getTablero();
    encontrarX(tablero, posX, posY);
    char vehiculo = ' ';
    bool movimientoValido = false;
    vector<pair<int,int>> V;

    switch (movimiento)
    {
    case 'u':
        V.clear();
        if (dentroLimites(posX + 1, posY))
        {
            vehiculo = tablero.getTablero()[posX + 1][posY];
            delimitarVehiculo(tablero, posX + 1, posY, V, vehiculo);
            if (V.size() == 1)
            {
                tableroTemp[posX + 1][posY] = 'X';
                tableroTemp[posX][posY] = vehiculo;
                tablero.setTablero(tableroTemp);
                movimientoValido = true;
            }
            else if (puedeMoverse(posY, V, movimiento))
            {
                if (V.size() == 2)
                {
                    tableroTemp[posX + 2][posY] = 'X';
                    tableroTemp[posX][posY] = vehiculo;
                    tablero.setTablero(tableroTemp);
                    movimientoValido = true;
                }
                else
                {
                    tableroTemp[posX + 3][posY] = 'X';
                    tableroTemp[posX][posY] = vehiculo;
                    tablero.setTablero(tableroTemp);
                    movimientoValido = true;
                }
            }
        }
        break;

    case 'd':
        V.clear();
        if (dentroLimites(posX - 1, posY))
        {
            vehiculo = tablero.getTablero()[posX - 1][posY];
            delimitarVehiculo(tablero, posX - 1, posY, V, vehiculo);
            if (V.size() == 1)
            {
                tableroTemp[posX - 1][posY] = 'X';
                tableroTemp[posX][posY] = vehiculo;
                tablero.setTablero(tableroTemp);
                movimientoValido = true;
            }
            else if (puedeMoverse(posY, V, movimiento))
            {
                if (V.size() == 2)
                {
                    tableroTemp[posX - 2][posY] = 'X';
                    tableroTemp[posX][posY] = vehiculo;
                    tablero.setTablero(tableroTemp);
                    movimientoValido = true;
                }
                else
                {
                    tableroTemp[posX - 3][posY] = 'X';
                    tableroTemp[posX][posY] = vehiculo;
                    tablero.setTablero(tableroTemp);
                    movimientoValido = true;
                }
            }
        }
        break;

    case 'r':
        V.clear();
        if (dentroLimites(posX, posY - 1))
        {
            vehiculo = tablero.getTablero()[posX][posY - 1];
            delimitarVehiculo(tablero, posX, posY - 1, V, vehiculo);
            if (V.size() == 1)
            {
                tableroTemp[posX][posY - 1] = 'X';
                tableroTemp[posX][posY] = vehiculo;
                tablero.setTablero(tableroTemp);
                movimientoValido = true;
            }
            else if (puedeMoverse(posX, V, movimiento))
            {
                if (V.size() == 2)
                {
                    tableroTemp[posX][posY - 2] = 'X';
                    tableroTemp[posX][posY] = vehiculo;
                    tablero.setTablero(tableroTemp);
                    movimientoValido = true;
                }
                else
                {
                    tableroTemp[posX][posY - 3] = 'X';
                    tableroTemp[posX][posY] = vehiculo;
                    tablero.setTablero(tableroTemp);
                    movimientoValido = true;
                }
            }
        }
        break;

    case 'l':
        V.clear();
        if (dentroLimites(posX, posY + 1))
        {
            vehiculo = tablero.getTablero()[posX][posY + 1];
            delimitarVehiculo(tablero, posX, posY + 1, V, vehiculo);
            if (V.size() == 1)
            {
                tableroTemp[posX][posY + 1] = 'X';
                tableroTemp[posX][posY] = vehiculo;
                tablero.setTablero(tableroTemp);
                movimientoValido = true;
            }
            else if (puedeMoverse(posX, V, movimiento))
            {
                if (V.size() == 2)
                {
                    tableroTemp[posX][posY + 2] = 'X';
                    tableroTemp[posX][posY] = vehiculo;
                    tablero.setTablero(tableroTemp);
                    movimientoValido = true;
                }
                else
                {
                    tableroTemp[posX][posY + 3] = 'X';
                    tableroTemp[posX][posY] = vehiculo;
                    tablero.setTablero(tableroTemp);
                    movimientoValido = true;
                }
            }
        }
        break;

    default:
        movimientoValido = false;
        break;
    }

    if (!movimientoValido)
    {
        if (mostrarMensajes)
        {
            if (movimiento == 'u')
            {
                cout << "No se ha podido mover el vehículo hacia arriba" << endl;
            }
            else if (movimiento == 'd')
            {
                cout << "No se ha podido mover el vehículo hacia abajo" << endl;
            }
            else if (movimiento == 'r')
            {
                cout << "No se ha podido mover el vehículo hacia la derecha" << endl;
            }
            else if (movimiento == 'l')
            {
                cout << "No se ha podido mover el vehículo hacia la izquierda" << endl;
            }
            else
            {
                cout << "Movimiento no válido, escoja de nuevo" << endl;
            }
        }
        return ' ';
    }

    if (mostrarMensajes)
    {
        cout << "Has movido el vehículo " << vehiculo << endl;
    }
    return vehiculo;
}

bool esObjetivo(Tablero tablero)
{
    int xZ = 0;
    int yZ = 0;
    encontrarZ(tablero, xZ, yZ);
    return xZ == 0 || xZ == 5 || yZ == 0 || yZ == 5;
}

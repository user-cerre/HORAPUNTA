#include "operacionesMatriz.h"

vector<vector<char>> Visitados(filas, vector<char>(columnas, false));

void mostrar(const vector<vector<char>> &tablero)
{
    for (const auto &fila : tablero)
    {
        for (const auto &celda : fila)
        {
            cout << celda << " ";
        }
        cout << endl;
    }
}

void cargarTablero(vector<vector<char>> &tablero)
{
    map<int, string> Tableros;
    vector<string> salaDeEspera; // Se utiliza para ordenar el nombre de los ficheros antes de ponerlo en el mapa
    string rutaSalidas = "../Salidas";
    if (exists(rutaSalidas) && is_directory(rutaSalidas))
    {

        for (directory_entry archivo : directory_iterator(rutaSalidas))
        { // directory_entry no pilla los directorios en orden, por eso metemos las ruta en un vector
            salaDeEspera.push_back(archivo.path());
        }
        sort(salaDeEspera.begin(), salaDeEspera.end());

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
        cout << "Seleccione un tablero para jugar (1-" << Tableros.size() << "): ";
        cin >> opcion;
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
                            tablero[i][j] = (c);
                            j++;
                        }
                    }
                    i++;
                }
                archivo.close();
                opcionValida = true;
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
}

void encontrarX(vector<vector<char>> &tablero, int &posX, int &posY)
{
    bool encontrado = false;
    for (int i = 0; i < filas && !encontrado; i++)
    {
        for (int j = 0; j < columnas; j++) // Antes era +2 pero como hemos quitado comas, pues +1
        {

            if (tablero[i][j] == 'X')
            {
                posX = i;
                posY = j;
                encontrado = true;
                break;
            }
        }
    }
}
void encontrarZ(vector<vector<char>> &tablero, int &posX, int &posY)
{
    bool encontrado = false;
    for (int i = 0; i < filas && !encontrado; i++)
    {
        for (int j = 0; j < columnas; j++) // Antes era +2 pero como hemos quitado comas, pues +1
        {
            if (tablero[i][j] == 'Z')
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

void delimitarVehiculo(vector<vector<char>> tablero, int x, int y, vector<pair<int, int>> &V, char vehiculo)
{
    if (dentroLimites(x, y) && tablero[x][y] == vehiculo && !Visitados[x][y])
    {
        pair<int, int> coord(x, y);
        V.push_back(coord);
        Visitados[x][y] = true;
        delimitarVehiculo(tablero, x, y + 1, V, vehiculo);
        delimitarVehiculo(tablero, x + 1, y, V, vehiculo); // sumo 2 al desplazarme a izq o dere para evitar las comas // OJO, ya
        delimitarVehiculo(tablero, x, y - 1, V, vehiculo);
        delimitarVehiculo(tablero, x - 1, y, V, vehiculo);
        Visitados[x][y] = false;
    }
}
// Revisión
bool puedeMoverse(int coordenadaDeX, vector<pair<int, int>> V, char mov)
{
    bool puede = true;
    for (int i = 0; i < V.size() && puede; i++)
    {
        switch (mov)
        {
        case 'u':
        case 'd':
            if (V[i].second != coordenadaDeX)
            {
                puede = false;
            }
            break;

        case 'r':
        case 'l':
            if (V[i].first != coordenadaDeX)
            {
                puede = false;
            }
            break;

        default:
            break;
        }
    }
    return puede;
}

void movimiento(vector<vector<char>> &tablero, char movimiento)
{
    int posX, posY;
    encontrarX(tablero, posX, posY);
    char vehiculo;
    bool movimientoValido = false;
    vector<pair<int, int>> V;
    switch (movimiento)
    {
    case 'u':
        V.clear();
        if (dentroLimites(posX + 1, posY))
        {
            vehiculo = tablero[posX + 1][posY];
            delimitarVehiculo(tablero, posX + 1, posY, V, vehiculo);
            if (V.size() == 1)
            {
                tablero[posX + 1][posY] = 'X';
                tablero[posX][posY] = vehiculo;
                movimientoValido = true;
            }
            else if (puedeMoverse(posY, V, movimiento))
            {
                if (V.size() == 2)
                {
                    tablero[posX + 2][posY] = 'X';
                    tablero[posX][posY] = vehiculo;
                    movimientoValido = true;
                }
                else
                {
                    tablero[posX + 3][posY] = 'X';
                    tablero[posX][posY] = vehiculo;
                    movimientoValido = true;
                }
            }
            if (movimientoValido)
            {
                cout << "Has movido el vehículo " << vehiculo << " hacia arriba" << endl;
            }
            else
            {
                cout << "No se ha podido mover el vehículo " << vehiculo << " hacia arriba" << endl;
                cout << endl;
            }
        }
        else
        {
            cout << "No se ha podido mover el vehículo hacia arriba porque se encuentra en el límite del tablero" << endl;
        }
        break;
    case 'd':
        V.clear();
        if (dentroLimites(posX - 1, posY))
        {
            vehiculo = tablero[posX - 1][posY];
            delimitarVehiculo(tablero, posX - 1, posY, V, vehiculo);
            if (V.size() == 1)
            {
                tablero[posX - 1][posY] = 'X';
                tablero[posX][posY] = vehiculo;
                movimientoValido = true;
            }
            else if (puedeMoverse(posY, V, movimiento))
            {
                if (V.size() == 2)
                {
                    tablero[posX - 2][posY] = 'X';
                    tablero[posX][posY] = vehiculo;
                    movimientoValido = true;
                }
                else
                {
                    tablero[posX - 3][posY] = 'X';
                    tablero[posX][posY] = vehiculo;
                    movimientoValido = true;
                }
            }
            if (movimientoValido)
            {
                cout << "Has movido el vehículo " << vehiculo << " hacia abajo" << endl;
            }
            else
            {
                cout << "No se ha podido mover el vehículo " << vehiculo << " hacia abajo" << endl;
            }
        }
        else
        {
            cout << "No se ha podido mover el vehículo hacia abajo porque se encuentra en el límite del tablero" << endl;
            cout << endl;
        }
        break;

    case 'r': // Estoy cambiando esto para adaptar el movimiento a la matriz sin comas
        V.clear();
        if (dentroLimites(posX, posY - 1))
        {
            vehiculo = tablero[posX][posY - 2];
            delimitarVehiculo(tablero, posX, posY - 2, V, vehiculo);
            if (V.size() == 1)
            {
                tablero[posX][posY - 1] = 'X';
                tablero[posX][posY] = vehiculo;
                movimientoValido = true;
            }
            else if (puedeMoverse(posX, V, movimiento))
            {
                if (V.size() == 2)
                {
                    tablero[posX][posY - 2] = 'X';
                    tablero[posX][posY] = vehiculo;
                    movimientoValido = true;
                }
                else
                {
                    tablero[posX][posY - 3] = 'X';
                    tablero[posX][posY] = vehiculo;
                    movimientoValido = true;
                }
            }
            if (movimientoValido)
            {
                cout << "Has movido el vehículo " << vehiculo << " hacia la derecha" << endl;
            }
            else
            {
                cout << "No se ha podido mover el vehículo " << vehiculo << " hacia la derecha" << endl;
                cout << endl;
            }
        }
        else
        {
            cout << "No se ha podido mover el vehículo hacia la derecha porque se encuentra en el límite del tablero" << endl;
        }
        break;

    case 'l':
        V.clear();
        if (dentroLimites(posX, posY + 1))
        {
            vehiculo = tablero[posX][posY + 2];
            delimitarVehiculo(tablero, posX, posY + 2, V, vehiculo);
            if (V.size() == 1)
            {
                tablero[posX][posY + 1] = 'X';
                tablero[posX][posY] = vehiculo;
                movimientoValido = true;
            }
            else if (puedeMoverse(posX, V, movimiento))
            {
                if (V.size() == 2)
                {
                    tablero[posX][posY + 2] = 'X';
                    tablero[posX][posY] = vehiculo;
                    movimientoValido = true;
                }
                else
                {
                    tablero[posX][posY + 3] = 'X';
                    tablero[posX][posY] = vehiculo;
                    movimientoValido = true;
                }
            }
            if (movimientoValido)
            {
                cout << "Has movido el vehículo " << vehiculo << " hacia la izquierda" << endl;
            }
            else
            {
                cout << "No se ha podido mover el vehículo " << vehiculo << " hacia la izquierda" << endl;
                cout << endl;
            }
        }
        else
        {
            cout << "No se ha podido mover el vehículo hacia la izquierda porque se encuentra en el límite del tablero" << endl;
        }
        break;

    default:
        cout << "Movimiento no válido, escoja de nuevo" << endl;
        break;
    }
}

int calcularHeuristicaManhattan(vector<vector<char>> &tablero)
{
    int posXdeX, posYdeX, posXdeZ, posYdeZ;
    int distanciaManhattan;
    encontrarX(tablero, posXdeX, posYdeX);
    encontrarZ(tablero, posXdeZ, posYdeZ);
    return abs(posXdeX - (posXdeZ)) + abs((posYdeX - posYdeZ)); // dividimos entre 2 porque las columnas van de 2 en 2 por las comas
}
int calcularEscaladaSimple(vector<vector<char>> &tablero)
{
    if (calcularHeuristicaManhattan(tablero) == 0)
    {
        return 0;
    }
    else
    {
        int mejorHeuristica = calcularHeuristicaManhattan(tablero);
        vector<char> movimientos = {'u', 'd', 'r', 'l'};
        for (char mov : movimientos)
        {
            vector<vector<char>> tableroCopia = tablero;
            movimiento(tableroCopia, mov);
            int heuristica = calcularHeuristicaManhattan(tableroCopia);
            if (heuristica < mejorHeuristica)
            {
                return heuristica;
            }
        }
        return mejorHeuristica;
    }
}

int calcularEscaladaMaximaPendiente(vector<vector<char>> &tablero)
{
    // TODO: Implementar cálculo de escalada de máxima pendiente
    return 0;
}

int calcularEscaladaSimpleEstocastica(vector<vector<char>> &tablero)
{
    // TODO: Implementar cálculo de escalada simple estocástica
    return 0;
}

int calcularAlgoritmoEstrella(vector<vector<char>> &tablero)
{
    // TODO: Implementar algoritmo A*
    return 0;
}

int calcularAlgoritmoAndOr(vector<vector<char>> &tablero)
{
    // TODO: Implementar algoritmo AND-OR
    return 0;
}

// TODO : queda pendiente hacer otros 3 tipos de heurísticas
// TODO: reescribir la matriz para la próxima entrega (problema de comas que no influye en esta entrega pero que sí lo hará en la próxima)
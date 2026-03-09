#include "operacionesMatriz.h"
#include "Tablero.h"
#include <filesystem>

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

void cargarTablero(Tablero &tablero)
{
    map<int, string> Tableros;
    vector<string> salaDeEspera; // Se utiliza para ordenar el nombre de los ficheros antes de ponerlo en el mapa
    string rutaSalidas = "../Salidas";
    vector<vector<char>> tableroAux (filas, vector<char>(columnas));
    if (filesystem::exists(rutaSalidas) && filesystem::is_directory(rutaSalidas))
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

char moverVehiculo(Tablero &tablero, char movimiento)
{
    int posX, posY;
    encontrarX(tablero, posX, posY);
    char vehiculo;
    bool movimientoValido = false;
    vector<pair<int, int>> V;
    vector<vector<char>> tableroRef = tablero.getTablero();
    switch (movimiento)
    {
    case 'u':
        V.clear();
        if (dentroLimites(posX + 1, posY))
        {
            vehiculo = tableroRef[posX + 1][posY];
            delimitarVehiculo(tablero, posX + 1, posY, V, vehiculo);
            if (V.empty())
            {
                movimientoValido = false;
            }
            else if (V.size() == 1)
            {
                tableroRef[posX + 1][posY] = 'X';
                tableroRef[posX][posY] = vehiculo;
                movimientoValido = true;
            }
            else if (puedeMoverse(posY, V, movimiento))
            {
                if (V.size() == 2)
                {
                    if (dentroLimites(posX + 2, posY))
                    {
                        tableroRef[posX + 2][posY] = 'X';
                        tableroRef[posX][posY] = vehiculo;
                        movimientoValido = true;
                    }
                }
                else
                {
                    if (dentroLimites(posX + 3, posY))
                    {
                        tableroRef[posX + 3][posY] = 'X';
                        tableroRef[posX][posY] = vehiculo;
                        movimientoValido = true;
                    }
                }
            }
            if (movimientoValido)
            {
                tablero.setTablero(tableroRef);// Actualizamos el tablero con los cambios realizados en tableroRef
                cout << "Has movido el vehículo " << vehiculo << " hacia arriba" << endl;
            }
            else
            {
                cout << "No se ha podido mover el vehículo " << vehiculo << " hacia arriba" << endl;
                cout << endl;
                vehiculo = ' ';
            }
        }
        else
        {
            cout << "No se ha podido mover el vehículo hacia arriba porque se encuentra en el límite del tablero" << endl;
            vehiculo = ' ';
        }
        break;
    case 'd':
        V.clear();
        if (dentroLimites(posX - 1, posY))
        {
            vehiculo = tableroRef[posX - 1][posY];
            delimitarVehiculo(tablero, posX - 1, posY, V, vehiculo);
            if (V.empty())
            {
                movimientoValido = false;
            }
            else if (V.size() == 1)
            {
                tableroRef[posX - 1][posY] = 'X';
                tableroRef[posX][posY] = vehiculo;
                movimientoValido = true;
            }
            else if (puedeMoverse(posY, V, movimiento))
            {
                if (V.size() == 2)
                {
                    if (dentroLimites(posX - 2, posY))
                    {
                        tableroRef[posX - 2][posY] = 'X';
                        tableroRef[posX][posY] = vehiculo;
                        movimientoValido = true;
                    }
                }
                else
                {
                    if (dentroLimites(posX - 3, posY))
                    {
                        tableroRef[posX - 3][posY] = 'X';
                        tableroRef[posX][posY] = vehiculo;
                        movimientoValido = true;
                    }
                }
            }
            if (movimientoValido)
            {
                tablero.setTablero(tableroRef);// Actualizamos el tablero con los cambios realizados en tableroRef
                cout << "Has movido el vehículo " << vehiculo << " hacia abajo" << endl;
            }
            else
            {
                cout << "No se ha podido mover el vehículo " << vehiculo << " hacia abajo" << endl;
                vehiculo = ' ';
            }
        }
        else
        {
            cout << "No se ha podido mover el vehículo hacia abajo porque se encuentra en el límite del tablero" << endl;
            cout << endl;
            vehiculo = ' ';
        }
        break;

    case 'r': // Estoy cambiando esto para adaptar el movimiento a la matriz sin comas
        V.clear();
        if (dentroLimites(posX, posY - 1))
        {
            vehiculo = tableroRef[posX][posY - 1];
            delimitarVehiculo(tablero, posX, posY - 1, V, vehiculo);
            if (V.empty())
            {
                movimientoValido = false;
            }
            else if (V.size() == 1)
            {
                tableroRef[posX][posY - 1] = 'X';
                tableroRef[posX][posY] = vehiculo;
                movimientoValido = true;
            }
            else if (puedeMoverse(posX, V, movimiento))
            {
                if (V.size() == 2)
                {
                    if (dentroLimites(posX, posY - 2))
                    {
                        tableroRef[posX][posY - 2] = 'X';
                        tableroRef[posX][posY] = vehiculo;
                        movimientoValido = true;
                    }
                }
                else
                {
                    if (dentroLimites(posX, posY - 3))
                    {
                        tableroRef[posX][posY - 3] = 'X';
                        tableroRef[posX][posY] = vehiculo;
                        movimientoValido = true;
                    }
                }
            }
            if (movimientoValido)
            {
                tablero.setTablero(tableroRef);// Actualizamos el tablero con los cambios realizados en tableroRef
                cout << "Has movido el vehículo " << vehiculo << " hacia la derecha" << endl;
            }
            else
            {
                cout << "No se ha podido mover el vehículo " << vehiculo << " hacia la derecha" << endl;
                cout << endl;
                vehiculo = ' ';
            }
        }
        else
        {
            cout << "No se ha podido mover el vehículo hacia la derecha porque se encuentra en el límite del tablero" << endl;
            vehiculo = ' ';
        }
        break;

    case 'l':
        V.clear();
        if (dentroLimites(posX, posY + 1))
        {
            vehiculo = tableroRef[posX][posY + 1];
            delimitarVehiculo(tablero, posX, posY + 1, V, vehiculo);
            if (V.empty())
            {
                movimientoValido = false;
            }
            else if (V.size() == 1)
            {
                tableroRef[posX][posY + 1] = 'X';
                tableroRef[posX][posY] = vehiculo;
                movimientoValido = true;
            }
            else if (puedeMoverse(posX, V, movimiento))
            {
                if (V.size() == 2)
                {
                    if (dentroLimites(posX, posY + 2))
                    {
                        tableroRef[posX][posY + 2] = 'X';
                        tableroRef[posX][posY] = vehiculo;
                        movimientoValido = true;
                    }
                }
                else
                {
                    if (dentroLimites(posX, posY + 3))
                    {
                        tableroRef[posX][posY + 3] = 'X';
                        tableroRef[posX][posY] = vehiculo;
                        movimientoValido = true;
                    }
                }
            }
            if (movimientoValido)
            {
                tablero.setTablero(tableroRef);// Actualizamos el tablero con los cambios realizados en tableroRef
                cout << "Has movido el vehículo " << vehiculo << " hacia la izquierda" << endl;
            }
            else
            {
                cout << "No se ha podido mover el vehículo " << vehiculo << " hacia la izquierda" << endl;
                cout << endl;
                vehiculo = ' ';
            }
        }
        else
        {
            cout << "No se ha podido mover el vehículo hacia la izquierda porque se encuentra en el límite del tablero" << endl;
            vehiculo = ' ';
        }
        break;

    default:
        cout << "Movimiento no válido, escoja de nuevo" << endl;
        break;
    }

    return vehiculo;
}

int calcularHeuristicaXAZ(Tablero &tablero)
{
    int posXdeX, posYdeX, posXdeZ, posYdeZ;
    int distanciaManhattan;
    encontrarX(tablero, posXdeX, posYdeX);
    encontrarZ(tablero, posXdeZ, posYdeZ);
    return abs(posXdeX - (posXdeZ)) + abs((posYdeX - posYdeZ));
}

int distanciaZABorde(Tablero &tablero)
{
    int posXdeZ, posYdeZ;
    encontrarZ(tablero, posXdeZ, posYdeZ);
    int distancia = min(posXdeZ, posYdeZ);
    if (distancia < 3)
    {
        return distancia;
    }
    return 5 - distancia;
}
int distanciaXABordeDeZ(Tablero &tablero)
{
    int posXdeZ, posYdeZ;
    encontrarZ(tablero, posXdeZ, posYdeZ);
    int posXdeX, posYdeX;
    encontrarX(tablero, posXdeX, posYdeX);
    if(posXdeZ == posXdeX){
        int distancia = posXdeX;
        if (distancia < 3)
        {
            return distancia;
        }
        return 5 - distancia;
    } else if(posYdeZ == posYdeX){
        int distancia = posYdeX;
        if (distancia < 3)
        {
            return distancia;
        }else{
            return 5 - distancia;
        }
    }else{
        if(posXdeZ < posYdeZ){//TODO revisar esta parte
            int distancia = posXdeX;
            if (distancia < 3 && posXdeZ < 3)
            {
                return distancia;
            }
            return 5 - distancia;
        } else{
            int distancia = posYdeX;
            if (distancia < 3 && posYdeZ < 3)
            {
                return distancia;
            }
            return 5 - distancia;
        }
    }
}

// int calcularDistancia(const vector<vector<char>> &tablero) {
//     int posX, posY;
//     encontrarX(tablero, posX, posY);
//     int distanciaMinima = INT_MAX;
//     // Calcular distancia al borde más cercano
//     for (int i = 0; i < filas; i++) {
//         for (int j = 0; j < columnas; j++) {
//             if (tablero[i][j] == 'Z') {
//                 int distancia = min({posX, filas - 1 - posX, posY, columnas - 1 - posY});
//                 distanciaMinima = min(distanciaMinima, distancia);
//             }
//         }
//     }
//     return distanciaMinima;
// }

int calcularHeuristicaTablero(Tablero &tablero, char vehiculo)
{
    if( vehiculo == ' '){
        return INT_MAX;
    }
    int heurZ = calcularHeuristicaXAZ(tablero);
    int heurBordeZ = distanciaZABorde(tablero);
    int heurBordeX = distanciaXABordeDeZ(tablero);
    return heurZ + heurBordeZ /* + heurBordeX*/;
}

vector<char> calcularEscaladaSimple(Tablero &tablero)
{
    vector<char> solucion;
    int heuristicaActual;
    char vehiculo = '?';
    while(calcularHeuristicaTablero(tablero, vehiculo) != 1 ){ 
        heuristicaActual = calcularHeuristicaTablero(tablero, vehiculo);
        for (int i = 0; i < 4; i++)
        {
            vector<vector<char>> tableroTemp = tablero.getTablero();
            char mov;
            switch (i)
            {
            case 0:
                mov = 'u';
                break;
            case 1:
                mov = 'd';
                break;
            case 2:
                mov = 'l';
                break;
            case 3:
                mov = 'r';
                break;
            }
            vehiculo = moverVehiculo(tablero, mov);
            if (calcularHeuristicaTablero(tablero, vehiculo) <= heuristicaActual)
            {
                tablero.setTablero(tableroTemp);
                solucion.push_back(vehiculo);
                // mostrar(tablero);
                // cout << "Movimiento válido: " << vehiculo << endl;
                // cout << "Heurística actual: " << calcularHeuristicaTablero(tablero, vehiculo) << endl;
            }
        }
    }
    tablero.agregarSolucion(solucion);
    return solucion;
    
}

vector<char> calcularEscaladaMaximaPendiente(Tablero &tablero)
{
    vector<char> solucion;
    // TODO: Implementar cálculo de escalada de máxima pendiente
    return solucion;
}

vector<char> calcularEscaladaSimpleEstocastica(Tablero &tablero)
{
    vector<char> solucion;
    // TODO: Implementar cálculo de escalada simple estocástica
    return solucion;
}

vector<char> calcularAlgoritmoEstrella(Tablero &tablero)
{
    vector<char> solucion;
    // TODO: Implementar algoritmo A*
    return solucion;
}

vector<char> calcularAlgoritmoAndOr(Tablero &tablero)
{
    vector<char> solucion;
    // TODO: Implementar algoritmo AND-OR
    return solucion;
}
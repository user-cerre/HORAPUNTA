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

void movimiento(Tablero &tablero, char movimiento){
    int posX, posY;
    vector<vector<char>> tableroTemp = tablero.getTablero();
    encontrarX(tablero, posX, posY);
    char vehiculo;
    bool movimientoValido = false;
    vector<pair<int,int>> V;
    switch (movimiento)
    {
    case 'u':
    V.clear();
    if(dentroLimites(posX+1, posY)){
        vehiculo = tablero.getTablero()[posX+1][posY];
        delimitarVehiculo(tablero, posX+1, posY, V, vehiculo);
        if(V.size() == 1){
            tableroTemp[posX+1][posY] = 'X';
            tableroTemp[posX][posY] = vehiculo;
            tablero.setTablero(tableroTemp);
            movimientoValido = true;
        } else if(puedeMoverse(posY, V, movimiento)){
            if(V.size() == 2){
                tableroTemp[posX+2][posY] = 'X';
                tableroTemp[posX][posY] = vehiculo;
                tablero.setTablero(tableroTemp);
                movimientoValido = true;
            } else{
                tableroTemp[posX+3][posY] = 'X';
                tableroTemp[posX][posY] = vehiculo;
                tablero.setTablero(tableroTemp);
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
    V.clear();
    if(dentroLimites(posX-1, posY)){
        vehiculo = tablero.getTablero()[posX-1][posY];
        delimitarVehiculo(tablero, posX-1, posY, V, vehiculo);
        if(V.size() == 1){
            tableroTemp[posX-1][posY] = 'X';
            tableroTemp[posX][posY] = vehiculo;
            tablero.setTablero(tableroTemp);
            movimientoValido = true;
        } else if(puedeMoverse(posY, V, movimiento)){
            if(V.size() == 2){
                tableroTemp[posX-2][posY] = 'X';
                tableroTemp[posX][posY] = vehiculo;
                tablero.setTablero(tableroTemp);
                movimientoValido = true;
            } else{
                tableroTemp[posX-3][posY] = 'X';
                tableroTemp[posX][posY] = vehiculo;
                tablero.setTablero(tableroTemp);
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
    V.clear();
    if(dentroLimites(posX, posY-1)){
        vehiculo = tablero.getTablero()[posX][posY-1];
        delimitarVehiculo(tablero, posX, posY-1, V, vehiculo);
        if(V.size() == 1){
            tableroTemp[posX][posY-1] = 'X';
            tableroTemp[posX][posY] = vehiculo;
            tablero.setTablero(tableroTemp);
            movimientoValido = true;
        } else if(puedeMoverse(posX, V, movimiento)){
            if(V.size() == 2){
                tableroTemp[posX][posY-2] = 'X';
                tableroTemp[posX][posY] = vehiculo;
                tablero.setTablero(tableroTemp);
                movimientoValido = true;
            } else{
                tableroTemp[posX][posY-3] = 'X';
                tableroTemp[posX][posY] = vehiculo;
                tablero.setTablero(tableroTemp);
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
    V.clear();
    if(dentroLimites(posX, posY+1)){
        vehiculo = tablero.getTablero()[posX][posY+1];
        delimitarVehiculo(tablero, posX, posY+1, V, vehiculo);
        if(V.size() == 1){
            tableroTemp[posX][posY+1] = 'X';
            tableroTemp[posX][posY] = vehiculo;
            tablero.setTablero(tableroTemp);
            movimientoValido = true;
        } else if(puedeMoverse(posX, V, movimiento)){
            if(V.size() == 2){
                tableroTemp[posX][posY+2] = 'X';
                tableroTemp[posX][posY] = vehiculo;
                tablero.setTablero(tableroTemp);
                movimientoValido = true;
            } else{
                tableroTemp[posX][posY+3] = 'X';
                tableroTemp[posX][posY] = vehiculo;
                tablero.setTablero(tableroTemp);
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

bool esObjetivo(Tablero &tablero)
{
    int xZ = 0;
    int yZ = 0;
    encontrarZ(tablero, xZ, yZ);
    return xZ == 0 || xZ == 5 || yZ == 0 || yZ == 5;
}

int calcularHeuristicaTablero(Tablero &tablero, char vehiculo)
{
    int xZ = 0;
    int yZ = 0;
    encontrarZ(tablero, xZ, yZ);
    int a = min (xZ, yZ);
    int b = min (5 - xZ, 5 - yZ);
    int h1= min(a, b);
    int xX = 0;
    int yX = 0;
    encontrarX(tablero, xX, yX);
    if(xX == xZ || yX == yZ){
        return h1;
    }else{
        return h1 +1;
    }
    // int heurZ = calcularHeuristicaXAZ(tablero);
    // int heurBordeZ = distanciaZABorde(tablero);
    // int heurBordeX = distanciaXABordeDeZ(tablero);
    // return heurZ + heurBordeZ /* + heurBordeX*/;
    // return min()
}

vector<char> calcularEscaladaSimple(Tablero &tablero)
{
    vector<char> solucion;
    int heuristicaActual;
    int iterMax = 200;
    int iter = 0;
    char vehiculo = '?';
    while(!esObjetivo(tablero) && iter < iterMax){
        heuristicaActual = calcularHeuristicaTablero(tablero, vehiculo);
        bool huboMejora = false;
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
            vehiculo = moverVehiculo(tablero, mov, false);
            if (vehiculo == ' ')
            {
                tablero.setTablero(tableroTemp);
                continue;
            }
            int heuristicaNueva = calcularHeuristicaTablero(tablero, vehiculo);
            if (heuristicaNueva < heuristicaActual)
            {
                heuristicaActual = heuristicaNueva;
                huboMejora = true;
                solucion.push_back(vehiculo);
                cout << "Has movido el vehículo " << vehiculo << endl;
                mostrar(tablero);
                cout << "Heurística actual: " << heuristicaNueva << endl;
                cout << endl;
                if (heuristicaNueva == 0)
                {
                    break;
                }
                break;
            }
            else
            {
                tablero.setTablero(tableroTemp);
            }
        }
        if (esObjetivo(tablero))
        {
            break;
        }
        if (!huboMejora)
        {
            break;
        }
        iter++;
    }
    cout << "Pasos realizados: ";
    if (solucion.empty())
    {
        cout << "(sin pasos)";
    }
    else
    {
        for (int i = 0; i < solucion.size(); i++)
        {
            cout << solucion[i];
            if (i + 1 < solucion.size())
            {
                cout << ", ";
            }
        }
    }
    cout << endl;

    tablero.agregarSolucion(solucion);
    return solucion;
    
}

vector<char> calcularEscaladaMaximaPendiente(Tablero &tablero)
{
    vector<char> solucion;
    int iterMax = 200;
    int iter = 0;
    while(!esObjetivo(tablero) && iter < iterMax){
        int heuristicaActual = calcularHeuristicaTablero(tablero, '?');
        int mejorHeuristica = heuristicaActual;
        char vehiculo = '?';
        char mejorVehiculo = '?';
        char mejorMovimiento = '?';
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
            vehiculo = moverVehiculo(tablero, mov, false);
            if (vehiculo == ' ')
            {
                tablero.setTablero(tableroTemp);
                continue;
            }
            int heuristicaNueva = calcularHeuristicaTablero(tablero, vehiculo);
            if (heuristicaNueva < mejorHeuristica)
            {
                mejorHeuristica = heuristicaNueva;
                mejorVehiculo = vehiculo;
                mejorMovimiento = mov;
            }
            tablero.setTablero(tableroTemp);
        }
        if (mejorVehiculo != '?')
        {
            moverVehiculo(tablero, mejorMovimiento, false);
            solucion.push_back(mejorVehiculo);
            cout << "Has movido el vehículo " << mejorVehiculo << endl;
            mostrar(tablero);
            cout << "Heurística actual: " << mejorHeuristica << endl;
            cout << endl;
            if (mejorHeuristica == 0)
            {
                break;
            }
        }
        else
        {
            break; // No se encontró un movimiento mejor
        }
        if (mejorHeuristica >= heuristicaActual)
        {
            break;
        }
        iter++;
    }
    cout << "Pasos realizados: ";
    if (solucion.empty())
    {
        cout << "(sin pasos)";
    }
    else
    {
        for (int i = 0; i < solucion.size(); i++)
        {
            cout << solucion[i];
            if (i + 1 < solucion.size())
            {
                cout << ", ";
            }
        }
    }
    cout << endl;

    tablero.agregarSolucion(solucion);
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
    // Estructura para un nodo en la búsqueda A*
    struct Nodo {
        Tablero estado;
        vector<char> camino;
        int g;  // costo desde el inicio (número de pasos)
        int h;  // heurística estimada al objetivo
        
        int f() const { return g + h; }
        
        bool operator>(const Nodo& otro) const {
            return f() > otro.f();  // Para min-heap
        }
    };
    
    // Cola de prioridad ordenada por f(n)
    priority_queue<Nodo, vector<Nodo>, greater<Nodo>> abiertos;
    
    // Conjunto de estados visitados (usando string como representación)
    set<string> cerrados;
    
    // Nodo inicial
    Nodo inicial{tablero, {}, 0, calcularHeuristicaTablero(tablero, '?')};
    
    abiertos.push(inicial);
    
    int iterMax = 10000;
    int iter = 0;
    
    while (!abiertos.empty() && iter < iterMax)
    {
        Nodo actual = abiertos.top();
        abiertos.pop();
        
        cout << "---A* Iteración " << (iter + 1) << " (f=" << actual.f() 
             << ", g=" << actual.g << ", h=" << actual.h << ")---" << endl;
        mostrar(actual.estado);
        cout << endl;
        
        // Verificar si es objetivo
        if (esObjetivo(actual.estado))
        {
            cout << "¡Objetivo encontrado!" << endl;
            tablero = actual.estado;
            cout << "Pasos realizados: ";
            if (actual.camino.empty())
            {
                cout << "(sin pasos)";
            }
            else
            {
                for (int i = 0; i < actual.camino.size(); i++)
                {
                    cout << actual.camino[i];
                    if (i + 1 < actual.camino.size())
                    {
                        cout << ", ";
                    }
                }
            }
            cout << endl;
            tablero.agregarSolucion(actual.camino);
            return actual.camino;
        }
        
        // Crear representación string del estado para control de duplicados
        string estadoKey = "";
        for (const auto& fila : actual.estado.getTablero())
        {
            for (char celda : fila)
            {
                estadoKey += celda;
            }
        }
        
        // Si ya hemos visitado este estado, saltar
        if (cerrados.find(estadoKey) != cerrados.end())
        {
            continue;
        }
        cerrados.insert(estadoKey);
        
        // Expandir: intentar 4 movimientos
        for (char mov : {'u', 'd', 'l', 'r'})
        {
            Nodo sucesor{actual.estado, actual.camino, 0, 0};
            
            char vehiculo = moverVehiculo(sucesor.estado, mov, false);
            
            if (vehiculo != ' ')
            {
                sucesor.camino.push_back(vehiculo);
                sucesor.g = actual.g + 1;
                sucesor.h = calcularHeuristicaTablero(sucesor.estado, vehiculo);
                
                // Crear clave del sucesor para verificar si ya está en cerrados
                string sucesorKey = "";
                for (const auto& fila : sucesor.estado.getTablero())
                {
                    for (char celda : fila)
                    {
                        sucesorKey += celda;
                    }
                }
                
                // Solo agregar si no hemos visitado este estado
                if (cerrados.find(sucesorKey) == cerrados.end())
                {
                    abiertos.push(sucesor);
                }
            }
        }
        
        iter++;
    }
    
    cout << "No se encontró solución después de " << iter << " iteraciones" << endl;
    vector<char> solucion;
    return solucion;  // No encontrado
}

vector<char> calcularAlgoritmoAndOr(Tablero &tablero)
{
    vector<char> solucion;
    // TODO: Implementar algoritmo AND-OR
    return solucion;
}
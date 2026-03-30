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

// ==========================================================================
// FUNCIONES AUXILIARES PARA REFACTORIZACIÓN DE moverVehiculo
// ==========================================================================

/**
 * Obtiene los datos de una dirección de movimiento
 * @param movimiento - carácter que representa la dirección ('u', 'd', 'l', 'r')
 * @return DireccionMovimiento con deltaX, deltaY y descripción
 */
DireccionMovimiento obtenerDatosMovimiento(char movimiento)
{
    switch (movimiento)
    {
    case 'u':
        return {-1, 0, "arriba"};
    case 'd':
        return {1, 0, "abajo"};
    case 'r':
        return {0, 1, "derecha"};
    case 'l':
        return {0, -1, "izquierda"};
    default:
        return {0, 0, "inválida"};
    }
}

/**
 * Realiza la lógica de movimiento de un vehículo en una dirección específica
 * @param tablero - referencia al tablero actual
 * @param tableroRef - copia de referencia del tablero para modificaciones
 * @param posX, posY - posición actual de X
 * @param movimiento - carácter del movimiento
 * @param direccion - estructura con datos de la dirección
 * @return carácter del vehículo movido, o espacio si no se pudo mover
 */
char intentarMoverVehiculo(Tablero &tablero, vector<vector<char>> &tableroRef, 
                           int posX, int posY, char movimiento, 
                           const DireccionMovimiento &direccion,
                           bool mostrarMensajes)
{
    int nuevaX = posX + direccion.deltaX;
    int nuevaY = posY + direccion.deltaY;
    char vehiculo = ' ';
    bool movimientoValido = false;
    vector<pair<int, int>> V;

    // Verificar si la nueva posición está dentro de límites
    if (!dentroLimites(nuevaX, nuevaY))
    {
        if (mostrarMensajes)
        {
            cout << "No se ha podido mover el vehículo hacia " << direccion.descripcion 
                 << " porque se encuentra en el límite del tablero" << endl;
        }
        return vehiculo;
    }

    // Obtener el vehículo en la dirección del movimiento
    vehiculo = tableroRef[nuevaX][nuevaY];
    delimitarVehiculo(tablero, nuevaX, nuevaY, V, vehiculo);

    // Procesar según tamaño del vehículo
    if (V.empty())
    {
        // No hay vehículo adyacente
        movimientoValido = false;
    }
    else if (V.size() == 1)
    {
        // Vehículo de tamaño 1: siempre se puede mover
        tableroRef[nuevaX][nuevaY] = 'X';
        tableroRef[posX][posY] = vehiculo;
        movimientoValido = true;
    }
    else
    {
        // Vehículo de tamaño 2 o 3: verificar alineación
        int coordenadaFija = (movimiento == 'u' || movimiento == 'd') ? posY : posX;
        
        if (puedeMoverse(coordenadaFija, V, movimiento))
        {
            int distancia = V.size();  // El tamaño del vehículo es la distancia a mover
            int destinoX = posX + (direccion.deltaX * (distancia + 1));
            int destinoY = posY + (direccion.deltaY * (distancia + 1));

            if (dentroLimites(destinoX, destinoY))
            {
                tableroRef[destinoX][destinoY] = 'X';
                tableroRef[posX][posY] = vehiculo;
                movimientoValido = true;
            }
        }
    }

    // Mostrar resultado y actualizar tablero
    if (movimientoValido)
    {
        tablero.setTablero(tableroRef);
        if (mostrarMensajes)
        {
            cout << "Has movido el vehículo " << vehiculo << " hacia " << direccion.descripcion << endl;
        }
    }
    else
    {
        if (mostrarMensajes)
        {
            cout << "No se ha podido mover el vehículo " << vehiculo << " hacia " << direccion.descripcion << endl;
            cout << endl;
        }
        vehiculo = ' ';
    }

    return vehiculo;
}

/**
 * Mueve el vehículo X en la dirección especificada
 * Controla el movimiento de vehículos de diferentes tamaños (1-3 celdas)
 * 
 * @param tablero - referencia al tablero actual
 * @param movimiento - dirección del movimiento ('u'=arriba, 'd'=abajo, 'l'=izquierda, 'r'=derecha)
 * @return carácter del vehículo movido, o espacio si no se pudo mover
 */
char moverVehiculo(Tablero &tablero, char movimiento, bool mostrarMensajes)
{
    int posX, posY;
    encontrarX(tablero, posX, posY);
    vector<vector<char>> tableroRef = tablero.getTablero();

    // Obtener información de la dirección
    DireccionMovimiento direccion = obtenerDatosMovimiento(movimiento);

    // Validar movimiento
    if (direccion.deltaX == 0 && direccion.deltaY == 0)
    {
        if (mostrarMensajes)
        {
            cout << "Movimiento no válido, escoja de nuevo" << endl;
        }
        return ' ';
    }

    // Intentar mover el vehículo
    return intentarMoverVehiculo(tablero, tableroRef, posX, posY, movimiento, direccion, mostrarMensajes);
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
    char vehiculo = '?';
    while(!esObjetivo(tablero)){
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
            if (heuristicaNueva <= heuristicaActual)
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
            if (heuristicaNueva <= heuristicaActual)  // Aceptar mejoras o iguales
            {
                heuristicaActual = heuristicaNueva;
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
            cout << "Heurística actual: " << heuristicaActual << endl;
            cout << endl;
            if (heuristicaActual == 0)
            {
                break;
            }
        }
        else
        {
            break; // No se encontró un movimiento mejor
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
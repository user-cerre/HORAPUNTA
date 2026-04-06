#include "operacionesMatriz.h"

using namespace std::filesystem;

const int alfa = 2;

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
    string rutaEntradas = "../Entradas";
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
        if (!(cin >> opcion)) // Si la lectura falla (devuelve false)
    {
        cerr << "Error: Debes introducir un numero." << endl;
        
        cin.clear(); //Limpiar el estado de error de cin
        
        //Descarta lo que haya en el buffer hasta el siguiente salto de línea
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        continue; // Salta al inicio del bucle para volver a preguntar
    }
        if (opcion == 0)
        {
            cout << "Saliendo del programa. ¡Hasta luego!" << endl;
            return;
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
        for (int i = 0; i < solucion.size(); i++) {
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

// int calcularHeuristicaXAZ(Tablero &tablero)
// {
//     int posXdeX, posYdeX, posXdeZ, posYdeZ;
//     encontrarX(tablero, posXdeX, posYdeX);
//     encontrarZ(tablero, posXdeZ, posYdeZ);
//     return abs(posXdeX - (posXdeZ)) + abs((posYdeX - posYdeZ));
// }

// int distanciaZABorde(Tablero &tablero)
// {
//     int posXdeZ, posYdeZ;
//     encontrarZ(tablero, posXdeZ, posYdeZ);
//     int distancia = min(posXdeZ, posYdeZ);
//     if (distancia < 3)
//     {
//         return distancia;
//     }
//     return 5 - distancia;
// }

bool esObjetivo(Tablero tablero)
{
    int xZ = 0;
    int yZ = 0;
    encontrarZ(tablero, xZ, yZ);
    return xZ == 0 || xZ == 5 || yZ == 0 || yZ == 5;
}

int distanciaManhattan(int x1, int y1, int x2, int y2)
{
    return (x1 > x2 ? x1 - x2 : x2 - x1) +
           (y1 > y2 ? y1 - y2 : y2 - y1);
}

int contarBloqueadoresEnLinea(const vector<vector<char>> &tableroAux,
                              int origenX, int origenY,
                              int dX, int dY)
{
    //1º) Primero crea un conjunto que almacenará los vehículos bloqueadores. Como un conjunto sólo guarda elementos
    // únicos, no habrá duplicados aunque un vehículo bloquee la línea en varias posiciones (es decir, que sea más grande
    // de 1x1).
    set<char> bloqueadores;
    //2º) Luego, empieza en la celda inmediatamente siguiente a Z, por eso se le suma dX y dY a las coordenadas de origen, 
    //y va avanzando en la dirección dada por dX y dY (que pueden ser 1, -1 o 0).
    int x = origenX + dX;
    int y = origenY + dY;

    while (dentroLimites(x, y))
    {
        char celda = tableroAux[x][y];
        //3º) Si la celda no es X ni está fuera del tablero, añade ese vehículo al conjunto.
        if (celda != 'X')
        {
            bloqueadores.insert(celda);
        } 
        //4º) Avanza una posición en la misma dirección y repite el proceso hasta que salga del tablero.
        x += dX;
        y += dY;
    }
    //5º) Finalmente, devuelve el número de vehículos bloqueadores, que es el tamaño del conjunto
    return bloqueadores.size();
}

bool estaEnEsquinaDeTablero(int xX, int xY)
{
    return (xX == 0 && xY == 0) || 
    abs(xX - xY) == 5 ||
    (xX == 5 && xY == 5);
}

int calcularHeuristicaTablero(Tablero &tablero)
{
    int zX, zY;
    encontrarZ(tablero, zX, zY);

    if (esObjetivo(tablero))
    {
        return 0;
    }

    int xX, xY;
    encontrarX(tablero, xX, xY);
    //Hasta aquí simplemente hemos guardado las posiciones de Z y X para poder usarlas después en 
    //todos los cálculos de la heurística. También hemos comprobado si Z ya está en una posición objetivo,
    //en ese caso la heurística es 0 porque ya hemos llegado a la solución.
    vector<vector<char>> tableroAux = tablero.getTablero();
    int mejorCoste = INT_MAX;
    //Guardamos una copia del tablero para no modificar el estado del juego al calcular la heurística.
    //También inicializamos una variable que lleve el mejor coste encontrado hasta ahora con un valor máximo, la cual
    //se utilizará para buscar el cote mínimo entre las cuatro direcciones.
    const vector<pair<int, int>> direcciones = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    //Creamos un vector con las cuatro direcciones posibles (arriba, abajo, izquierda, derecha respectivamente) representadas como 
    //cambios en las coordenadas x e y.
    for (auto [dx, dy] : direcciones)
    {
        int pasoX = zX + dx;
        int pasoY = zY + dy;
        if (!dentroLimites(pasoX, pasoY))
        {
            continue;
        }

        int pasosHastaBorde;
        if (dx < 0)
            pasosHastaBorde = zX;
        else if (dx > 0)
            pasosHastaBorde = filas - 1 - zX;
        else if (dy < 0)
            pasosHastaBorde = zY;
        else
            pasosHastaBorde = columnas - 1 - zY;

        int bloqueadores = contarBloqueadoresEnLinea(tableroAux, zX, zY, dx, dy);
        int costoX = tableroAux[pasoX][pasoY] == 'X'
                        ? 0
                        : distanciaManhattan(xX, xY, pasoX, pasoY);

        int ajusteEsquina = estaEnEsquinaDeTablero(xX, xY) ? -1 : 0; 

        int costoDireccion = pasosHastaBorde + bloqueadores * 2 + costoX + ajusteEsquina;
        if(costoDireccion < 0){
            costoDireccion = 0;
        }
        if (costoDireccion < mejorCoste)
        {
            mejorCoste = costoDireccion;
        }
    }

    /*
    Explicación del bucle por pasos: Este bucle for itera sobre las cuatro direcciones posibles. 
    Para cada una de ellas hacemos:
    1º) Se calcula la posición que debe ocupar X para que Z pueda moverse en esa dirección (pasoX, pasoY). 
    Esto se hace sumando el cambio de dirección (dx, dy) a las coordenadas actuales de Z (zX, zY). Si esa
    casilla está fuera del tablero, descartamos esa dirección.
    2º) Se calcula cuántas casillas tiene que recorrer Z para llegar al borde en esa dirección (pasosHastaBorde). 
    Esto depende de la dirección: si es hacia arriba, es igual a la coordenada x de Z, si es hacia abajo, 
    es la diferencia entre el número de filas y la coordenada x de Z, etc.
    3º) Se cuenta cuántos vehículos distintos bloquean la línea desde Z hacia el borde en esa dirección(bloqueadores).
    4º) Se calcula el coste de mover X a la posición necesaria. Si esa posición ya la ocupa X, el coste es 0, si no, 
    se calcula la distancia Manhattan entre la posición actual de X y esa posición.
    5º) Se aplica una bonificación si X está en una esquina del tablero, ya que esto siempre permite el movimiento de 2
    vehículos en lugar de 1, lo cual es muy beneficioso para desbloquear a Z.
    6º) Se calcula el coste total de esa dirección como la suma de los pasos hasta el borde, el doble del número de 
    bloqueadores (se multiplica por 2 para dar más peso al hecho de que desbloquear un vehículo es costoso) y el coste 
    de mover X. Si este coste es menor que el mejor coste encontrado hasta ahora, se actualiza el mejor coste.
    */

    if (mejorCoste == INT_MAX)
    {
        int distanciaMinimaBorde =
            std::min({zX, zY, filas - 1 - zX, columnas - 1 - zY});
        int distanciaXZ = distanciaManhattan(xX, xY, zX, zY);
        return distanciaMinimaBorde + distanciaXZ + 1;
    }

    //Esta parte es sólo en caso de que no se encuentre ninguna dirección válida (muy improbable).
    //En este caso se calcula una heurística de emergencia que es la suma de la distancia de Z al borde más cercano, 
    //la distancia de X a Z y un pequeño valor adicional para penalizar esta situación.

    return mejorCoste;
}

vector<char> calcularEscaladaSimple(Tablero &tablero){
    cout << "Calculando solución con el método de Escalada Simple..." << endl;
    auto inicio = chrono::high_resolution_clock::now();
    vector<char> solucion;
    int numNodosGenerados = 0;
    int iterMax = 200;
    int iter = 0;

    while (!esObjetivo(tablero) && iter < iterMax) {
        int heuristicaActual = calcularHeuristicaTablero(tablero);
        char mejorVehiculo = ' ';
        char mejorMovimiento = ' ';
        int mejorHeuristica = heuristicaActual;

        for (char mov : {'u', 'd', 'l', 'r'}) {
            numNodosGenerados++;
            Tablero temp = tablero;
            char vehiculo = moverVehiculo(temp, mov, false);
            if (vehiculo == ' ') {
                continue;
            }

            mejorHeuristica = calcularHeuristicaTablero(temp);
            if (mejorHeuristica < heuristicaActual) {
                tablero = temp;
                mejorVehiculo = vehiculo;
                mejorMovimiento = mov;
                break; //En el momento que encontramos un estado con una heurística mejor que la actual, 
                       //salimos del bucle para no probar ningún otro estado, cosa que caracteriza a la escalada simple.
            }
        }

        if (mejorVehiculo == ' ') {
            break;
        }

        solucion.push_back(mejorVehiculo);

        cout << "Has movido el vehículo " << mejorVehiculo
             << " (" << mejorMovimiento << ")" << endl;
        mostrar(tablero);
        cout << "Heurística actual: " << mejorHeuristica << endl;
        cout << endl;

        if (mejorHeuristica == 0) {
            break;
        }

        iter++;
    }

    if(numNodosGenerados == 0){
        solucion.push_back(' ');
    }

    mostrarSolucionTablero(solucion, calcularHeuristicaTablero(tablero));
    auto fin = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio);
    cout << "Tiempo de ejecución: " << duracion.count() << " µs" << endl;
    cout << "Número de nodos generados: " << numNodosGenerados << endl;

    tablero.agregarSolucion(solucion);
    return solucion;
}

vector<char> calcularEscaladaMaximaPendiente(Tablero &tablero) {
    cout << "Calculando solución con el método de Escalada de Máxima Pendiente..." << endl;
    auto inicio = chrono::high_resolution_clock::now();
    vector<char> solucion;
    int numNodosGenerados = 0;
    int iterMax = 200;
    int iter = 0;

    while (!esObjetivo(tablero) && iter < iterMax) {
        int heuristicaActual = calcularHeuristicaTablero(tablero);
        int mejorHeuristica = heuristicaActual;
        char mejorVehiculo = ' ';
        char mejorMovimiento = ' ';
        Tablero mejorTablero = tablero;

        for (char mov : {'u', 'd', 'l', 'r'}) {
            numNodosGenerados++;
            Tablero temp = tablero;
            char vehiculo = moverVehiculo(temp, mov, false);
            if (vehiculo == ' ') {
                continue;
            }

            int heuristicaNueva = calcularHeuristicaTablero(temp);
            if (heuristicaNueva < mejorHeuristica) {
                mejorHeuristica = heuristicaNueva;
                mejorVehiculo = vehiculo;
                mejorMovimiento = mov;
                mejorTablero = temp;
            }
        }

        if (mejorVehiculo == ' ') {
            break;
        }

        tablero = mejorTablero;
        solucion.push_back(mejorVehiculo);

        cout << "Has movido el vehículo " << mejorVehiculo
             << " (" << mejorMovimiento << ")" << endl;
        mostrar(tablero);
        cout << "Heurística actual: " << mejorHeuristica << endl;
        cout << endl;

        if (mejorHeuristica == 0) {
            break;
        }

        iter++;
    }

    if(numNodosGenerados == 0){
        solucion.push_back(' ');
    }

    mostrarSolucionTablero(solucion, calcularHeuristicaTablero(tablero));
    auto fin = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio);
    cout << "Tiempo de ejecución: " << duracion.count() << " µs" << endl;
    cout << "Número de nodos generados: " << numNodosGenerados << endl;

    tablero.agregarSolucion(solucion);
    return solucion;
}

vector<char> calcularEscaladaSimpleEstocastica(Tablero &tablero)
{
    cout << "Calculando solución con el método de Escalada Simple Estocástica..." << endl;
    auto inicio = chrono::high_resolution_clock::now();
    vector<char> solucion;
    int numNodosGenerados = 0;
    int iterMax = 200;
    int iter = 0;
    
    //generar random
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine motor(seed);

    while (!esObjetivo(tablero) && iter < iterMax) {
        int heuristicaActual = calcularHeuristicaTablero(tablero);
        char mejorVehiculo = ' ';
        char mejorMovimiento = ' ';
        int mejorHeuristica = heuristicaActual;

        //Aleatorizamos el orden de las direcciones para que no siempre se prueben en el mismo orden, lo cual haría que el algoritmo se comporte igual cada vez.
        vector<char> direcciones = {'u', 'd', 'l', 'r'};
        shuffle(direcciones.begin(), direcciones.end(), motor);

        for (char mov : direcciones) {
            numNodosGenerados++;
            Tablero temp = tablero;
            char vehiculo = moverVehiculo(temp, mov, false);
            if (vehiculo == ' ') {
                continue;
            }
            
            int heuristicaNueva = calcularHeuristicaTablero(temp);
            int mejora = heuristicaActual - heuristicaNueva;
            if (mejora >= alfa) { //Comparamos el estado actual con alfa, característico de la escalada simple estocástica.
                mejorHeuristica = heuristicaNueva;
                tablero = temp;
                mejorVehiculo = vehiculo;
                mejorMovimiento = mov;
                break; //En el momento que encontramos un estado con una heurística mejor que la actual, 
                       //salimos del bucle para no probar ningún otro estado, cosa que caracteriza a la escalada simple.
            }
        }

        if (mejorVehiculo == ' ') {
            break;
        }

        solucion.push_back(mejorVehiculo);

        cout << "Has movido el vehículo " << mejorVehiculo
             << " (" << mejorMovimiento << ")" << endl;
        mostrar(tablero);
        cout << "Heurística actual: " << mejorHeuristica << endl;
        cout << endl;

        if (mejorHeuristica == 0) {
            break;
        }

        iter++;
    }

    if(numNodosGenerados == 0){
        solucion.push_back(' ');
    }

    mostrarSolucionTablero(solucion, calcularHeuristicaTablero(tablero));
    auto fin = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio);
    cout << "Tiempo de ejecución: " << duracion.count() << " µs" << endl;
    cout << "Número de nodos generados: " << numNodosGenerados << endl;

    tablero.agregarSolucion(solucion);
    return solucion;
}

Nodo * comprobarEnVectores(vector<Nodo*> &abiertos, vector<Nodo*> &cerrados, Nodo *nuevoNodo){
    for(Nodo *nodoAbierto : abiertos){
        if(nodoAbierto->getEstado().getTablero() == nuevoNodo->getEstado().getTablero()){
            return nodoAbierto;
        }
    }
    for(Nodo *nodoCerrado : cerrados){
        if(nodoCerrado->getEstado().getTablero() == nuevoNodo->getEstado().getTablero()){
            return nodoCerrado;
        }
    }
    return nullptr;
}

void actualizarDescendientesRecursivo(vector<Nodo*> &cerrados, Nodo* nodoMejorado) {
    for (Nodo *descendiente : cerrados) {
        if (descendiente->getPadre() == nodoMejorado) {
            int nuevoCoste = nodoMejorado->getCoste() + 1;
            if (nuevoCoste < descendiente->getCoste()) {
                descendiente->setCoste(nuevoCoste);
                descendiente->setPadre(nodoMejorado);
                actualizarDescendientesRecursivo(cerrados, descendiente);
            }
        }
    }
}

void reconstruirSolucionAEstrella(Nodo* nodo, vector<char> &solucion){
    while(nodo != nullptr && nodo->getPadre() != nullptr){
        solucion.insert(solucion.begin(), nodo->getMovimiento());
        nodo = nodo->getPadre();
    }
}

vector<char> calcularAlgoritmoAEstrella(Tablero &tablero)
{
    cout << "Calculando solución con el método de Algoritmo A*..." << endl;
    auto inicio = chrono::high_resolution_clock::now();

    vector<char> solucion;
    int numNodosGenerados = 0;
    int iterMax = 200;
    int iter = 0;
    bool objetivoEncontrado = false;

    vector<Nodo*> abiertos;
    Nodo* raiz = new Nodo(tablero, nullptr, calcularHeuristicaTablero(tablero), 0);
    abiertos.push_back(raiz);
    vector<Nodo*> cerrados;

    while(!abiertos.empty() && iter < iterMax && !objetivoEncontrado){

        Nodo *nodoActual = abiertos.front();
        abiertos.erase(abiertos.begin());

        tablero = nodoActual->getEstado();

        cerrados.push_back(nodoActual);

        cout << "Expandiendo nodo con f'= " << nodoActual->getValor() 
        << " (g = " << nodoActual->getCoste() 
        << ", h = " << nodoActual->getHeuristica() << ")";

        if(nodoActual->getCoste() == 0){
            cout << ". Estado Origen ";
        }

        cout << endl;

        if(esObjetivo(tablero)){
            cout << "Tablero resuelto: " << endl;
            mostrar(tablero);
            cout << "Heurística: " << calcularHeuristicaTablero(tablero) << endl;
            reconstruirSolucionAEstrella(nodoActual, solucion);
            objetivoEncontrado = true;
        }
        
        if(!objetivoEncontrado){
            mostrar(tablero);

            for(char mov : {'u', 'd', 'l', 'r'}){
                numNodosGenerados++;

                Tablero temp = tablero;
                char vehiculo = moverVehiculo(temp, mov, false);

                if(vehiculo == ' '){
                    continue;
                }

                if(nodoActual->getCoste() == 0){
                    raiz->setMovimiento(vehiculo);
                }

                int heuristicaNueva = calcularHeuristicaTablero(temp);
                int costeNuevo = nodoActual->getCoste() + 1;

                Nodo* nuevoNodo = new Nodo(temp, nodoActual, heuristicaNueva, costeNuevo);
                nuevoNodo->setMovimiento(vehiculo);
                Nodo* nodoEncontrado = comprobarEnVectores(abiertos, cerrados, nuevoNodo);

                if(nodoEncontrado != nullptr){
                    int fAntiguo = nodoEncontrado->getValor();
                    int fNuevo = nodoEncontrado->getValor();

                    cout << "Nodo duplicado: f'antigua = " << fAntiguo 
                     << ", f'nueva = " << fNuevo << " moviendo el vehículo " 
                     << nuevoNodo->getMovimiento();

                    if(fNuevo < fAntiguo){

                        cout << " | f' Mejorada" << endl;

                        nodoEncontrado->setHeuristica(heuristicaNueva);
                        nodoEncontrado->setCoste(costeNuevo);
                        nodoEncontrado->setPadre(nodoActual);

                        bool estaEnCerrados = false;
                        for(Nodo *nodoCerrado : cerrados){
                            if(nodoCerrado == nodoEncontrado){
                                estaEnCerrados = true;
                                break;
                            }
                        }

                        if(estaEnCerrados){
                            actualizarDescendientesRecursivo(cerrados,nodoEncontrado);
                        }
                    } else {
                        cout << " | f' No Mejorada " << endl;
                    }
                } else {
                    cout << "Nuevo nodo: f'= " << nuevoNodo->getValor() 
                     << " (g = " << costeNuevo << ", h = " << heuristicaNueva << ")" 
                     << " moviendo el vehículo " << nuevoNodo->getMovimiento() << endl;

                    abiertos.push_back(nuevoNodo);
                }
                sort(abiertos.begin(), abiertos.end(), [](const Nodo *a, const Nodo *b) {
                return a->getValor() < b->getValor();
                });
            }
            cout << "ABIERTOS: " << abiertos.size() << " | CERRADOS: " << cerrados.size() << endl;
            cout << endl;

            iter++;
        }
    }

    if(numNodosGenerados == 0){
        solucion.push_back(' ');
    }

    mostrarSolucionTablero(solucion, calcularHeuristicaTablero(tablero));
    auto fin = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio);
    cout << "Tiempo de ejecución: " << duracion.count() << " µs" << endl;
    cout << "Número de nodos generados: " << numNodosGenerados << endl;

    tablero.agregarSolucion(solucion);
    return solucion;
}
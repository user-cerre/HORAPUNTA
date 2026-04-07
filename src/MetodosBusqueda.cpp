#include "MetodosBusqueda.h"

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
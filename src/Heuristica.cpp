#include "Heuristica.h"

const int alfa = 2;

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
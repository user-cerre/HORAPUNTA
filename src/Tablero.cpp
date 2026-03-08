#include "Tablero.h"

Tablero::Tablero(int filas, int columnas) : filas(filas), columnas(columnas)
{
    tablero.resize(filas, vector<char>(columnas, ' ')); // Inicializa el tablero con espacios vacíos
    solucionesEncontradas = 0;                          // Inicializa el contador de soluciones encontradas
    visitados.resize(4, false);                         // Inicializa el vector de visitados para los movimientos (UP, DOWN, LEFT, RIGHT)
}

void Tablero::setTablero(const vector<vector<char>> &nuevoTablero)
{
    if (nuevoTablero.size() == filas && nuevoTablero[0].size() == columnas)
    {
        tablero = nuevoTablero;
    }
    else
    {
        cerr << "Error: El tamaño del nuevo tablero no coincide con las dimensiones del tablero actual." << endl;
    }
}

const vector<vector<char>> &Tablero::getTablero() const
{
    return tablero;
}

vector<vector<char>> Tablero::getSoluciones() const
{
    return soluciones;
}

void Tablero::agregarSolucion(const vector<char> &movimientos)
{
    soluciones.push_back(movimientos); // Agrega la nueva solución al vector de soluciones
    solucionesEncontradas++;           // Incrementa el contador de soluciones encontradas
}

const vector<bool> &Tablero::getVisitados()
{
    return visitados;
}

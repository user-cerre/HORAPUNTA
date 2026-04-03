#include <iostream>
#include <vector>
#include <limits.h>
#include <map>
#include <algorithm>
#include <fstream>
#include <filesystem>

using namespace std;
using namespace filesystem;
namespace fs = std::filesystem;

#ifndef TABLERO_H
#define TABLERO_H
/*
*
*/
class Tablero
{
        private:
            vector<vector<char>> tablero;
        int filas;
        int columnas;
        vector<vector<char>> soluciones; // Vector para almacenar las soluciones encontradas. Por cada solucio se guarda los movimientos realizados para llegar a ella.
        int solucionesEncontradas;       // Contador para llevar la cuenta de las soluciones encontradas. Tambien usado para saber donde guardar la siguiente lista de valores en el vector de soluciones.
        vector<bool> visitados;          // UP DOWN LEFT RIGHT
public:
        /*
        *
        */
        Tablero(int filas, int columnas);
        /*
        *
        */
        void setTablero(const vector<vector<char>> &nuevoTablero);
        /*
        *
        */
        const vector<vector<char>> &getTablero() const;
        /*
        *
        */
        vector<vector<char>> getSoluciones() const;
        /*
        *
        */
        void agregarSolucion(const vector<char> &movimientos);
        /*
        *
        */
        const vector<bool> &getVisitados();
    };  
#endif
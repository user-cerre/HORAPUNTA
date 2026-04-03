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
/**
 * @brief representa el tablero por el que se hacen las operaciones
 * 
 * Esta clase almacena la matriz de caracteres del tablero,
 * el número de filas, el número de columnas, los tableros solución, 
 * el número de soluciones encontradas y las posiciones posibles que 
 * han sido ya visitadas.
 * 
 */
class Tablero
{
        private:
        vector<vector<char>> tablero;   ///< Matriz de caracteres en donde se contiene la información del tablero.
        int filas;  ///< Número de filas del tablero.
        int columnas;   ///< Número de columnas del tablero
        vector<vector<char>> soluciones;    ///< Vector de soluciones, donde cada solución es una secuencia de movimientos.
        int solucionesEncontradas;  ///< Contador para llevar la cuenta de las soluciones encontradas. Tambien usado para saber donde guardar la siguiente lista de valores en el vector de soluciones.
        vector<bool> visitados; ///< UP DOWN LEFT RIGHT
public:
        /**
         * @brief Constructor parametrizado de la clase Tablero
         * 
         * @param filas Número de filas.
         * @param columnas Número de columnas.
         */
        Tablero(int filas, int columnas);
        /**
         * @brief Obtiene la matriz del tablero actual.
         * @return Referencia constante a la matriz de caracteres del tablero.
         */
        const vector<vector<char>> &getTablero() const;
        /**
         * @brief Obtiene los pasos para llegar a las soluciones encontradas.
         * @return Matriz de soluciones.
         */
        vector<vector<char>> getSoluciones() const;
        /**
         * @brief Obtiene los pasos usados por el tablero.
         * @return Referencia constante al vector de posiciones visitadas(UP DOWN LEFT RIGHT).
         */
        const vector<bool> &getVisitados();
        /**
         * @brief Añade los movimientos para llegar a solución.
         * @param movimientos Vector de movimientos uadso para llegar a una solución válida
         */
        void agregarSolucion(const vector<char> &movimientos);
        /**
         * @brief Establece la matriz del tablero.
         * @param nuevoTablero Nuevo matriz del tablero.
         */
        void setTablero(const vector<vector<char>> &nuevoTablero);
        
    };  
#endif
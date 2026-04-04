#include <iostream>
#include "Tablero.h"
using namespace std;

/**
* @brief representa un nodo dentro de la estructura de búsqueda
*
* Esta clase almacena el estado actual del tablero, 
* un puntero el nodo padre, la heuristica y el coste acumulado para llegar a ese estado.
*/
class Nodo {
    private:
        Tablero estado;
        Nodo* padre;    ///< Puntero al nodo padre, utilizado para reconstruir el camino desde el nodo raíz hasta el nodo actual.
        int heuristica; ///< Valor heurístico que estima el costo restante para alcanzar el objetivo desde el nodo actual.
        int coste;  ///< Coste acumulado hasta dicho nodo.
        char movimiento;   ///< Vehículo movido en el tablero para llegar a este estado.
    public:
        /**
        * @brief Constructor parametrizado de la clase Nodo.
        *
        * @param t Estado del tablero.
        * @param p Puntero al nodo padre.
        * @param h Valor heurístico actual.
        * @param c Coste acumulado inicial
        */
        Nodo(Tablero t, Nodo* p, int h, int c);
        /**
        * @brief Obtiene el estado del nodo.
        * @return Tablero Estado actual.
        */
        Tablero getEstado() const;
        /**
        * @brief Obtiene el predecesor del nodo.
        * @return Puntero al objeto nodo padre.
        */
        Nodo* getPadre() const;
        /**
        * @brief Obtiene la heurística actual del nodo.
        * @return Valor actual del valor heurístico.
        */
        int getHeuristica() const;
        /**
        * @brief Obtiene el coste acumulado del nodo.
        * @return Valor del coste acumulado.
        */
        int getCoste() const;
        /**
         * @brief Obtiene el vehículo movido para llegar a ese estado.
         * @return Letra identificativa del vehículo.
         */
        char getMovimiento() const;
        /**
        * @brief Establece el estado del nodo.
        * @param t Nuevo estado del tablero.
        */
        void setEstado(Tablero t);
        /**
        * @brief Establece el padre del nodo.
        * @param p Nuevo Padre del nodo.
        */
        void setPadre(Nodo* p); 
        /** 
        * @brief Establece la heurística del nodo
        * @param h Nueva heurística del tablero.
        */
        void setHeuristica(int h);
        /**
        * @brief Establece el coste acumulado del nodo.
        * @param c Nuevo coste acumulado.
        */
        void setCoste(int c);
        /**
        * @brief Obtiene el valor del nodo.
        *
        * Utilizado en algoritmos de búsqueda informada (como A*),
        * donde el valor se define como la suma del coste acumulado
        * y la heurística.
        * @return int valor actual 
        */
        int getValor() const;
        /**
         * @brief Establece el vehículo movido para llegar a este estado.
         * @param m Letra identificativa del vehículo.
         */
        void setMovimiento(char m);
        /**
        * @brief Compara dos nodos.
        *
        * Dos nodos osn considerados iguales si sus estados son equivalentes.
        * @param n Nodo a comparar
        * @return true Si los nodos son iguales.
        * @return false En caso contrario.
        */
        bool operator==(const Nodo& n) const;

};
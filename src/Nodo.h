#include <iostream>
#include "Tablero.h"
using namespace std;

/*
*@brief representa un nodo dentro de la estructura de busqueda
*
*Esta clase almacena el estado actual del tablero, 
*un puntero el nodo padre, la heuristica y el coste acumulado para llegar a ese estado.
*/
class Nodo {
    private:
        Tablero estado;
        Nodo* padre;    ///< Puntero al nodo padre, utilizado para reconstruir el camino desde el nodo raíz hasta el nodo actual.
        int heuristica; ///< Valor heurístico que estima el costo restante para alcanzar el objetivo desde el nodo actual.
        int coste;  ///< Coste acumulado hata dicho nodo.
    public:
        /*
        *@brief Constructor parametrizado de la clase Nodo
        *
        * @param t Estado del tablero.
        * @param p Puntero al nodo padre.
        * @param h Valor heurístico atual.
        * @param c Coste acumulado inicial
        */
        Nodo(Tablero t, Nodo* p, int h, int c);
        /*
        *
        */
        Tablero getEstado() const;
        /*
        *
        */
        Nodo* getPadre() const;
        /*
        *
        */
        int getHeuristica() const;
        /*
        *
        */
        int getCoste() const;
        /*
        *
        */
        void setEstado(Tablero t);
        /*
        *
        */
        void setPadre(Nodo* p); 
        /*
        *
        */
        void setHeuristica(int h);
        /*
        *
        */
        void setCoste(int c);
        /*
        *
        */
        int getValor() const;
        /*
        *
        */
        bool operator==(const Nodo& otro) const;

};
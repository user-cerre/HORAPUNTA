#include <iostream>
#include "Tablero.h"
using namespace std;

class Nodo {
    private:
        Tablero estado;
        Nodo* padre;
        int heuristica;
        int coste;
    public:
        Nodo(Tablero t, Nodo* p, int h, int c);

        Tablero getEstado();

        Nodo* getPadre();

        int getHeuristica();

        int getCoste();

        void setEstado(Tablero t);

        void setPadre(Nodo* p); 

        void setHeuristica(int h);

        void setCoste(int c);

        int getValor();

        bool operator==(const Nodo& otro) const;

};
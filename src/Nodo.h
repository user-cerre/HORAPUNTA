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

        Tablero getEstado() const;

        Nodo* getPadre() const;

        int getHeuristica() const;

        int getCoste() const;

        void setEstado(Tablero t);

        void setPadre(Nodo* p); 

        void setHeuristica(int h);

        void setCoste(int c);

        int getValor() const;

        bool operator==(const Nodo& otro) const;

};
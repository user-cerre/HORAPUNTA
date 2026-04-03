#include "Nodo.h"

Nodo::Nodo(Tablero t, Nodo* p, int h, int c)
    : estado(t), padre(p), heuristica(h), coste(c)
{
}

Tablero Nodo::getEstado() const {
    return estado;
}

Nodo* Nodo::getPadre() const {
    return padre;
}

int Nodo::getHeuristica() const {
    return heuristica;
}

int Nodo::getCoste() const {
    return coste;
}

void Nodo::setEstado(Tablero t){
    estado = t;
}

void Nodo::setPadre(Nodo* p){
    padre = p;
}

void Nodo::setHeuristica(int h){
    heuristica = h;
}

void Nodo::setCoste(int c){
    coste = c;
}

int Nodo::getValor() const {
    return heuristica + coste;
}

bool Nodo::operator==(const Nodo& n) const {
    const auto& tab1 = this->estado.getTablero();
    const auto& tab2 = n.estado.getTablero();
    
    if (tab1.size() != tab2.size()) return false;
    
    for (size_t i = 0; i < tab1.size(); ++i) {
        if (tab1[i].size() != tab2[i].size()) return false;
        for (size_t j = 0; j < tab1[i].size(); ++j) {
            if (tab1[i][j] != tab2[i][j]) return false;
        }
    }
    
    return true;
}
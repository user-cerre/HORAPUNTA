#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include "operacionesMatriz.h"
using namespace std; 

int main(){ 
    vector<vector<char>> tablero(filas, vector<char>(columnas));
    cargarTablero(tablero);
    mostrar(tablero);

    return 0; 
}

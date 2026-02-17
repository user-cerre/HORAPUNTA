#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

using namespace std;
#ifndef OPERACIONESMATRIZ_H
#define OPERACIONESMATRIZ_H

int const filas = 6; //variables globales
int const columnas = 12;  //variables globales son 12 columnas porque hay que tener en cuenta también las posiciones de las comas.
void mostrar(const vector<vector<char>> &tablero);

void cargarTablero(vector<vector<char>> &tablero);


#endif
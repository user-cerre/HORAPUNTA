#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <filesystem>


using namespace std;
using namespace filesystem;
namespace fs = std::filesystem;

#ifndef OPERACIONESMATRIZ_H
#define OPERACIONESMATRIZ_H

int const filas = 6; //variables globales
int const columnas = 12;  //variables globales son 12 columnas porque hay que tener en cuenta también las posiciones de las comas.

void mostrar(const vector<vector<char>> &tablero);

void cargarTablero(vector<vector<char>> &tablero);

void encontrarX(vector<vector<char>> &tablero, int &posX, int &posY);

bool dentroLimites(int x, int y);

void delimitarVehiculo(vector<vector<char>> tablero, int x, int y, vector<pair<int,int>> V);

bool puedeMoverse(vector<vector<char>> tablero, int coordenadaDeX, vector<pair<int,int>> V);

void movimiento(vector<vector<char>> &tablero, char movimiento);

#endif
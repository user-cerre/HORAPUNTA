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

#ifndef OPERACIONESMATRIZ_H
#define OPERACIONESMATRIZ_H

int const filas = 6; //variables globales
int const columnas = 6;  //variables globales son 12 columnas porque hay que tener en cuenta también las posiciones de las comas.

void mostrar(const vector<vector<char>> &tablero);

void cargarTablero(vector<vector<char>> &tablero);

void encontrarX(vector<vector<char>> &tablero, int &posX, int &posY);

void encontrarZ(vector<vector<char>> &tablero, int &posX, int &posY);

bool dentroLimites(int x, int y);

void delimitarVehiculo(const vector<vector<char>> &tablero, int x, int y, vector<pair<int, int>> &V, char vehiculo,
					   vector<vector<char>> &visitados);

bool puedeMoverse(int coordenadaDeX, vector<pair<int, int>> V, char mov);

char moverVehiculo(vector<vector<char>> &tablero, char movimiento);

int calcularHeuristicaXAZ(vector<vector<char>> &tablero);

int distanciaZABorde(vector<vector<char>> &tablero);

int calcularHeuristicaTablero(vector<vector<char>> &tablero, char vehiculo);

vector<char> calcularEscaladaSimple(vector<vector<char>> &tablero);

vector<char> calcularEscaladaMaximaPendiente(vector<vector<char>> &tablero);

vector<char> calcularEscaladaSimpleEstocastica(vector<vector<char>> &tablero);

vector<char> calcularAlgoritmoEstrella(vector<vector<char>> &tablero);

vector<char> calcularAlgoritmoAndOr(vector<vector<char>> &tablero);

#endif
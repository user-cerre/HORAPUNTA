#ifndef OPERACIONESMATRIZ_H
#define OPERACIONESMATRIZ_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>
#include <filesystem>
#include <set>
#include <queue>
#include <chrono>
#include "Tablero.h"

using namespace std;
namespace fs = std::filesystem;

extern const int filas;
extern const int columnas;
extern const int alfa;  // Valor umbral para la escalada simple estocástica.

extern vector<vector<bool>> Visitados; 
/*
*
*/
void mostrar(Tablero &tablero);
/*
*
*/
void cargarTablero(Tablero &tablero);
/*
*
*/
void mostrarSolucionTablero(vector<char> solucion);
/*
*
*/
void encontrarX(Tablero &tablero, int &posX, int &posY);
/*
*
*/
void encontrarZ(Tablero &tablero, int &posX, int &posY);
/*
*
*/
bool dentroLimites(int x, int y);
/*
*
*/
void delimitarVehiculo(Tablero &tablero, int x, int y, vector<pair<int, int>> &V, char vehiculo);
/*
*
*/
bool puedeMoverse(int coordenadaDeX, vector<pair<int, int>> V, char mov);
/*
*
*/
char moverVehiculo(Tablero &tablero, char movimiento, bool mostrarMensajes);
/*
*
*/
bool esObjetivo(Tablero &tablero);
/*
*
*/
int distanciaManhattan(int x1, int y1, int x2, int y2);
/*
*
*/
//Cuenta los vehículos que bloquean la línea desde Z hacia un borde.
int contarBloqueadoresEnLinea(const vector<vector<char>> &tableroAux,
                              int origenX, int origenY,
                              int dX, int dY);
/*
*
*/
bool estaEnEsquinaDeTablero(int xX, int xY);
/*
*
*/
//int calcularHeuristicaXAZ(Tablero &tablero);
/*
*
*/
//int distanciaZABorde(Tablero &tablero);
/*
*
*/
/*
Esta heurística intenta valorar la mejor dirección de salida para Z, de la siguiente manera:
    - Cuanto más cerca esté Z de un borde, menor es el coste.
    - Cuantos más vehículos bloqueen la línea, mayor es el coste.
    - Si X está lejos de la casilla necesaria para mover Z (a su alrededor), mayor es el coste. 
*/
int calcularHeuristicaTablero(Tablero &tablero);
/*
*
*/
vector<char> calcularEscaladaSimple(Tablero &tablero);
/*
*
*/
vector<char> calcularEscaladaMaximaPendiente(Tablero &tablero);
/*
*
*/
vector<char> calcularEscaladaSimpleEstocastica(Tablero &tablero);
/*
*
*/
vector<char> calcularAlgoritmoEstrella(Tablero &tablero);

#endif
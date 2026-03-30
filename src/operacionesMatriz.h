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


using namespace std;
namespace fs = std::filesystem;

#include "Tablero.h"

// 2. IMPORTANTE: Traer las constantes globales
extern const int filas;
extern const int columnas;

// 3. Corregir la matriz global (debe ser bool para coincidir con tu lógica)
extern vector<vector<bool>> Visitados; 

// A partir de aquí, tus funciones ya no deberían marcar error en 'Tablero'
void mostrar(Tablero &tablero);

void cargarTablero(Tablero &tablero);

void encontrarX(Tablero &tablero, int &posX, int &posY);

void encontrarZ(Tablero &tablero, int &posX, int &posY);

bool dentroLimites(int x, int y);

void delimitarVehiculo(Tablero &tablero, int x, int y, vector<pair<int, int>> &V, char vehiculo,
					   vector<vector<bool>> &visitados);

bool puedeMoverse(int coordenadaDeX, vector<pair<int, int>> V, char mov);

// Estructura para encapsular información de una dirección de movimiento
struct DireccionMovimiento {
    int deltaX;
    int deltaY;
    const char* descripcion;
};

// Funciones auxiliares para moverVehiculo
DireccionMovimiento obtenerDatosMovimiento(char movimiento);
char intentarMoverVehiculo(Tablero &tablero, vector<vector<char>> &tableroRef, 
                           int posX, int posY, char movimiento, 
                           const DireccionMovimiento &direccion,
                           bool mostrarMensajes = true);

char moverVehiculo(Tablero &tablero, char movimiento, bool mostrarMensajes = true);

int calcularHeuristicaXAZ(Tablero &tablero);

int distanciaZABorde(Tablero &tablero);

int calcularHeuristicaTablero(Tablero &tablero, char vehiculo);

vector<char> calcularEscaladaSimple(Tablero &tablero);

vector<char> calcularEscaladaMaximaPendiente(Tablero &tablero);

vector<char> calcularEscaladaSimpleEstocastica(Tablero &tablero);

vector<char> calcularAlgoritmoEstrella(Tablero &tablero);

vector<char> calcularAlgoritmoAndOr(Tablero &tablero);

#endif
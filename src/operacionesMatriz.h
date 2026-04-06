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
#include <random>
#include "Nodo.h"
#include "Tablero.h"

using namespace std;
namespace fs = std::filesystem;

extern const int filas;
extern const int columnas;
extern const int alfa;  // Valor umbral para la escalada simple estocástica.

extern vector<vector<bool>> Visitados; 
/**
 * @brief Escribe por pantalla el tablero.
 * @param tablero Referencia al objeto de la clase tablero.
 */

void mostrar(Tablero &tablero);
/**
 * @brief Sobreescribe el valor del objeto de la clase Tablero.
 * @param tablero ReferenciA al objeto de la clase Tablero.
 * @return true Si el tablero se cargó correctamente.
 * @return false En caso contrario.
 */
bool cargarTablero(Tablero &tablero);
/**
 * @brief Escribe por pantalla los pasos para encontrar las soluciones.
 * @param solucion Vector de caracteres en donde se han guardado los pasos para llegar a la solución.
 */
void mostrarSolucionTablero(vector<char> solucion, int heuristica);
/**
 * @brief Devuelve las posiciones X e Y de X en el tablero.
 * 
 * Este método recorre todo el tablero que  ha sido pasado por referncia 
 * y busca X. Si existe dicho valor, devuelve la referencia a sus posiciones X e Y.
 * 
 * @param tablero Referencia al tablero donde se busca X.
 * @param posX Posicion X donde se encuentra X.
 * @param posY Posición Y donde se encuentra X.
 */
void encontrarX(Tablero &tablero, int &posX, int &posY);
/**
 * @brief Devuelve las posiciones X e Y de Z en el tablero.
 * 
 * Este método recorre todo el tablero que  ha sido pasado por referncia 
 * y busca Z. Si existe dicho valor, devuelve la referencia a sus posiciones X e Y.
 * 
 * @param tablero Referencia al tablero donde se busca Z.
 * @param posX Posicion X donde se encuentra Z.
 * @param posY Posición Y donde se encuentra Z.
 */
void encontrarZ(Tablero &tablero, int &posX, int &posY);
/**
 * @brief Verifica que el tablero tiene el tamaño esperado.
 * @param x Número de columnas.
 * @param y Número de filas.
 * @return true Si x e y son menores que el tamaño máximo del tablero.
 * @return flase En caso contrario.
 */
bool dentroLimites(int x, int y);
/**
 * @brief Delimita la posición del vehículo en todas sus casillas ocupadas.
 * 
 * Este método recorre de forma recursiva y comprueba las posiciones del vehículo 
 * con el carácter especificado
 * 
 * @param tablero Referencia al objeto tablero en donde se busca el vehículo.
 * @param x Posición X de la casilla del vehículo.
 * @param y Posición Y de la casilla del vehículo.
 * @param V Vector de pares de enteros donde se guardan las coordenadas de las casillas ocupadas por el vehículo.
 * @param vehiculo Carácter que representa el vehículo a delimitar.
 */
void delimitarVehiculo(Tablero &tablero, int x, int y, vector<pair<int, int>> &V, char vehiculo);
/**
 * @brief Verifica si un vehículo puede moverse en una dirección dada.
 * @param coordenadaDeX Coordenada X de la posición desde donde se intenta mover el vehículo.
 * @param V Vector de pares de enteros donde se guardan las coordenadas de las casillas ocupadas por el vehículo.
 * @param mov Carácter que representa la dirección en la que se intenta mover el vehículo.
 * @return true Si el vehículo puede moverse en la dirección dada.
 * @return false En caso contrario.
 */
bool puedeMoverse(int coordenadaDeX, vector<pair<int, int>> V, char mov);
/**
 * @brief Mueve un vehículo en el tablero.
 * @param tablero Referencia al objeto tablero en donde se encuentra el vehículo.
 * @param movimiento Carácter que representa el movimiento a realizar.
 * @param mostrarMensajes Booleano que indica si se deben mostrar mensajes de error.
 * @return El carácter que representa el vehículo movido. Si el movimiento no es válido, devuelve un espacio en blanco.
 */
char moverVehiculo(Tablero &tablero, char movimiento, bool mostrarMensajes);
/**
 * @brief Verifica si el tablero es un estado objetivo.
 * @param tablero Referencia al objeto tablero.
 * @return true Si el tablero es un estado objetivo.
 * @return false En caso contrario.
 */
bool esObjetivo(Tablero tablero);
/**
 * @brief Calcula la distancia de Manhattan entre dos puntos.
 * @param x1 Coordenada X del primer punto.
 * @param y1 Coordenada Y del primer punto.
 * @param x2 Coordenada X del segundo punto.
 * @param y2 Coordenada Y del segundo punto.
 * @return La distancia de Manhattan entre los dos puntos.
 */
int distanciaManhattan(int x1, int y1, int x2, int y2);
/**
 * @brief Cuenta los vehículos que bloquean la línea desde Z hacia un borde.
 * @param tableroAux Referencia al tablero auxiliar.
 * @param origenX Coordenada X del origen.
 * @param origenY Coordenada Y del origen.
 * @param dX Coordenada X de la dirección.
 * @param dY Coordenada Y de la dirección.
 * @return El número de vehículos que bloquean la línea.
 */
int contarBloqueadoresEnLinea(const vector<vector<char>> &tableroAux,
                              int origenX, int origenY,
                              int dX, int dY);
/**
 * @brief Verifica si una posición está en una esquina del tablero.
 * @param xX Coordenada X de la posición.
 * @param xY Coordenada Y de la posición.
 * @return true Si la posición está en una esquina del tablero.
 * @return false En caso contrario.
 */
bool estaEnEsquinaDeTablero(int xX, int xY);
/**
 * @brief Calcula la heurística para la posición de X respecto a Z.
 * @param tablero Referencia al objeto tablero.
 * @return El valor de la heurística.
 */
// int calcularHeuristicaXAZ(Tablero &tablero);
/**
 * @brief Calcula la heurística para la posición de Z respecto a los bordes.
 * @param tablero Referencia al objeto tablero.
 * @return El valor de la heurística.
 */
// int distanciaZABorde(Tablero &tablero);
/**
 * @brief Calcula la heurística para el tablero.
 * 
 * Esta heurística intenta valorar la mejor dirección de salida para Z, de la siguiente manera:
 *   - Cuanto más cerca esté Z de un borde, menor es el coste.
 *   - Cuantos más vehículos bloqueen la línea, mayor es el coste.
 *   - Si X está lejos de la casilla necesaria para mover Z (a su alrededor), mayor es el coste.
 * @param tablero Referencia al objeto tablero.
 * @return El valor de la heurística.
*/
int calcularHeuristicaTablero(Tablero &tablero);
/**
 * @brief Calcula la solución utilizando el método de Escalada Simple.
 * @param tablero Referencia al objeto tablero.
 * @return El vector de caracteres que representa la solución.
 */
vector<char> calcularEscaladaSimple(Tablero &tablero);
/**
 * @brief Calcula la solución utilizando el método de Escalada Máxima Pendiente.
 * @param tablero Referencia al objeto tablero.
 * @return El vector de caracteres que representa la solución.
 */
vector<char> calcularEscaladaMaximaPendiente(Tablero &tablero);
/**
 * @brief Calcula la solución utilizando el método de Escalada Simple Estocástica.
 * @param tablero Referencia al objeto tablero.
 * @return El vector de caracteres que representa la solución.
 */
vector<char> calcularEscaladaSimpleEstocastica(Tablero &tablero);
/***
 * @brief Calcula la solución utilizando el método de Algoritmo A*.
 * @param tablero Referencia al objeto tablero.
 * @return El vector de caracteres que representa la solución.
 */
Nodo * comprobarEnVectores(vector<Nodo*> &abiertos, vector<Nodo*> &cerrados, Nodo *nuevoNodo);
/***
 * @brief Reconstruye la solución a partir del nodo objetivo encontrado por el algoritmo A*.
 * @param nodoObjetivo Puntero al nodo objetivo encontrado.
 * @param solucion Vector de caracteres donde se guardarán los movimientos para llegar a la solución.
 */
void actualizarDescendientesRecursivo(vector<Nodo*> &cerrados, Nodo* nodoMejorado);
/**
 * @brief Reconstruye la solución a partir del nodo objetivo encontrado por el algoritmo A*.
 * @param nodoObjetivo Puntero al nodo objetivo encontrado.
 * @param solucion Vector de caracteres donde se guardarán los movimientos para llegar a la solución.
 */
void reconstruirSolucionAEstrella(Nodo* nodo, vector<char> &solucion);
/**
 * @brief Calcula la solución utilizando el método de Algoritmo A*.
 * @param tablero Referencia al objeto tablero.
 * @return El vector de caracteres que representa la solución.
 */
vector<char> calcularAlgoritmoAEstrella(Tablero &tablero);

#endif
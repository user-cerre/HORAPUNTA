#ifndef HEURISTICA_H
#define HEURISTICA_H

#include <vector>
#include <set>
#include "operacionesMatriz.h"
#include "Tablero.h"

extern const int alfa;  // Valor umbral para la escalada simple estocástica.

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

#endif // HEURISTICA_H

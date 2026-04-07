#ifndef METODOSBUSQUEDA_H
#define METODOSBUSQUEDA_H

#include <chrono>
#include <random>
#include "operacionesMatriz.h"
#include "Nodo.h"
#include "Heuristica.h"

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

#endif // METODOSBUSQUEDA_H
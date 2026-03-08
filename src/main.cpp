#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include "operacionesMatriz.h"
#include "Tablero.h"
using namespace std; 

    const int filas = 6;
    const int columnas = 6;
    
int main(){ 

    cout<<"=================HORA PUNTA================="<<endl;
    cout<<"Authors:"<<endl;
    cerr<<"Eduardo Cabello Gil"<<endl;
    cerr<<"Carlos Paredes Ruiz"<<endl;
    cerr<<"Gabriel Ramos Sanchez"<<endl;
    cout<<endl;
    
    Tablero tablero(filas, columnas);

    vector<vector<char>> aux(filas, vector<char>(columnas));
    cargarTablero(tablero);
    mostrar(tablero);
    cout << endl;

    calcularEscaladaSimple(tablero);

    return 0; 
}

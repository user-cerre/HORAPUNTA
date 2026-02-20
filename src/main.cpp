#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include "operacionesMatriz.h"
using namespace std; 

int main(){ 
    cout<<"=================HORA PUNTA================="<<endl;
    cout<<"Authors:"<<endl;
    cerr<<"Eduardo Cabello Gil"<<endl;
    cerr<<"Carlos Paredes Ruiz"<<endl;
    cerr<<"Gabriel Ramos Sanchez"<<endl;
    cout<<endl;
    
    vector<vector<char>> tablero(filas, vector<char>(columnas));
    vector<vector<char>> aux(filas, vector<char>(columnas));
    cargarTablero(tablero);
    mostrar(tablero);
    cout << endl;

    //cout << "Elija un movimiento para hacer sobre el tablero elegido: " << endl;
    //cout << "u (arriba), d (abajo), l (izquierda), r (derecha), e (salir)" << endl;

    //char opcion;
    //while(opcion != 'e'){
    //    aux = tablero;
    //    cin >> opcion;
    //    if(opcion != 'e'){
    //        movimiento(aux, opcion);
    //        mostrar(aux);
    //    } 
    // }

    cout << "Inicio de los movimientos de prueba: " << endl;

    aux = tablero;
    movimiento(aux, 'u');
    mostrar(aux);
    aux = tablero;
    movimiento(aux, 'd');
    mostrar(aux);
    aux = tablero;
    movimiento(aux, 'r');
    mostrar(aux);
    aux = tablero;
    movimiento(aux, 'l');
    mostrar(aux); 

    cout << "Fin de los movimientos de prueba" << endl;

    return 0; 
}

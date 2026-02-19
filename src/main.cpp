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
    cargarTablero(tablero);
    mostrar(tablero);

    return 0; 
}

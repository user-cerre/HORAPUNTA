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
    cerr<<"Gabriel Ramos Sánchez"<<endl;
    cout<<endl;
    
    Tablero tablero(filas, columnas);

    vector<vector<char>> aux(filas, vector<char>(columnas));
    bool finPrograma = false;
    while(!finPrograma){
        if(!cargarTablero(tablero)){
            finPrograma = true;
            continue;
        };
        mostrar(tablero);
        cout << endl;

        int algoritmoBusqueda;
        cout << "Elige qué algoritmo usar para intentar resolver el tablero elegido: " << endl;
        cout << "1 - Escalada Simple." << endl;
        cout << "2 - Escalada Máxima Pendiente." << endl;
        cout << "3 - Escalada Simple Estocástica." << endl;
        cout << "4 - Algoritmo A*." << endl;
        cin >> algoritmoBusqueda;

        switch (algoritmoBusqueda)
        {
        case 1:
            calcularEscaladaSimple(tablero);
            break;

        case 2:
            calcularEscaladaMaximaPendiente(tablero);
            break;
        
        case 3:
            calcularEscaladaSimpleEstocastica(tablero);
            break;

        case 4:
            calcularAlgoritmoAEstrella(tablero);
            break;
        
        default:
            cout << "Opción inválida, elija de nuevo. " << endl;
            break;
        }
    }
    return 0; 
}

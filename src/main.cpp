#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
using namespace std; 

int filas = 6; 
int columnas = 12;  //son 12 columnas porque hay que tener en cuenta también las posiciones de las comas.

int main(){ 
    map<int, string> Tableros = {
        {1, "../Salidas/HORAPUNTA1.txt"},
        {2, "../Salidas/HORAPUNTA2.txt"},
        {3, "../Salidas/HORAPUNTA3.txt"},
        {4, "../Salidas/HORAPUNTA4.txt"},
        {5, "../Salidas/HORAPUNTA5.txt"},
        {6, "../Salidas/HORAPUNTA6.txt"},
        {7, "../Salidas/HORAPUNTA7.txt"},
    };
    bool opcionValida = false;
    int opcion;
    vector<vector<char>> tablero(filas, vector<char>(columnas));

    while(!opcionValida){
        cout << "Seleccione un tablero para jugar (1-7): ";
        cin >> opcion;
        if (Tableros.find(opcion) != Tableros.end()) {
            string nombreArchivo = Tableros[opcion];
            ifstream archivo(nombreArchivo);
            if (archivo.is_open()) {
                string linea;
                int j = 0;
                while (getline(archivo, linea)) {   
                    int i = 0;
                    while(i < int(linea.size())){
                        tablero[j][i] = (linea[i]); 
                        i++;
                    }
                    j++;
                }
                archivo.close();
                opcionValida = true;
            } else {
                cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
                break;
            }
        } else {
            cout << "Opción no válida. Por favor, seleccione un número entre 1 y 7." << endl;
        }
    }

    for (int i = 0; i < int(tablero.size()); i++)
    {
        for (int j = 0; j < int(tablero[i].size()); j++)
        {
            cout << tablero[i][j];
        }
        cout << endl; 
    }

    return 0; 
}

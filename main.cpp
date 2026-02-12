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
        {1, "HORAPUNTA1.txt"},
        {2, "HORAPUNTA2.txt"},
        {3, "HORAPUNTA3.txt"},
        {4, "HORAPUNTA4.txt"},
        {5, "HORAPUNTA5.txt"},
        {6, "HORAPUNTA6.txt"},
        {7, "HORAPUNTA7.txt"},
    };
    int opcion;
    vector<vector<char>> tablero(filas, vector<char>(columnas));
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
                while(i < linea.size()){
                    tablero[j][i] = (linea[i]); 
                    i++;
                }
                j++;
            }
            archivo.close();
        } else {
            cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        }
    } else {
        cout << "Opción no válida. Por favor, seleccione un número entre 1 y 7." << endl;
    }

    for (int i = 0; i < tablero.size(); i++)
    {
        for (int j = 0; j < tablero[i].size(); j++)
        {
            cout << tablero[i][j];
            if(j == tablero[i].size() - 1){ 
                cout << endl; 
            }

        }
    }
    return 0; 
}
//esto es una prueba
//esto es otra prueba
//https://prod.liveshare.vsengsaas.visualstudio.com/join?57F40A250B7574817
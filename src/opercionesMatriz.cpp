#include "operacionesMatriz.h"



void mostrar(const vector<vector<char>>& tablero) {
    for (const auto& fila : tablero) {
        for (const auto& celda : fila) {
            cout << celda;
        }
        cout << endl;
    }
}

void cargarTablero(vector<vector<char>> &tablero){
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
}
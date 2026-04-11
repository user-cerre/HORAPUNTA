# HORAPUNTA

Proyecto de la asignatura IASI (Inteligencia Artificial y Sistemas Inteligentes) 2025/2026. Es un juego/puzzle de tablero 6 x 6 en el que el vehículo Z debe llegar a cualquiera de los bordes del tablero moviendo el resto de vehículos a través del hueco X.

## Integrantes

- Eduardo Cabello Gil
- Carlos Paredes Ruiz
- Gabriel Ramos Sánchez

## Descripción

La aplicación carga tableros desde ficheros de texto, los muestra por consola y permite ejecutar distintos algoritmos de búsqueda para intentar resolverlos. El programa informa de la secuencia de movimientos encontrada, los nodos generados y el tiempo de ejecución.

La documentación base del proyecto está en [Enunciado/Sesiones/DocumentacionHORAPUNTA.pdf](Enunciado/Sesiones/DocumentacionHORAPUNTA.pdf).

## Ejecución

El programa está pensado para ejecutarse desde la raíz del repositorio, porque carga los tableros desde la carpeta [Entradas](Entradas).

```sh
g++ -std=c++17 -Wall -Wextra -pedantic src/*.cpp -o horapunta
./horapunta
```

Al arrancar:

1. Se selecciona uno de los tableros disponibles.
2. Se elige el algoritmo de búsqueda.
3. Se muestran los estados intermedios, la solución encontrada, los nodos generados y el tiempo empleado.

## Formato de los tableros

Los tableros están en [Entradas](Entradas) y siguen un formato CSV muy simple:

- 6 filas por tablero.
- 6 celdas por fila.
- Cada celda separada por coma.
- `X` representa el hueco.
- `Z` representa el vehículo objetivo.

Ejemplo de una fila:

```text
O,C,C,D,L,L,
```

## Estructura del proyecto

- [src/main.cpp](src/main.cpp): punto de entrada y menú principal.
- [src/operacionesMatriz.cpp](src/operacionesMatriz.cpp): carga, visualización y operaciones sobre el tablero.
- [src/Heuristica.cpp](src/Heuristica.cpp): cálculo de la función heurística.
- [src/MetodosBusqueda.cpp](src/MetodosBusqueda.cpp): algoritmos de búsqueda.
- [src/Tablero.h](src/Tablero.h) y [src/Tablero.cpp](src/Tablero.cpp): clase que representa el tablero.
- [src/Nodo.h](src/Nodo.h) y [src/Nodo.cpp](src/Nodo.cpp): nodos usados por A*.

## Algoritmos implementados

### Búsqueda local

- Escalada simple: prueba sucesores uno a uno y acepta el primero que mejora la heurística.
- Escalada máxima pendiente: evalúa todos los sucesores y elige el que más reduce la heurística.
- Escalada simple estocástica: introduce un umbral alfa para aceptar movimientos con una mejora suficiente.

### Búsqueda global

- A*: combina el coste acumulado g con la heurística h para priorizar los estados más prometedores.

## Heurística

La heurística intenta estimar lo cerca que está Z de salir del tablero, combinando cuatro factores:

- Distancia de Z al borde.
- Número de vehículos bloqueadores en la línea de salida.
- Distancia Manhattan entre X y la casilla necesaria para mover Z.
- Bonificación si X está en una esquina.

El cálculo evalúa las cuatro direcciones posibles y se queda con el coste más favorable.

## Lógica de movimiento

El operador de movimiento identifica el hueco X, determina qué vehículo adyacente puede desplazarse y comprueba si el movimiento es válido según la orientación y el tamaño del vehículo. Esto permite transformar un estado del tablero en otro durante la búsqueda.

## Metodología de desarrollo

El desarrollo del proyecto se ha apoyado en un flujo mixto entre trabajo humano y asistencia de IA, usando VS Code, GitHub y Live Share. También se han empleado técnicas de prompting como:

- Socratic prompting.
- Coach prompting.

## Resultados de prueba

Resumen de las baterías incluidas en la documentación:

### HORAPUNTA1

- Escalada simple: D, C, Z | 320 µs | 7 nodos
- Escalada máxima pendiente: D, C, Z | 366 µs | 12 nodos
- Escalada simple estocástica: no se ha hallado la solución | 249 µs | 8 nodos
- A*: D, C, Z | 826 µs | 12 nodos

### HORAPUNTA2

- Escalada simple: C, A, Z | 365 µs | 8 nodos
- Escalada máxima pendiente: C, A, Z | 376 µs | 12 nodos
- Escalada simple estocástica: no se ha hallado la solución | 110 µs | 4 nodos
- A*: C, A, Z | 713 µs | 12 nodos

### HORAPUNTA3

- Escalada simple: B | 176 µs | 5 nodos
- Escalada máxima pendiente: B | 216 µs | 8 nodos
- Escalada simple estocástica: no se ha hallado la solución | 109 µs | 4 nodos
- A*: B, A, J, S, G, I, Z | 2386 µs | 40 nodos

### HORAPUNTA4

- Escalada simple: no se ha hallado la solución | 294 µs | 6 nodos
- Escalada máxima pendiente: no se ha hallado la solución | 243 µs | 8 nodos
- Escalada simple estocástica: no se ha hallado la solución | 115 µs | 5 nodos
- A*: no se ha hallado la solución | 587 µs | 16 nodos

### HORAPUNTA5

- Escalada simple: E, T, J, A, Z | 444 µs | 13 nodos
- Escalada máxima pendiente: E, T, J, A, Z | 694 µs | 20 nodos
- Escalada simple estocástica: no se ha hallado la solución | 245 µs | 5 nodos
- A*: E, T, J, A, Z | 1442 µs | 20 nodos

### HORAPUNTA6

- Escalada simple: no se ha hallado la solución | 334 µs | 8 nodos
- Escalada máxima pendiente: no se ha hallado la solución | 301 µs | 8 nodos
- Escalada simple estocástica: no se ha hallado la solución | 249 µs | 6 nodos
- A*: no se ha hallado la solución | 8094 µs | 88 nodos

### HORAPUNTA7

- Escalada simple: no se ha hallado la solución | 247 µs | 7 nodos
- Escalada máxima pendiente: no se ha hallado la solución | 369 µs | 8 nodos
- Escalada simple estocástica: no se ha hallado la solución | 245 µs | 7 nodos
- A*: no se ha hallado la solución | 599 µs | 16 nodos

### HORAPUNTA8

- Escalada simple: no se ha hallado la solución | 293 µs | 7 nodos
- Escalada máxima pendiente: no se ha hallado la solución | 391 µs | 8 nodos
- Escalada simple estocástica: no se ha hallado la solución | 171 µs | 4 nodos
- A*: J, L, Z, J, L, N, M, Z | 7926 µs | 80 nodos

### HORAPUNTA9

- Escalada simple: el tablero ya está resuelto, no es necesario realizar ningún movimiento | 32 µs | 0 nodos
- Escalada máxima pendiente: el tablero ya está resuelto, no es necesario realizar ningún movimiento | 32 µs | 0 nodos
- Escalada simple estocástica: el tablero ya está resuelto, no es necesario realizar ningún movimiento | 36 µs | 0 nodos
- A*: el tablero ya está resuelto, no es necesario realizar ningún movimiento | 112 µs | 0 nodos

### HORAPUNTA10

- Escalada simple: no se ha hallado la solución | 205 µs | 4 nodos
- Escalada máxima pendiente: no se ha hallado la solución | 206 µs | 4 nodos
- Escalada simple estocástica: no se ha hallado la solución | 229 µs | 4 nodos
- A*: no se ha hallado la solución | 18881 µs | 120 nodos

## Conclusiones de la documentación

- A* es el método más robusto y el que mejor tasa de acierto presenta.
- Las variantes de escalada son muy rápidas en tableros sencillos, pero se atascan con facilidad en mínimos locales.
- La escalada simple estocástica fue la más débil en este escenario concreto.
- Para garantizar la resolución del puzle, la documentación recomienda A*.

## Referencias y herramientas mencionadas en la documentación

- Eclipse Foundation: Eclipse C/C++ Development Tooling.
- Microsoft Visual Studio Code y Live Share.
- GitHub para control de versiones.
- Modelos de lenguaje usados con Socratic prompting y Coach prompting.
- CodeSnap para generar imágenes de pseudocódigo.
- Material docente de la asignatura.

## Licencia

Este proyecto incluye la licencia GNU GPL v3 en [LICENSE](LICENSE).


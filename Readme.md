# TP1 Introduccion a Inteligencia Artifical 2017 UTNFRBA - TSP - A*

El programa TSP.c recibe como argumento un archivo .txt con la informacion del
problema y lo resuelve utilizando A*.

## Archivos que se incluyen

- TSP.c
- TSP.h
- 15 archivos .txt con datos de distintas configuraciones de ciudades.
- Resultados.txt con los costos optimos de cada .txt
- 2 carpetas con 20 archivos (10 c/u) con mas archivos utilizados para benchmarking

### Compilando

Para compilar utilizar:

make


En Windows sin full support para C99 a partir de VisualStudio 2015:
En el archivo TSP.h descomentar #define WINDOWS
Luego en la consola de Desarrollador de VisualStudio:

cl TSP.c


### Corriendo el programa

Para correr el programa hay que pasarle como argumento uno de los archivos .txt

Ej.
./TSP ruta_de_los_TXT/TSP_IN_01.txt

En WINDOWS
En la consola:
cd TSP.exe TSP_IN_01.txt

En consola se imprimira la informacion de las ciudades, estadistica de las mismas,
la Distancia minima usada para h[0]; y los resultados del algoritmo con informacion
para benchmarking.

Ej.

	***************  Ciudades  ***************

	Ciudad 0:
		 Distancia a Ciudad 1: 5
		 Distancia a Ciudad 2: 15
		 Distancia a Ciudad 3: 17
		 Distancia a Ciudad 4: 7

	Ciudad 1:
		 Distancia a Ciudad 0: 5
		 Distancia a Ciudad 2: 6
		 Distancia a Ciudad 3: 19
		 Distancia a Ciudad 4: 20

	Ciudad 2:
		 Distancia a Ciudad 0: 15
		 Distancia a Ciudad 1: 6
		 Distancia a Ciudad 3: 7
		 Distancia a Ciudad 4: 21

	Ciudad 3:
		 Distancia a Ciudad 0: 17
		 Distancia a Ciudad 1: 19
		 Distancia a Ciudad 2: 7
		 Distancia a Ciudad 4: 5

	Ciudad 4:
		 Distancia a Ciudad 0: 7
		 Distancia a Ciudad 1: 20
		 Distancia a Ciudad 2: 21
		 Distancia a Ciudad 3: 5
	*******************************************

	**************  Estadística  **************
	Cantidad de ciudades: 5
	Cantidad de conexiones: 10
	Cantidad de caminos posibles: 12
	Media de Distancias: 12.200000
	Desvío Estándar de Distancias: 6.415606
	*******************************************

	**************  Heurística  ***************
	h(0) = 25
	*******************************************

	**************  Resultados  ***************
	Camino Optimo: 0;4;3;2;1;0;
	Distancia Total = 30
	Nodos Abiertos: 9
	Nodos Creados: 17
	Nodos Eliminados: 0
	Tiempo de ejecucion = 0.222000 ms
	*******************************************

### Modificando TSP.h

El archivo TSP.h tiene 3 defines que se pueden comentar o descomentar para
modificar la informacion de salida. A saber:

#define HEURISTICS_ON
 Si esta habilitado se utiliza una heuristica distinta de 0.
 Si esta comentado h(n) = 0;

#define NO_REPETIDOS
 Si esta habilitado no se crean nodos repetidos o se reemplazan existentes
 Si esta comentado se crean todos los nodos sin importar si estan repetidos

#define DEBUG
 Si esta habilitado se muestra informacion adicional de las listas abierta y
 cerrada.
 ** Tener en cuenta que para algunos problemas la cantidad de Nodos creados es grande **

### Modificando TSP.c

El programa se maneja con 2 listas una openList y otra closedList.
Cada nodo que se crea se inserta en openList en orden de menor a mayor f(n) = g(n) + h(n).
Cada nodo que se abre pasa al final de la lista closedList.
el camino recorrido de cada nodo se obtiene sigiendo los punteros a father de cada
nodo.

La funcion que aplica el algoritmo de busqueda es:
void TSP(city* cityArray);

La funcion TSP llama a
void addNode(city* cityArray, int j, listNode* fatherNode,  int* dist, int depth, int* histogram, depthNode** depthList);
que se ocupa de agregar los nodos correspondientes y ubicarlos en la openList.

La funcion que se encarga de calcular distancias hasta el GOAL para obtener h(n)
es:
int findMinimumDistances(city* cityArray, int depth, int currentCity, int* histogram);

La h(n) de cada nodo se almacena en la variable int heuristic en la funcion addNode.

A menos que quieran guardar la informacion para manejarla de forma distinta no
deberia hacer falta modificar las funciones
void initializeCity(city* cityArray);
void populateCity(city* cityArray, char* data);

Cualquier consulta acerca del funcionamiento del programa pueden mandarnos mail.

### Authors

* **Nicolas Amaro**      email: nicoamaro@gmail.com
* **Sebastian Hernando** email: hernandosebastian@gmail.com
* **Ezequiel Wajs**      email: ezequiel.wajs@gmail.com

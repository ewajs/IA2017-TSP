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

Para compilar navegar hasta el directorio de la repo y utilizar:

	make


En **Windows** sin full support para C99 a partir de VisualStudio 2015:
En el archivo TSP.h descomentar
	#define WINDOWS
Luego en la consola de Desarrollador de VisualStudio:

      cl TSP.c


### Corriendo el programa

Para correr el programa hay que pasarle como argumento uno de los archivos .txt

Ej.

	./TSP Inputs/TSP_IN_03.txt

En **WINDOWS**
En la consola:

      TSP.exe Inputs\TSP_IN_01.txt

En consola se imprimira la informacion de las ciudades, estadistica de las mismas,
la Distancia minima usada para h[0]; y los resultados del algoritmo con informacion
para benchmarking.

Ej.

	------------------------------------------------------------
	Archivo Inputs/TSP_IN_03.txt
	------------------------------------------------------------


	********************  Ciudades  ********************

	Ciudad 1:
		 Distancia a Ciudad 2: 5
		 Distancia a Ciudad 3: 15
		 Distancia a Ciudad 4: 17
		 Distancia a Ciudad 5: 7
		 Distancia a Ciudad 6: 6

	Ciudad 2:
		 Distancia a Ciudad 1: 5
		 Distancia a Ciudad 3: 6
		 Distancia a Ciudad 4: 19
		 Distancia a Ciudad 5: 20
		 Distancia a Ciudad 6: 7

	Ciudad 3:
		 Distancia a Ciudad 1: 15
		 Distancia a Ciudad 2: 6
		 Distancia a Ciudad 4: 3
		 Distancia a Ciudad 5: 21
		 Distancia a Ciudad 6: 5

	Ciudad 4:
		 Distancia a Ciudad 1: 17
		 Distancia a Ciudad 2: 19
		 Distancia a Ciudad 3: 3
		 Distancia a Ciudad 5: 4
		 Distancia a Ciudad 6: 7

	Ciudad 5:
		 Distancia a Ciudad 1: 7
		 Distancia a Ciudad 2: 20
		 Distancia a Ciudad 3: 21
		 Distancia a Ciudad 4: 4
		 Distancia a Ciudad 6: 8

	Ciudad 6:
		 Distancia a Ciudad 1: 6
		 Distancia a Ciudad 2: 7
		 Distancia a Ciudad 3: 5
		 Distancia a Ciudad 4: 7
		 Distancia a Ciudad 5: 8
	*****************************************************

	*******************  Estadística  *******************
	Cantidad de ciudades: 6
	Cantidad de conexiones: 15
	Cantidad de caminos posibles: 60
	Media de Distancias: 10.000000
	Desvío Estándar de Distancias: 6.186006
	*****************************************************

	*******************  Heurística  ********************
	               Path Mayorante (Greedy)
	                   1;2;3;4;5;6;1;

	Distancia Mayorante (Greedy): 32
	h(0) = 27
	*****************************************************

	*******************  Resultados  ********************
	                   Camino Óptimo
	                   1;5;4;3;6;2;1;

	Distancia Óptima = 31
	Nodos Abiertos: 24
	Nodos Creados: 37
	Nodos Eliminados: 19
	Nodos Removidos: 2
	Nodos Descartados por Restricción Mayorante: 14
	Nodos Descartados por camino parcial subóptimo: 3
	Tiempo de ejecucion = 0.462000 ms
	*****************************************************

### Modificando TSP.h

El archivo TSP.h tiene varios defines que se pueden comentar o descomentar para
modificar la informacion de salida o la declaracion de variables. A saber:

#### #define HEURISTICS_ON

 Si esta habilitado se utiliza una heuristica distinta de 0.

 Si esta comentado h(n) = 0;

#### #define MAYORANTE_ON

 Si esta habilitado se utiliza un algoritmo greedy para determinar un path mayorante

 Si esta comentado no se descartan nodos por Restriccion Mayorante.

 Solo funciona con HEURISTICS_ON, en otro caso, se ignora.

#### #define NO_REPETIDOS

 Si esta habilitado no se crean nodos repetidos o se reemplazan existentes

 Si esta comentado se crean todos los nodos sin importar si estan repetidos

#### #define CIUDADES

  Habilita la impresion del detalle de las distancias entre ciudades

#### #define ESTADISTICA

 Habilita la impresion de datos como la media de las distancias entre ciudades, el desvio standard, la cantidad de caminos posibles y la cantidad de conexiones posibles para la cantidad de ciudades dadas

#### #define INFO_EXTRA

 Habilita la impresion de informacion acerca del comportamiento del algoritmo como ser la cantidad de nodos creados, eliminados, removidos y descartados.

#### #define DEBUG

 Si esta habilitado se muestra informacion adicional de las listas abierta y
 cerrada.

 **Tener en cuenta que para algunos problemas la cantidad de Nodos creados es grande**

#### #define WINDOWS

 Establece longitudes fijas a los vectores para poder compilar en entornos sin soporte a VLAs.


### Modificando TSP.c

El programa se maneja con 2 listas una openList y otra closedList.
Cada nodo que se crea se inserta en openList en orden de menor a mayor f(n) = g(n) + h(n).
Cada nodo que se abre pasa al final de la lista closedList.
el camino recorrido de cada nodo se obtiene sigiendo los punteros a father de cada
nodo. Asimismo se utiliza un vector de listas de profundidad llamado depthList
en el que se guardan los caminos recorridos para descartar aquellos nodos que
los repitan de manera menos eficiente. Nuevos nodos menos eficientes serán descartados y
en caso de encontrarse un nodo menos eficiente en la lista abierta se lo remueve antes
de insertar el nuevo nodo encontrado.


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

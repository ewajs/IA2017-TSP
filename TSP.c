/*
 * TSP.c
 *
 *  Created on: April 1, 2017
 *  Authors: Amaro Nicolas, Hernando Sebastian, Wajs Ezequiel
 */


/*Includes */
#include "TSP.h"


int cityNum;
int startNode =0;

// Variables para Benchmarking
int createdNodes, deletedNodes, removedNodes;
double mean, sd;
int main(int argc, char* argv[])
{
  FILE *importFile;
  char importText[1024*8];
  int len;
  char* delimiter;

#ifdef ESTADISTICA
  mean = 0;
  sd = 0;
#endif //ESTADISTICA
#ifdef INFO_EXTRA  
  createdNodes = 0;
  deletedNodes = 0;
  removedNodes = 0;
#endif //INFO_EXTRA  
  if(argc != 2)
  {
    printf("Debe pasar el archivo como parámetro\n");
    return 1;
  }
  //********** EMPIEZO A CONTAR TIEMPO DESDE ACA **********
  clock_t startTime = clock();

  importFile = fopen(argv[1], "r");
  if (!importFile)
    {
      printf("NO SE ENCONTRO EL ARCHIVO %s\n",argv[1]);
      return 0;
    }
  fgets(importText, 1024, importFile);
  delimiter = strchr(importText,';');
  *delimiter = '\0';
  cityNum = atoi(importText);
  printf("------------------------------------------------------------\n");
  printf("Archivo %s\n",argv[1]);
  printf("------------------------------------------------------------\n");
  printf("Cantidad de ciudades: %d\n", cityNum);
#ifdef WINDOWS  
  city cityArray[vectorLength];
#else
  city cityArray[cityNum];
#endif
  initializeCity(cityArray);
  fgets(importText, 1024*8, importFile);
  populateCity(cityArray, importText);
  TSP(cityArray);
  clock_t endTime = clock();
  //********** TERMINE DE CONTAR TIEMPO ACA **********
  double executionTime = (double)1000*(endTime - startTime) / CLOCKS_PER_SEC;
  printf("Tiempo de ejecucion = %f ms\n", executionTime);
  printf("*******************************************\n");
  return 0;
}


/*--------------------------------------------------------------------------------
 * Function:    findMinimumDistances
 *
 * @brief       Calcula la distancia minima como la suma de los promedios de
 *              las 2 distancias minimas de cada ciudad que falta recorrer.
 *
 * @param[in]	city* cityArray	- Array que contiene la info de todas las ciudades.
 * @param[in]	int depth       - Cantidad de ciudades visitadas.
 * @param[in]   int currentCity - Ciudad Actual.
 * @param[in]   int* histogram  - Histograma con ciudades visitadas.
 *
 *
 * @return 	int distance    - Distancia minima calculada (h[n])
 --------------------------------------------------------------------------------*/
int findMinimumDistances(city* cityArray, int depth, int currentCity, int* histogram)
{
  int distance = 0, i = 0,j = 0, k=0;
  int min1, min2, min = 0;

  if(depth < cityNum -1) //Entra si el nodo no es el GOAL o la anteultima ciudad
    {
      histogram[startNode] = 0;  //Tiene en cuenta las distancias a la ciudad de origen
      for(i = 0; i < cityNum; i++)
        {
          if (!histogram[i])   //Si la ciudad no fue recorrida, busco las distancias minimas
            {
              for(j = 0; j<cityNum; j++)
                {
                  if (!histogram[j] && j!=i)
                    break;
                }
              if(i < j)
                min2 = cityArray[i].distance[j-1];  //Inicializo segundo minimo
              else
                min2 = cityArray[i].distance[j];
              for(j = j+1; j<cityNum; j++)
                {
                  if (!histogram[j] && j!=i)
                    break;
                }
              if(i < j)
                {
                  min1 = cityArray[i].distance[j-1];  //Inicializo primer minimo
                  j = j-1;
                }
              else
                min1 = cityArray[i].distance[j];
              j = j+1;
              while(j<cityNum)  //Recorro todas las ciudades y solo tengo en cuenta las no visitadas
                {
                  if(!histogram[j] && j!=i)
                    {
                      if( i < j)
                        k = j-1;
                      else
                        k =j;
                      if (min1 > cityArray[i].distance[k])
                        {
                          if(min2>min1)
                            min2 = min1;
                          min1 = cityArray[i].distance[k];
                        }
                      else
                        if (min2 > cityArray[i].distance[k])
                          min2 = cityArray[i].distance[k];
                    }
                  j++;
                }
              if(depth)
                {
                  if( i == startNode || i == currentCity)
                    {
                      if(min1<min2)
                        min  += min1; // Tomo la distancia minima
                      else
                        min += min2;
                      if(i >= startNode && i >= currentCity)
                        distance += min/2; //Solo me quedar recorrer un camino por ciudad, uso los  minimos
                    }
                  else
                    distance += (min1+min2)/2; //Para la distancia uso el promedio de los dos minimos
                }
              else
                distance += (min1+min2)/2; //Para la distancia uso el promedio de los dos minimos
            }
        }
    }
  else
    {
      if (depth == cityNum) //Si estoy en el ultimo nodo = GOAL
        {
          histogram[startNode] = 1; //restauro histograma
          return (0);
        }
      else                 //Estoy en la ultima ciudad antes del GOAL
        {
          if ( currentCity > startNode)
            distance = cityArray[currentCity].distance[startNode];
          else
            distance = cityArray[currentCity].distance[startNode-1];
        }
    }
  histogram[startNode] = 1; //restauro histograma
  return (distance);
}


/*--------------------------------------------------------------------------------
 * Function:    initializeCity
 *
 * @brief       Reserva lugar en memoria del vector de ciudades y guarda su id.
 *
 * @param[in]	city* cityArray	- Array donde se va a guardar info de todas las
 *                                ciudades.
 *
 *
 *
 * @return 	NONE
 --------------------------------------------------------------------------------*/
void initializeCity(city* cityArray)
{
  for(int i = 0; i < cityNum; i++)
  {
    cityArray[i].nextCity = malloc((cityNum - 1 )*sizeof(int));
    cityArray[i].distance = malloc((cityNum - 1 )*sizeof(int));
    cityArray[i].id = i;
  }
  return;
}


/*--------------------------------------------------------------------------------
 * Function:    populateCity
 *
 * @brief       Completa el array de ciudades con la informacion pasada en data.
 *
 * @param[in]	city* cityArray	- Array a completar con la info de todas las ciudades.
 * @param[in]	int cityNum     - Cantidad de ciudades.
 * @param[in]	char* data      - Puntero a inicio de informacion a guardar.
 *
 *
 * @return 	NONE
 --------------------------------------------------------------------------------*/
void populateCity(city* cityArray, char* data)
{

  int distances;
  char* delimiter;

  int connections = 0;
  for(int i = 0; i < cityNum - 1; i++)
  {
    for(int j = i + 1; j < cityNum; j++)
    {
      delimiter = strchr(data,';');
      *delimiter = '\0';
      distances = atoi(data);
      data = delimiter + 1;
      cityArray[i].distance[j-1] = distances;
      cityArray[i].nextCity[j-1] = j;

      cityArray[j].distance[i] = distances;
      cityArray[j].nextCity[i] = i;
#ifdef ESTADISTICA
      mean = mean + distances;
      sd = sd + distances*distances;
      connections++;  // Variable que cuenta la cantidad de conexiones entre ciudades
#endif //ESTADISTICA      
    }
  }
#ifdef ESTADISTICA
  mean = mean/connections; //Calcula la media de las distancias entre ciudades
  sd = sd/connections;     
  sd = sd - mean*mean;
  sd = sqrt(sd);          //Calcula el desvio standard
#endif //ESTADISTICA
#ifdef CIUDADES
  printf("\n***************  Ciudades  ***************\n");
  for(int i = 0; i < cityNum; i++)
  {
    printf("\nCiudad %d:\n", cityArray[i].id+1);
    for(int j = 0; j < cityNum-1; j++)
      {
      printf("\t Distancia a Ciudad %d: %d\n", cityArray[i].nextCity[j]+1, cityArray[i].distance[j]);
      }
  }
#endif //CIUDADDES
#ifdef ESTADISTICA  
  printf("*******************************************\n");
  printf("\n**************  Estadística  **************\n");
  printf("Cantidad de conexiones: %d\n", connections);
  printf("Cantidad de caminos posibles: %ld\n", factorial((long int) (cityNum - 1))/2);
  printf("Media de Distancias: %f\n", mean);
  printf("Desvío Estándar de Distancias: %f\n", sd);
  printf("*******************************************\n");
#endif //ESTADISTICA
  return;

}

/*--------------------------------------------------------------------------------
 * Function:    TSP
 *
 * @brief       Se encarga de aplicar el algoritmo A* para resolver TSP
 *
 * @param[in]	city* cityArray	- Array que contiene la info de todas las ciudades.
 *
 *
 *
 * @return 	NONE
 --------------------------------------------------------------------------------*/
void TSP(city* cityArray)
{
  int  depth=0, cityFlag=0, i=0, NA=0, Node=0, currentCity=0;
  listNode* currentNode; //Puntero a nuevo nodo a crear
  listNode* openList; //Puntero a inicio de lista abierta
  listNode* closedList = NULL; //Puntero a inicio de lista cerrada
  listNode* fatherNode; //Puntero a nodo a abrir (Siempre el primer elemento de lista abierta)
#ifdef WINDOWS
  depthNode* depthList[vectorLength];
  int path[vectorLength+1]; //Reservo un vector para guardar el camino recorrido
  int histogram[vectorLength]; //Usamos un histograma para hacer comparacion rapida de caminos
  int minimumDistancesArray[vectorLength];//Generar vector de distancias minimas
#else
  depthNode* depthList[cityNum]; //Puntero a vector de listas de profundidades
  int path[cityNum+1]; //Reservo un vector para guardar el camino recorrido
  int histogram[cityNum]; //Usamos un histograma para hacer comparacion rapida de caminos
  int minimumDistancesArray[cityNum];//Generar vector de distancias minimas
#endif//WINDOWS
  for(int i = 0; i < cityNum; i++) // Inicializo mis listas de profundidad
    depthList[i] = NULL;

  for(int i = 0; i < cityNum; i++) // Inicializo histograma
    {
      histogram[i] = 0;
    }

#ifdef HEURISTICS_ON
  int minDistance = findMinimumDistances(cityArray, depth, startNode, histogram); //Hallamos h[0] y llenamos minimumDistanesArray con la segunda menor distancia de cada ciudad
  printf("\n**************  Heurística  ***************\n");
  printf("h(0) = %d\n",minDistance);
  printf("*******************************************\n");
#else
  printf("\n************  Sin Heurística  *************\n");
#endif //HEURISTICS_ON
  
#ifdef DEBUG
   printf("START NODE = %d\n",cityArray[startNode].id);
#endif //DEBUG

  //Empiezo con el primer nodo
  //Configuro el primer nodo Lista abierta
  openList = (listNode*)malloc(sizeof (listNode));
  openList->idCurrentCity = startNode;
  openList->cost = 0;

#ifdef HEURISTICS_ON
  openList->heuristic = minDistance;
#else
  openList->heuristic = 0;
#endif //HEURISTICS_ON

  openList->previousListItem = NULL;
  openList->nextListItem = NULL;
  openList->father = NULL;
  openList->isFather = 0;

  closedList = NULL;
  //Salgo cuando lista esta vacia con error o encuentro GOAL
  while(openList)
  {
    NA++;  //Contador de NODOS ABIERTOS
    fatherNode = openList; //father Node primer elemento de openlist
    currentNode = fatherNode;//currentNode para recorrer el camino de parents
    ////////BUSCAMOS CAMINO RECORRIDO SIGUIENDO LOS NODOS FATHER
    ////////////////////
    for(int i = 0; i < cityNum; i++) // reseteo histograma
      {
        histogram[i] = 0;
      }
    depth = 0; //reseteo contador de profundidad (CIUDADES VISITADAS)
    path[depth] = currentNode->idCurrentCity; //Primer elemento de camino es ciudad actual
    histogram[currentNode->idCurrentCity] = 1; //Marco ciudad visitada en histograma
    depth++;

    while(currentNode)
      {
        if (currentNode->father)
          {
            path[depth] = currentNode->father->idCurrentCity; //Guardo la ciudad visitada
            histogram[currentNode->father->idCurrentCity] = 1; //Marco ciudad visitada en histograma
            depth++;
          }
        currentNode = currentNode->father;//Paso un nivel mas arriba
      }
      
    ////////////////////
    if(openList->idCurrentCity == startNode && depth > 1) // Encontre GOAL!!!!
      {
#ifdef DEBUG
        printf("\n\n************************* GOAL  ******************************************\n\n");
        printf("------------------------------------------------------\n\n");
        printf("--------------- Open List ---------------\n");
        printList(openList);
        printf("------------------------------------------------------\n\n");
        printf("--------------- Closed  List ---------------\n");
        printList(closedList);
#endif //DEBUG
        printf("\n**************  Resultados  ***************\n");
        printf("Camino Óptimo: ");
        while(depth)
          {
            printf("%d;",path[depth-1]+1);
            depth--;
          }
        printf("\nDistancia Total = %d\n",openList->cost);
        printf("Nodos Abiertos: %d\n",NA);
#ifdef INFO_EXTRA        
        printf("Nodos Creados: %d\n",createdNodes);
        printf("Nodos Eliminados: %d\n",deletedNodes);
        printf("Nodos Removidos: %d\n",removedNodes);
        printf("Nodos Descartados: %d\n",deletedNodes - removedNodes);
#endif //INFO_EXTRA
        //Liberamos memoria
#ifdef NO_REPETIDOS
        for(int selectedDepth = 0; selectedDepth <cityNum-1; selectedDepth++)
          {
            depthNode* currDepthListItem = depthList[selectedDepth];
            if(currDepthListItem)
              {
                if(currDepthListItem->nextDepthNode)
                  while(currDepthListItem->nextDepthNode)
                    {
                      currDepthListItem = currDepthListItem->nextDepthNode;
                      free(currDepthListItem->previousDepthNode);
                    }
                free(currDepthListItem);
              }
          }
#endif  //NO_REPETIDOS
        freeMemory(openList);
        freeMemory(closedList);
        return;
      }

    //////////////////// SI NO ES NODO GOAL ABRIMOS, INCLUIMOS NUEVOS NODOS EN ORDEN ////////////////////////////////////////

    if(depth == cityNum)//Si estoy en el ultimo nodo agrego solo el nodo GOAL
      {
        Node = startNode; //El nodo GOAL es el mismo que el startNode
        if(fatherNode->idCurrentCity < Node)
          Node -= 1;
        addNode(cityArray,Node,fatherNode, minimumDistancesArray, depth, histogram, depthList);

      }
    else //Si no estoy en el ultimo nodo agrego todas las ciudades no visitadas.
      {
        for(int j = 0; j < cityNum -1; j++)
          {
            currentCity = cityArray[fatherNode->idCurrentCity].nextCity[j];
            if (!histogram[currentCity]) //Si no esta en el camino recorrido agrego el nodo
              {
                addNode(cityArray,j,fatherNode, minimumDistancesArray, depth, histogram, depthList);
              }
            cityFlag = 0;
          }
      }
    ////////////////////////////////////////////////////////////////////////////////

    ////// SACAMOS NODO ABIERTO DE LISTA ABIERTA Y LO PASAMOS A FINAL DE LISTA CERRADA //////
    openList = fatherNode->nextListItem; //Cambio primer item de lista abierta
    openList->previousListItem = NULL;
    currentNode = closedList; //Me paro en el inicio de la lista cerrada
    if(closedList) //Si hay algun elemento en la lista cerrarda busco hasta llegar al ultimo
      {
        while(currentNode->nextListItem)
          {
            currentNode = currentNode->nextListItem;
          }
        currentNode->nextListItem = fatherNode; //Ubicamos el nodo abierto al final de la lista
        fatherNode->previousListItem = currentNode; //Apuntamos puntero al anteultimo elemento
      }
    else //Si no hay ningun nodo en Lista CERRADA
      {
        closedList = fatherNode; //Ubicamos el primer nodo abierto al inicio de la lista
      }
    fatherNode->nextListItem = NULL; // Apunto en ultimo nodo agregado de cerrada a null
  }

  //SI SALGO DEL WHILE POR ACA NO ENCONTRE EL GOAL!!!!!!
  printf("\n\n****************************************************************************\n\n");
  printf("ERROR!! NO SE ENCONTRO SOLUCION.\n");

}


/*--------------------------------------------------------------------------------
 * Function:    printList
 *
 * @brief       Imprime en pantalla todos los nodos de la lista con su info.
 *
 * @param[in]	listNode* a	- Direccion de primer nodo de la lista a imprimir.
 *
 * @return 	NONE
 --------------------------------------------------------------------------------*/
void printList(listNode* a)
{
  listNode* currentNode = a;
  while (NULL != currentNode)
  {
    int myFather = 0;
    if (currentNode->father != NULL)
      {
        myFather = currentNode->father->idCurrentCity;
      }
    printf("id=%d, cost=%d, heur=%d, father=%d\n",currentNode->idCurrentCity+1, currentNode->cost, currentNode->heuristic,myFather+1);
    currentNode= currentNode->nextListItem;
  }
}


/*--------------------------------------------------------------------------------
 * Function:    addNode
 *
 * @brief       Agrega un nodo a la lista en orden dependiendo del costo total.
 *
 * @param[in]	city* cityArray	- Array que contiene la info de todas las ciudades.
 * @param[in]   int  j          - Indice de ciudad actual en array para el nodo padre.
 * @param[in]	listNode* fatherNode  - Direccion de nodo padre.
 * @param[in]   int* dist       - Direccion de vector de distancias minimas.
 * @param[in]   int* depth      - Profundidad del nodo a abrir.
 * @param[in]   int* histogram  - Histograma de ciudades recorridas por el nodo a abrir.
 * @param[in]   depthNode** dephtList    - Puntero a vector de listas de profundidad.
 *
 *
 * @return 	NONE
 --------------------------------------------------------------------------------*/
void addNode(city* cityArray, int j, listNode* fatherNode, int *dist, int depth, int* histogram, depthNode** depthList)
{
  listNode *pivotNode = fatherNode;
  listNode *prevNode = NULL;
  listNode *auxNode;
  int *hist = histogram;
#ifdef WINDOWS
  int histogram2[vectorLength];
#else  
  int histogram2[cityNum];
#endif //WINDOWS
  int costToMe = cityArray[fatherNode->idCurrentCity].distance[j]; //Costo de viajar de n-1 a n
  int currentCity = cityArray[fatherNode->idCurrentCity].nextCity[j]; 
  int currentCost = fatherNode->cost + costToMe; // g(n)
#ifdef NO_REPETIDOS
  depthNode* currDepthListItem = depthList[depth-1]; //Apunto a la lista de profundiad correspondiente a este nivel

  while(currDepthListItem) // Mientras haya algun item en la lista para comparar entro
  {

    if(currDepthListItem->node->idCurrentCity == currentCity) //Busco coincidencia con ciudad actual
    {
      for(int i = 0; i < cityNum; i++) //Limpio el histograma
        {
          histogram2[i] = 0;
        }

      auxNode = currDepthListItem->node->father; //Empiezo a armar el camino partiendo del padre

      while(auxNode)
      {
        histogram2[auxNode->idCurrentCity] = 1; //Marco ciudad recorrida en histograma
        auxNode = auxNode->father; //Pasamos a la ciudad anterior
      }

      int flagDif = 0; // Reseteo flag de coincidencia de caminos recorridos

      for(int i = 0; i < cityNum; i++)
        {
          if(histogram[i] - histogram2[i] != 0)
            {
              flagDif++;  //Si hay diferencia en el camino recorrido incremento el flag y salgo
              break;
            }
        }

      if(!flagDif) //llegué a un path que termina en la ciudad a abrir y compuesto por las mismas ciudades
        {
#ifdef INFO_EXTRA
          deletedNodes++; //Contador de nodos eliminados (removidos + no creados)
#endif //INFO_EXTRA
          if(currentCost < currDepthListItem->node->cost) // Si encontré un mejor camino bajo estas condiciones, borro el anterior.
            {
#ifdef INFO_EXTRA
              removedNodes++; //Contador de nodos removidos (fue creado pero encontre un camino mejor)
#endif //INFO_EXTRA
              listNode* nodeToDelete = currDepthListItem->node;
              // Borro de la lista abierta.
              if(nodeToDelete->previousListItem) // si no estoy en el primer elemento
                {
                  nodeToDelete->previousListItem->nextListItem = nodeToDelete->nextListItem;
                }
              if(nodeToDelete->nextListItem)
                nodeToDelete->nextListItem->previousListItem = nodeToDelete->previousListItem;
              // Borro de las listas de nivel.
              if(currDepthListItem->previousDepthNode) // si no estoy en el primer elemento
                {
                  currDepthListItem->previousDepthNode->nextDepthNode = currDepthListItem->nextDepthNode;
                }else
                {
                  depthList[depth-1] = currDepthListItem->nextDepthNode;
                }
              if(currDepthListItem->nextDepthNode)
                currDepthListItem->nextDepthNode->previousDepthNode = currDepthListItem->previousDepthNode;
              free(currDepthListItem); //Libero memoria
              if(!nodeToDelete->isFather) //Si el nodo a borrar esta en la lista abierta
                {
                  free(nodeToDelete); //Libero memoria
                }
              break;
            }else
            {
              return; // Si el costo actual es mayor a uno que tiene el mismo camino, no lo agrego.
            }
        }
    }
    currDepthListItem = currDepthListItem->nextDepthNode;
  }
#endif //NO_REPETIDOS
  
  // Si nunca explore el camino o el camino explorado era de mayor costo. Agrego nuevo Nodo a la lista abierta.
#ifdef INFO_EXTRA
  createdNodes++; //Contador de nodos creados
#endif //INFO_EXTRA
  listNode* currentNode =(listNode*) malloc(sizeof(listNode));
  currentNode->   idCurrentCity     = currentCity;
  currentNode->   cost              = currentCost;
#ifdef HEURISTICS_ON
  currentNode->heuristic = findMinimumDistances(cityArray, depth, currentCity, hist);
  if(fatherNode->heuristic > (currentNode->heuristic + costToMe)) //Verificamos que h(n) sea consistente
     currentNode->heuristic = fatherNode->heuristic - costToMe;
  if (currentNode->heuristic < 0) //La heuristica tiene que ser siempre >= 0
    currentNode->heuristic = 0;
#else
  currentNode->   heuristic         = 0;
#endif //HEURISTICS_ON

  currentNode->   isFather          = 0;
  currentNode->   father            = fatherNode;
  fatherNode->    isFather          = 1;
  currentCost = currentNode->cost + currentNode->heuristic; //f(n) = g(n) + h(n)
  while(pivotNode->nextListItem)//Recorro la lista hasta encontrar el lugar donde insertar el nodo
    {
      if(currentCost < (pivotNode->cost+pivotNode->heuristic)) //Tengo que insertar el nodo antes
        {
          currentNode->   previousListItem  = pivotNode->previousListItem;
          prevNode                          = pivotNode->previousListItem;
          if(prevNode)
            prevNode->    nextListItem      = currentNode;
          currentNode->   nextListItem      = pivotNode;
          pivotNode->     previousListItem  = currentNode;
          break;
        }
      pivotNode = pivotNode->nextListItem;
    }
  if(!pivotNode->nextListItem) //Llegue al final de la lista
    {

      currentNode->   previousListItem  = pivotNode;
      currentNode->   nextListItem      = NULL;
      pivotNode->     nextListItem      = currentNode;
    }
#ifdef  NO_REPETIDOS
  // Agrego el nodo actual a la lista de profundidades para comparar a futuro
  if(depthList[depth-1] == NULL) // Primer camino con esta profundidades
    {
      depthNode * newDepthNode = (depthNode*)malloc(sizeof(depthNode));
      depthList[depth-1] = newDepthNode;
      newDepthNode->previousDepthNode = NULL;
      newDepthNode->nextDepthNode = NULL;
      newDepthNode->node = currentNode;
    }else // si ya habían nodos con esta produndidad, lo agrego al final.
    {
      currDepthListItem = depthList[depth-1];
      while(currDepthListItem->nextDepthNode)
        currDepthListItem = currDepthListItem->nextDepthNode;
      depthNode* newDepthNode = (depthNode*)malloc(sizeof(depthNode));
      newDepthNode->nextDepthNode = NULL;
      newDepthNode->previousDepthNode = currDepthListItem;
      newDepthNode->node = currentNode;
      currDepthListItem->nextDepthNode = newDepthNode;
    }
#endif  //NO_REPETIDOS
  return;
}


/*--------------------------------------------------------------------------------
 * Function:    freeMemory
 *
 * @brief       Libera la memoria de todos los nodos de la lista a partir del nodo
 *              indicado.
 *
 * @param[in]	listNode* a	- Direccion de primer nodo a partir del cual
 *                                liberar memoria.
 *
 * @return 	NONE
 --------------------------------------------------------------------------------*/
void freeMemory(listNode* a)
{
  while(a->nextListItem)
    {
      a = a->nextListItem;
      free(a->previousListItem);
    }
  free(a);
}

/*--------------------------------------------------------------------------------
 * Function:    factorial
 *
 * @brief       Dado un numero entero, calcula el factorial del mismo
 *
 * @param[in]	long int	- Numero f al cual se desea calcular su factorial
 *
 * @return 	long int        - Factorial de f = f!
 --------------------------------------------------------------------------------*/
long int factorial(long int f)
{
    if ( f == 0 )
        return 1;
    return(f * factorial(f - 1));
}

/*
 * TSP.h
 *
 *  Created on: April 1, 2017
 *  Authors: Amaro Nicolas, Hernando Sebastian, Wajs Ezequiel
 */

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* Defines */
#define HEURISTICS_ON  //Para implementar heuristica HEURISTICS_ON
//#define DEBUG  //Para imprimir LISTAS ABIERTA y CERRADA y otra info
#define NO_REPETIDOS //Si esta activo no agrega Nodos de estados repetidos
//#define WINDOWS //Para compilar en entornos sin soporte para VLAs (arrays de long variables)

#define vectorLength  20
/* Variable type Declarations */
typedef struct {
  int id;           // ID de la ciudad
  int* nextCity;    // Puntero al vector de ciudades conectadas
  int* distance;    // Puntero al vector con distancia entre ciudades conectadas
}city;

typedef struct listNode{
  int idCurrentCity;  // ID de la ciudad
  int cost;           // Costo recorrido igual a g[n]
  int heuristic;      // Valor estimado que falta hasta la meta h[n]
  int isFather;       // isFather = 1 si el nodo fue abierto y tiene hijos, isFather = 0 otherwise
  struct listNode*  father; //Puntero a Nodo padre

  struct listNode* nextListItem;     // Puntero a siguiente item en la lista
  struct listNode* previousListItem; // Puntero a item anterior en la lista
}listNode;

typedef struct depthNode{
  struct listNode* node;                // Puntero a Nodo
  struct depthNode* nextDepthNode;      // Puntero a siguiente item en la lista
  struct depthNode* previousDepthNode;  // Puntero a item anterior en la lista
}depthNode;


/* Functions Declarations */
void initializeCity(city* cityArray);
void populateCity(city* cityArray, char* data);
void TSP(city* cityArray);
void printList(listNode* a);
void addNode(city* cityArray, int j, listNode* fatherNode,  int* dist, int depth, int* histogram, depthNode** depthList);
void freeMemory (listNode*a);
int findMinimumDistances(city* cityArray, int depth, int currentCity, int* histogram);
long int factorial(long int f);

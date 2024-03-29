//
// Created by liwbh on 06/05/23.
//

#ifndef PROYECTO_2_SO_NODO_H
#define PROYECTO_2_SO_NODO_H
#include "ListaDirecionesM.h"
#include "stdbool.h"
#include "string.h"

typedef struct NodoProceso{
    //variables para FCFS
    int id;
    char nombre[50];
    char nombreE_S[50];
    int peso;
    int nIteraciones;
    int tiempoE_S;
    ListaPosicion *listaPosicion;

    //variables de politica de administracio
    int listaPFVT;
    int nEjecucion;//para calcular tiempo promedio en ejecucion
    double sumTiempoEj;//para calcular tiempo promedio en ejecucion
    int nEspera;//para calcular tiempo promedio en espera
    double sumTiempoES;//para calcular tiempo promedio en espera
    bool crecimiento;//etiqueta proceso en crecimiento
    int numBloques;

    //variables manejo de lista
    struct NodoProceso *anterior;
    struct NodoProceso *siguiente;
}NodoProceso;

NodoProceso *crearNodoProceso(int id, char *nombre, int peso, int nIteraciones, char *nombreE_S, int tiempoE_S){
    NodoProceso *nodo = (NodoProceso*)malloc(sizeof(NodoProceso));
    //variables para FCFS
    nodo->id = id;
    strcpy(nodo->nombre,nombre);
    strcpy(nodo->nombreE_S,nombreE_S);
    nodo->peso = peso;
    nodo->nIteraciones = nIteraciones;
    nodo->tiempoE_S = tiempoE_S;
    nodo->listaPosicion = crearListaPosicion();

    //variables de politica de administracion
    nodo->listaPFVT = 0;//particiones fijas tamaño variable
    nodo->nEjecucion = 0;
    nodo->sumTiempoEj = 0.0;
    nodo->nEspera = 0;
    nodo->sumTiempoES = 0.0;
    nodo->crecimiento = false;
    nodo->numBloques = 0;

    //variables manejo de lista
    nodo->siguiente = NULL;
    nodo->anterior = NULL;
    return nodo;
}

NodoProceso *clonarNodo(NodoProceso *nodo){
    NodoProceso *nodoClonado = crearNodoProceso(nodo->id, nodo->nombre, nodo->peso, nodo->nIteraciones, nodo->nombreE_S, nodo->tiempoE_S);
    nodoClonado->listaPosicion = nodo->listaPosicion;
    nodoClonado->listaPFVT = nodo->listaPFVT;
    nodoClonado->numBloques = nodo->numBloques;
    nodoClonado->nEjecucion = nodo->nEjecucion;
    nodoClonado->sumTiempoEj = nodo->sumTiempoEj;
    nodoClonado->nEspera = nodo->nEspera;
    nodoClonado->sumTiempoES = nodo->sumTiempoES;
    nodoClonado->crecimiento = nodo->crecimiento;
    return nodoClonado;
}

//
void mostrarNodoProceso(NodoProceso *nodoProceso){
    printf("\033[0;34mId: %d, Nombre: %s, Peso: %d, Iteraciones: %d, Dispositivo E/S: %s, Tiempo E/S: %d \n\033[0m",nodoProceso->id,nodoProceso->nombre,nodoProceso->peso,nodoProceso->nIteraciones,nodoProceso->nombreE_S,nodoProceso->tiempoE_S);
}

#endif //PROYECTO_2_SO_NODO_H

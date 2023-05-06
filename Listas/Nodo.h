//
// Created by liwbh on 06/05/23.
//

#ifndef PROYECTO_2_SO_NODO_H
#define PROYECTO_2_SO_NODO_H
#include "ListaDirecionesM.h"
#include "stdbool.h"
#include "string.h"

extern pthread_mutex_t mutex_turno;
extern pthread_cond_t cond_turno;

typedef struct NodoProceso{
    int id;
    char nombre[50];
    int peso;
    int nIteraciones;
    bool ejecucion;
    char nombreE_S[50];
    int tiempoE_S;
    bool contexto;
    ListaPosicion *listaPosicion;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    struct NodoProceso *anterior;
    struct NodoProceso *siguiente;
}NodoProceso;

NodoProceso *crearNodoProceso(int id, char *nombre, int peso, int nIteraciones, bool ejecucion, char *nombreE_S, int tiempoE_S){
    NodoProceso *nodo = malloc(sizeof(NodoProceso));
    strcpy(nodo->nombre,nombre);
    strcpy(nodo->nombreE_S,nombreE_S);
    nodo->id = id;
    nodo->peso = peso;
    nodo->nIteraciones = nIteraciones;
    nodo->ejecucion = ejecucion;
    nodo->tiempoE_S = tiempoE_S;
    nodo->listaPosicion = crearListaPosicion();
    nodo->contexto = false;
    nodo->siguiente = NULL;
    nodo->anterior = NULL;
    return nodo;
}

void mostrarNodoProceso(NodoProceso *nodoProceso){
    printf("Id: %d, Nombre: %s, Peso: %d, Iteraciones: %d, Ejecucion: %d, Dispositivo E/S: %s, Tiempo E/S: %d \n",nodoProceso->id,nodoProceso->nombre,nodoProceso->peso,nodoProceso->nIteraciones,nodoProceso->ejecucion,nodoProceso->nombreE_S,nodoProceso->tiempoE_S);
}

#endif //PROYECTO_2_SO_NODO_H

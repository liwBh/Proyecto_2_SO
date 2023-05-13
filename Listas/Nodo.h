//
// Created by liwbh on 06/05/23.
//

#ifndef PROYECTO_2_SO_NODO_H
#define PROYECTO_2_SO_NODO_H
#include "ListaDirecionesM.h"
#include "stdbool.h"
#include "string.h"

typedef struct NodoProceso{
    int numBloques;
    int id;
    char nombre[50];
    int peso;
    int nIteraciones;
    bool ejecucion;
    char nombreE_S[50];
    int tiempoE_S;
    bool contexto;
    int listaPFVT;
//    char inicio[50];
//    char finalizacion[50];
    ListaPosicion *listaPosicion;
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
    nodo->listaPFVT = 0;
    nodo->numBloques = 0;
    nodo->siguiente = NULL;
    nodo->anterior = NULL;
    return nodo;
}
NodoProceso *crearNodoProcesoDos(int id, char *nombre, int peso, int nIteraciones, bool ejecucion, char *nombreE_S, int tiempoE_S, int numBloques){
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
    nodo->listaPFVT = 0;
    nodo->numBloques = numBloques;
    nodo->siguiente = NULL;
    nodo->anterior = NULL;
    return nodo;
}
NodoProceso *clonarNodo(NodoProceso *nodo){//la inf
    printf(" \n el nodo traer este numero de bloques: %d \n", nodo->numBloques);
    printf(" \n entro al clonar NODO \n");
    NodoProceso *nodoClonado = crearNodoProcesoDos(nodo->id,nodo->nombre,nodo->peso,nodo->nIteraciones,nodo->ejecucion,nodo->nombreE_S,nodo->tiempoE_S, nodo->numBloques);
    printf(" \n entro al clonar 50 \n");
    nodoClonado->listaPosicion = nodo->listaPosicion;
    printf(" \n entro al clonar 52 \n");
    nodoClonado->listaPFVT = nodo->listaPFVT;
    printf(" \n entro al clonar 54 \n");
   /* nodoClonado->numBloques = nodo->numBloques;
    printf(" \n num bloques nodo clonado: %d", nodoClonado->numBloques);*/
//    strcpy(nodoClonado->inicio,nodo->inicio);
//    strcpy(nodoClonado->finalizacion,nodo->finalizacion);
    return nodoClonado;

}

//
void mostrarNodoProceso(NodoProceso *nodoProceso){
    printf("\033[0;34mId: %d, Nombre: %s, Peso: %d, Iteraciones: %d, Ejecucion: %d, Dispositivo E/S: %s, Tiempo E/S: %d \n\033[0m",nodoProceso->id,nodoProceso->nombre,nodoProceso->peso,nodoProceso->nIteraciones,nodoProceso->ejecucion,nodoProceso->nombreE_S,nodoProceso->tiempoE_S);
}

#endif //PROYECTO_2_SO_NODO_H

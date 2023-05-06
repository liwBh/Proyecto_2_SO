//
// Created by liwbh on 06/05/23.
//

#ifndef PROYECTO_2_SO_NODO_E_S_H
#define PROYECTO_2_SO_NODO_E_S_H
#include "Lista.h"

typedef struct Nodo_E_S{
    ListaProcesos *listaProcesos;
    struct Nodo_E_S *siguiente;
    struct Nodo_E_S *anterior;
}Nodo_E_S;

Nodo_E_S *crearNodo_E_S(ListaProcesos *listaProcesos){
    Nodo_E_S *nodoES = malloc(sizeof(Nodo_E_S));
    nodoES->listaProcesos = listaProcesos;
    return nodoES;
}

void mostrarNodo_E_S(Nodo_E_S *nodo){
    NodoProceso *aux = nodo->listaProcesos->primero;
    while(aux != NULL){
        mostrarNodoProceso(aux);
        aux = aux->siguiente;
    }
}

#endif //PROYECTO_2_SO_NODO_E_S_H

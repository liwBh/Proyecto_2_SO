//
// Created by liwbh on 06/05/23.
//

#ifndef PROYECTO_2_SO_LISTA_E_S_H
#define PROYECTO_2_SO_LISTA_E_S_H
#include "Nodo_E_S.h"

typedef struct Lista_E_S{
    Nodo_E_S *primero;
    Nodo_E_S *ultimo;
}Lista_E_S;

//Crea y reserva espacio en memoria de la lista
Lista_E_S *crearLista_E_S(){
    Lista_E_S *lista = (Lista_E_S*) malloc(sizeof(Lista_E_S));
    lista->primero = NULL;
    lista->ultimo = NULL;
    return lista;
}

bool vacia(Lista_E_S *lista){
    if(lista->primero == NULL){
        return true;
    }else{
        return false;
    }
}

void insertarNodo_E_S(Lista_E_S *lista, Nodo_E_S *nodo){
    if(vacia(lista)){
        lista->primero = nodo;
        lista->ultimo = nodo;
    }else{
        lista->ultimo->siguiente = nodo;
        nodo->anterior = lista->ultimo;
        lista->ultimo = nodo;
    }
}

void mostrarLista_E_S(Lista_E_S *lista){
    Nodo_E_S *aux = lista->primero;
    printf("\n\n");
    while(aux != NULL){
        mostrarNodo_E_S(aux);
        aux = aux->siguiente;
    }
}

#endif //PROYECTO_2_SO_LISTA_E_S_H
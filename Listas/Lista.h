//
// Created by liwbh on 06/05/23.
//

#ifndef PROYECTO_2_SO_LISTA_H
#define PROYECTO_2_SO_LISTA_H

#include <unistd.h>
#include "Nodo.h"
#include "stdbool.h"


//Una lista que contiene los procesos
typedef struct ListaProcesos{
    NodoProceso *primero;
    NodoProceso *ultimo;
}ListaProcesos;

//Crea y reserva espacio en memoria de la lista
ListaProcesos *crearListaProcesos(){
    ListaProcesos *lista = (ListaProcesos*) malloc(sizeof(ListaProcesos));
    lista->primero = NULL;
    lista->ultimo = NULL;
    return lista;
}

//Verifica si la lista esta vacia
bool listaVacia(ListaProcesos *lista){
    if(lista->primero == NULL){
        return true;
    }else{
        return false;
    }
}

//Inserta un NodoProceso dentro de la lista de procesos
void insertar(ListaProcesos *lista, NodoProceso *nodo){
    if(listaVacia(lista)){
        lista->primero = nodo;
        lista->ultimo = nodo;
    }else{
        lista->ultimo->siguiente = nodo;
        nodo->anterior = lista->ultimo;
        lista->ultimo = nodo;
    }
}

void eliminarNodo(ListaProcesos *lista, int idNodoEliminar){ //ELimina un nodo (ficha) de una lista

    if(listaVacia(lista)){ //Primero se verifica que la lista no este vacia

        printf("\nLista vacia!\n");
        return;
    }

    NodoProceso *aux1 = lista->primero; //Un nodo que recorre la lista

    while (aux1 != NULL){
        //Si el nodo a eliminar es la ultima igualamos las variables de la lista a NULL
        if(idNodoEliminar == lista->ultimo->id && lista->primero->id == lista->ultimo->id ){
            //printf("\n---------------------------------- IF #1 Eliminar - el ultimo de la lista");
            lista->primero = NULL;
            lista->ultimo = NULL;
            //si elimina el ultimo nodo y solo hay 2 nodos
        }else if(  (idNodoEliminar== lista->ultimo->id) && (lista->primero->siguiente == lista->ultimo) && (lista->ultimo->anterior == lista->primero)){
            //printf("\n---------------------------------- IF #2 Eliminar - el ultimo de la lista");
            lista->ultimo = lista->primero;
            lista->primero->siguiente = NULL;
            lista->primero->anterior = NULL;
            //si elimina el primer nodo y solo hay 2 nodos
        }else if( (idNodoEliminar== lista->primero->id) && (lista->ultimo->anterior == lista->primero) && (lista->primero->siguiente == lista->ultimo) ){
            //printf("\n---------------------------------- IF #3 Eliminar - el primero de la lista");
            lista->primero = lista->ultimo;
            lista->ultimo->anterior = NULL;
            lista->ultimo->siguiente = NULL;
            //elimina el último de la lista, si hay más de 2 nodos
        }else if((idNodoEliminar == aux1->id) && (aux1->id == lista->ultimo->id)) {
            //printf("\n---------------------------------- IF #4 Eliminar - el ultimo de la lista");
            lista->ultimo = aux1->anterior;
            aux1->anterior->siguiente = NULL;
            aux1->anterior = NULL;
            //elimina el primero de la lista, si hay más de 2 nodos
        }else if((idNodoEliminar == aux1->id) && (aux1->id == lista->primero->id)) {
            //printf("\n---------------------------------- IF #5 Eliminar - el primero de la lista");
            lista->primero = aux1->siguiente;
            aux1->siguiente = NULL;
            aux1->anterior = NULL;
            //Elimina en el medio de la lista
        }else if(idNodoEliminar == aux1->id){
            //printf("\n---------------------------------- IF #6 Eliminar - en medio de la lista");
            aux1->anterior->siguiente = aux1->siguiente;
            aux1->siguiente->anterior = aux1->anterior;
            aux1->anterior = NULL;
            aux1->siguiente = NULL;
        }

        aux1 = aux1->siguiente;
    }

}

//Se recorre la lista para mostrar los procesos
void mostrarListaProcesos(ListaProcesos *lista){
    NodoProceso *aux = lista->primero;
    printf("\n\n");
    while(aux != NULL){
        mostrarNodoProceso(aux);
       // sleep(1);
        aux = aux->siguiente;
    }
}

#endif //PROYECTO_2_SO_LISTA_H

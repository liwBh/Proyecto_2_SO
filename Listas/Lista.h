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
    int tamanio;
}ListaProcesos;

//Crea y reserva espacio en memoria de la lista
ListaProcesos *crearListaProcesos(){
    ListaProcesos *lista = (ListaProcesos*) malloc(sizeof(ListaProcesos));
    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->tamanio = 0;
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
    lista->tamanio+=1;
}

void eliminarNodo(ListaProcesos *lista, int id) {

    if(listaVacia(lista)){
        printf("\033[1;31mNo hay procesos en lista para eliminar!\n");

        return;
    }

    if( id == lista->primero->id && lista->primero->id == lista->ultimo->id ){//solo hay un nodo
        lista->primero = NULL;
        lista->ultimo = NULL;

    }else if( id == lista->primero->id ){//se elimina el primero de la lista
        NodoProceso *actual = lista->primero;
        lista->primero = actual->siguiente;
        actual->siguiente->anterior = NULL;

    }else if( id == lista->ultimo->id ){//se elimina el ultimo de la lista
        NodoProceso *actual = lista->ultimo;
        lista->ultimo = actual->anterior;
        actual->anterior->siguiente = NULL;

    }else{ //se elimina en el medio del primero y ultimo de la lista
        NodoProceso *actual = lista->primero;
        while (actual != NULL){

            if(actual->id == id){
                actual->anterior->siguiente = actual->siguiente;
                actual->siguiente->anterior = actual->anterior;
                //se rompe el ciclo nodo encontrado
                break;
            }

            actual = actual->siguiente;
        }
    }

    lista->tamanio-=1;
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

//Se recorre la lista para mostrar los procesos
void mostrarListaProcesosPFVT(ListaProcesos *lista, int listaPFVT){
    NodoProceso *aux = lista->primero;
    printf("\n\n");
    printf("\033[0;33m\nLista de Particion fija de tamaño: %d kb, numero de bloques: %d!\033[0m\n", listaPFVT, listaPFVT/4);

    while(aux != NULL){
        if(aux->listaPFVT == listaPFVT){
            mostrarNodoProceso(aux);
        }
        // sleep(1);
        aux = aux->siguiente;
    }
}

#endif //PROYECTO_2_SO_LISTA_H

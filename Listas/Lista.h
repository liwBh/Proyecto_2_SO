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



//void eliminarNodo(ListaProcesos *lista, int idNodoEliminar){ //ELimina un nodo (ficha) de una lista
//
//    if(listaVacia(lista)){ //Primero se verifica que la lista no este vacia
//
//        printf("\nLista vacia!\n");
//        return;
//    }
//
//    NodoProceso *aux1 = lista->primero; //Un nodo que recorre la lista
//
//    while (aux1 != NULL){
//        //Si el nodo a eliminar es la ultima igualamos las variables de la lista a NULL
//        if(idNodoEliminar == lista->ultimo->id && lista->primero->id == lista->ultimo->id ){
//            //printf("\n---------------------------------- IF #1 Eliminar - el ultimo de la lista");
//            lista->primero = NULL;
//            lista->ultimo = NULL;
//            break;
//            //si elimina el ultimo nodo y solo hay 2 nodos
//        }else if(  (idNodoEliminar== lista->ultimo->id) && (lista->primero->siguiente == lista->ultimo) && (lista->ultimo->anterior == lista->primero)){
//            //printf("\n---------------------------------- IF #2 Eliminar - el ultimo de la lista");
//            lista->ultimo = lista->primero;
//            lista->primero->siguiente = NULL;
//            lista->primero->anterior = NULL;
//            break;
//            //si elimina el primer nodo y solo hay 2 nodos
//        }else if( (idNodoEliminar== lista->primero->id) && (lista->ultimo->anterior == lista->primero) && (lista->primero->siguiente == lista->ultimo) ){
//            //printf("\n---------------------------------- IF #3 Eliminar - el primero de la lista");
//            lista->primero = lista->ultimo;
//            lista->ultimo->anterior = NULL;
//            lista->ultimo->siguiente = NULL;
//            break;
//            //elimina el último de la lista, si hay más de 2 nodos
//        }else if((idNodoEliminar == aux1->id) && (aux1->id == lista->ultimo->id)) {
//            //printf("\n---------------------------------- IF #4 Eliminar - el ultimo de la lista");
//            lista->ultimo = aux1->anterior;
//            aux1->anterior->siguiente = NULL;
//            aux1->anterior = NULL;
//            break;
//            //elimina el primero de la lista, si hay más de 2 nodos
//        }else if((idNodoEliminar == aux1->id) && (aux1->id == lista->primero->id)) {
//            //printf("\n---------------------------------- IF #5 Eliminar - el primero de la lista");
//            lista->primero = aux1->siguiente;
//            aux1->siguiente = NULL;
//            aux1->anterior = NULL;
//            break;
//            //Elimina en el medio de la lista
//        }else if(idNodoEliminar == aux1->id){
//            //printf("\n---------------------------------- IF #6 Eliminar - en medio de la lista");
////            aux1->anterior->siguiente = aux1->siguiente;
////            aux1->siguiente->anterior = aux1->anterior;
//
//            //bug -> Aquiiiiiiii
//            if (aux1->anterior != NULL) {//aqui esta el bug
//                aux1->anterior->siguiente = aux1->siguiente;
//            }else{
//                printf("No elimino, el anterior es NULL");
//            }
//
//            if (aux1->siguiente != NULL) {//aqui esta el bug
//                aux1->siguiente->anterior = aux1->anterior;
//                printf("No elimino, el siguiente es NULL");
//            }
//
////            if (aux1 == lista->primero) {
////                lista->primero = aux1->siguiente;
////            }
////            if (aux1 == lista->ultimo) {
////                lista->ultimo = aux1->anterior;
////            }
//
//            aux1->anterior = NULL;
//            aux1->siguiente = NULL;
//            break;
//        }
//
//        aux1 = aux1->siguiente;
//    }
//    lista->tamanio-=1;
//}
//
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

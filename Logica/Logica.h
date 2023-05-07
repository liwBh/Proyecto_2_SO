//
// Created by jurguen on 18/04/23.
//

#ifndef QUIZ_SO_LOGICA_H
#define QUIZ_SO_LOGICA_H

#include <ctype.h>
#include "../Listas/Lista.h"
#include "../MatrizMemoria/Matriz.h"
#include <math.h>

void insertarUnProceso(ListaProcesos *lista, NodoProceso *nodo){
    insertar(lista, nodo);
}
void pasarProcesoDePeticionListos(ListaProcesos *listaPeticion, ListaProcesos *listaContenedor, ListaProcesos *listaListos){
    if(!listaVacia(listaPeticion)){
        //Crea un nodo con el primero de la lista de espera
        NodoProceso *almacenarProceso = clonarNodo(listaPeticion->primero);
        //Cambia la referencia del primero de lista
        listaPeticion->primero = listaPeticion->primero->siguiente;
        //Inserta el nodo en la lista contenedor
        insertar(listaContenedor,almacenarProceso);
        insertar(listaListos,almacenarProceso);
        //Elimina el nodo de la lista de espera
        eliminarProcesoEsperando(listaPeticion,almacenarProceso);
    }else{
        printf("Ya no hay mas procesos");
    }
}
//Pasa un proceso de la lista de espera a la lista que ya fueron atendidos en memoria
void pasarProcesoContenedor(ListaProcesos *listaPeticion, ListaProcesos *listaContenedor){
    if(!listaVacia(listaPeticion)){
        //Crea un nodo con el primero de la lista de espera
        NodoProceso *almacenarProceso = clonarNodo(listaPeticion->primero);
        //Cambia la referencia del primero de lista
        listaPeticion->primero = listaPeticion->primero->siguiente;
        //Inserta el nodo en la lista contenedor
        insertarUnProceso(listaContenedor,almacenarProceso);
        //Elimina el nodo de la lista de espera
        eliminarProcesoEsperando(listaPeticion,almacenarProceso);
    }else{
        printf("Ya no hay mas procesos");
    }
}

int encontrarCantidadDeBloques(int n) {
    int resultado = n / 4;
    if (n % 4 != 0) {
        resultado += 1;
    }
    return resultado;
}

//Metodo que recorra matriz y busque el primer espacio disponible, devuelve i y j
void asignarEspacioDisponible(struct Bloque matriz[8][8], NodoProceso *nodo){

    // determinar el numero de bloques del proceso
    int nBloques = encontrarCantidadDeBloques(nodo->peso);

    printf("\nEl numero de bloques: %d para el proceso: %d", nBloques, nodo->id);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if(matriz[i][j].disponible == 0){
                //insertar i,j en un nodo posicion
                NodoPosicion *nodoPosicion = crearNodoPosicion(i,j);

                matriz[i][j].disponible = 1;
                matriz[i][j].idProceso = nodo->id;

                //agregar la posicion en la lista del proceso
                insertarNodoPosicion(nodo->listaPosicion, nodoPosicion);

                nBloques--;

                if(nBloques == 0){
                    i = 8;
                    break;
                }
            }
        }
    }

    printf("\nDireciones de Memoria Asignadas");
    mostrarListaPosiciones(nodo->listaPosicion);
}

//Metodo para liberar memoria de la matriz segun las posiciones en que se ubica el proceso, con la lista de posiciones
void liberarMemoria(NodoProceso *nodo, struct Bloque matriz[8][8]){
    NodoPosicion *aux = nodo->listaPosicion->primero;
    while (aux != NULL){ //Se recorre la lista de posiciones del proceso

        matriz[aux->i][aux->j].disponible = 0; //Y se libera la disponibilidad en memoria
        matriz[aux->i][aux->j].idProceso = 0; //Se libera el id del proceso en memoria

        //printf("Eliminando pos i:%d j:%d ",);

        aux = aux->siguiente;
    }
}

void continuarProcesosEspera(ListaProcesos *listaEspera, ListaProcesos *listaListos){
    //validar que lista de espera no este vacia
    if(!listaVacia(listaEspera)){

        //recorre toda la lista de espera
        NodoProceso *aux = listaEspera->primero;
        while(aux != NULL){

            //El tiempo de espera restarle 1
            aux->tiempoE_S = aux->tiempoE_S - 1;

            //cuando sale de tiempo de espera llego a 0
            if(aux->tiempoE_S == 0){
                //se debe generar otro aleatorio de espera
                int nuevoTiempo = (rand() % 3) + 1;
                aux->tiempoE_S = nuevoTiempo;
                //moverlo a lista de listos
                insertar(listaListos,aux);
                //sacarlo de lista de espera
                eliminarProcesoEsperando(listaEspera,aux);

            }

            //pasarlo a lista de listos
            aux = aux->siguiente;
        }
    }
}

#endif //QUIZ_SO_LOGICA_H

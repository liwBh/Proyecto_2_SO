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
        eliminarNodo(listaPeticion,almacenarProceso->id);
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

    printf("\nIngresando un proceso a contexto de ejecucion");
    printf("\nEl numero de bloques: %d para el proceso con el ID: %d\n", nBloques, nodo->id);
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

void continuarProcesosEspera(ListaProcesos *listaEspera, ListaProcesos *listaListos, int id){
    //validar que lista de espera no este vacia
    if(!listaVacia(listaEspera)){

        //recorre toda la lista de espera
        NodoProceso *aux = listaEspera->primero;
        while(aux != NULL){

            //si no es el mismo proceso que estaba en ejecucion
            if(aux->id != id){
                //El tiempo de espera restarle 1
                aux->tiempoE_S = aux->tiempoE_S - 1;
            }

            //cuando sale de tiempo de espera llego a 0
            if(aux->tiempoE_S == 0 && aux->id != id){
                //se debe generar otro aleatorio de espera
                int nuevoTiempo = (rand() % 3) + 1;
                aux->tiempoE_S = nuevoTiempo;
                //moverlo a lista de listos
                NodoProceso *nodoClon = clonarNodo(aux);
                insertar(listaListos,nodoClon);
                //sacarlo de lista de espera
                eliminarNodo(listaEspera,aux->id);
                break;
            }

            //pasarlo a lista de listos
            aux = aux->siguiente;
        }
    }
}


void entrarContextoEjecucion(ListaProcesos *listaPeticion, ListaProcesos *listaContenedor, ListaProcesos *listaListos){
    if(listaVacia(listaPeticion)){
        printf("\nNo hay pocesos en lista de peticion para ejecutar!\n");
        return;
    }

    NodoProceso *nodoInsertar1= clonarNodo(listaPeticion->primero);
    NodoProceso *nodoInsertar2= clonarNodo(listaPeticion->primero);
    insertar(listaContenedor,nodoInsertar1);
    insertar(listaListos,nodoInsertar2);

    //Cambia la referencia del primero de lista de peticion, se elimina
    listaPeticion->primero = listaPeticion->primero->siguiente;
}

void salirContextoEjecucion(ListaProcesos *listaContenedor, ListaProcesos *listaListos, NodoProceso *nodoEliminar){
    if(listaVacia(listaContenedor)){
        printf("\nNo hay pocesos en lista de contexto para ejecutar!\n");
        return;
    }
    int idNodoEliminar = nodoEliminar->id;
    eliminarNodo(listaContenedor, idNodoEliminar);
    eliminarNodo(listaListos, idNodoEliminar);

    free(nodoEliminar);
}

void desfragmentarMemoria(struct Bloque matriz[8][8], ListaProcesos *listaContenedor){
    //limpiar posicion en matriz
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            matriz[i][j].disponible = 0;
            matriz[i][j].idProceso = 0;
        }
    }

    //recorrer lista
    NodoProceso *aux = listaContenedor->primero;
    while (aux != NULL){
        //resetea la lista de posiciones de cada proceso
        aux->listaPosicion = crearListaPosicion();
        //determinar el numero de bloques
        int nBloques = (aux->peso / 4);
        printf("\nMoviendo el numero de bloques: %d del proceso: %d", nBloques, aux->id);
        //colocar el numero de posciones desde la ultima posicion
        for(int i = 7; i >= 0; i--) {

            for(int j = 7; j >= 0; j--) {
                if(matriz[i][j].disponible == 0) {
                    NodoPosicion *nodoPosicion = crearNodoPosicion(i, j);
                    matriz[i][j].disponible = 1;
                    matriz[i][j].idProceso = aux->id;

                    //agregar la posicion en la lista del proceso
                    insertarNodoPosicion(aux->listaPosicion, nodoPosicion);
                    nBloques--;
                    if (nBloques == 0) {
                        i = 0;
                        break;
                    }
                }
            }
        }
        aux = aux->siguiente;
    }

    printf("\n\nLa memoria termino de ser desfragmentada\n");
    // mostrarMatriz(matriz);
}

//Metodo que recorra matriz y busque el primer espacio disponible, devuelve i y j
ListaPosicion *asignarEspacioDisponiblePFVT(struct Bloque matriz[8][8], NodoProceso *nodo) {

    int nBloques = encontrarCantidadDeBloques(nodo->peso);
    printf("El numero de bloques: %d para el proceso: %d", nBloques, nodo->id);
    //crear una lista
    ListaPosicion *listaPosiciones = crearListaPosicion();

    //insertar en la lista las posciones, si en algun momento no puede insertar se resetea la lista
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            NodoPosicion *almacena = crearNodoPosicion(0, 0);
            if (matriz[i][j].disponible == 0) {
                almacena->i = i;
                almacena->j = j;
                insertarNodoPosicion(listaPosiciones, almacena);
                nBloques--;
            } else {
                //resetear la lista
                listaPosiciones->primero = NULL;
                listaPosiciones->ultimo = NULL;
                nBloques = (nodo->peso / 4);
            }
            //detener ciclo, por que ya se asignaron bloques
            if (nBloques == 0) {
                i = 8;
                break;
            }
        }
    }

    if (!estaVacia(listaPosiciones)) {
        //recorer la lista y asinar bloques
        NodoPosicion *aux = listaPosiciones->primero;
        while (aux != NULL) {
            //Asignado en matriz
            matriz[aux->i][aux->j].disponible = 1;
            matriz[aux->i][aux->j].idProceso = nodo->id;
            //Agregando direccion al metodo de los bloques en memoria
            insertarNodoPosicion(nodo->listaPosicion, aux);
            aux = aux->siguiente;
        }
    } else {
        //liberar un proceso para evitar saturacion de memoria
//        NodoProceso *nodoEliminar = seleccionarNodoAleatorio(listaContenedor); // Almacenar nodo a eliminar
//        printf("\nLiberando de memoria %d bloques del proceso: %d",(nodoEliminar->peso/4), nodoEliminar->id);
//        liberarMemoria(nodoEliminar,matriz); //Libera la memoria por medio de la lista de posiciones que tiene el procesoodoEl

        //defragmentar
        //printf("\n\n*****Es necesario desfragmentar memoria*****\n");
        //desfragmentarMemoria(matriz, listaContenedor);
    }
}
#endif //QUIZ_SO_LOGICA_H

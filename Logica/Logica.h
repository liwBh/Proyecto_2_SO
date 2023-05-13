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
        printf("\033[1;31mYa no hay mas procesos\033[0m");

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

    printf("\033[1;33m\nIngresando un proceso a contexto de ejecucion");
    printf("\nEl numero de bloques: %d para el proceso con el ID: %d\n\033[0m", nBloques, nodo->id);

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
        printf("\033[1;31mNo hay procesos en lista de peticiones para ejecutar!\n");

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
        printf("\033[1;31m\nNo hay procesos en lista de contexto para ejecutar!\n\033[0m");

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
        printf("\033[38;5;214m\nMoviendo el numero de bloques: %d del proceso: %d\033[0m", nBloques, aux->id);

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

    printf("\033[0;32m%s\033[0m", "\n\nLa memoria terminó de ser desfragmentada\n");

    // mostrarMatriz(matriz);
}

void buscarEspacioDisponiblePFVT(struct Bloque matriz[8][8], NodoProceso *nodo) {
    int nBloques = encontrarCantidadDeBloques(nodo->peso);

    if(nBloques > 2 && nBloques <= 4){//16bits - 4 bloques
        nBloques = 4;
    }else if(nBloques > 4 && nBloques <= 8){//32bits - 8 bloques
        nBloques = 8;
    }


    printf("\033[0;32mEl numero de bloques: %d para el proceso: %d\033[0m", nBloques, nodo->id);


    //insertar en la lista las posciones, si en algun momento no puede insertar se resetea la lista
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            NodoPosicion *almacena = crearNodoPosicion(0, 0);
            if (matriz[i][j].disponible == 0) {
                almacena->i = i;
                almacena->j = j;
                insertarNodoPosicion(nodo->listaPosicion, almacena);
                nBloques--;
            } else {
                //resetear la lista
                nodo->listaPosicion->primero = NULL;
                nodo->listaPosicion->ultimo = NULL;
                nBloques =  encontrarCantidadDeBloques(nodo->peso);
                if(nBloques > 2 && nBloques <= 4){//16bits - 4 bloques
                    nBloques = 4;
                }else if(nBloques > 4 && nBloques <= 8){//32bits - 8 bloques
                    nBloques = 8;
                }
            }
            //detener ciclo, por que ya se asignaron bloques
            if (nBloques == 0) {
                i = 8;
                break;
            }
        }
    }

}

//Metodo para mostrar listas de particiones fijas
void mostrarPFVT(ListaProcesos *listaContenedor){
    int indice = 4;

    for (int i = 0; i < 4; ++i) {
        mostrarListaProcesosPFVT(listaContenedor, indice);
        indice *= 2;
    }
}

//Metodo que recorra matriz y busque el primer espacio disponible, devuelve i y j
void asignarEspacioDisponiblePFVT(struct Bloque matriz[8][8], NodoProceso *nodo, ListaProcesos *listaContenedor) {

    buscarEspacioDisponiblePFVT(matriz, nodo);
    //printf("\nPosiciones en memoria asiganas al proceso:");
    //mostrarListaPosiciones(nodo->listaPosicion);

    //si tiene hueco en memoria para el proceso
    if (!estaVacia(nodo->listaPosicion)) {

        //recorer la lista y asinar bloques en la memoria
        NodoPosicion *aux = nodo->listaPosicion->primero;
        while (aux != NULL) {
            //Asignado en matriz
            matriz[aux->i][aux->j].disponible = 1;
            matriz[aux->i][aux->j].idProceso = nodo->id;

            aux = aux->siguiente;
        }

        mostrarMatriz(matriz);

    } else {
        //defragmentar
        printf("\033[1;31m\n\n*****Es necesario desfragmentar memoria*****\n\033[0m");

        desfragmentarMemoria(matriz, listaContenedor);

        //Meterlo en memoria
        buscarEspacioDisponiblePFVT(matriz, nodo);
        //recorer la lista y asinar bloques en la memoria
        NodoPosicion *aux = nodo->listaPosicion->primero;
        while (aux != NULL) {
            //Asignado en matriz
            matriz[aux->i][aux->j].disponible = 1;
            matriz[aux->i][aux->j].idProceso = nodo->id;
            //Agregando direccion al metodo de los bloques en memoria
            insertarNodoPosicion(nodo->listaPosicion, aux);
            aux = aux->siguiente;
        }

        //hacer una solicitud de cambio de politica de administracion de memoria
    }

    /*
         Maximo de memoria 256
         listaPFVT_4 maximo nodos = 64, cantidad de bloques a utilizar = 1
         listaPFVT_8 maximo nodos = 32, cantidad de bloques a utilizar = 2
         listaPFVT_16 maximo nodos = 16, cantidad de bloques a utilizar = 4
         listaPFVT_32 maximo nodos = 8, cantidad de bloques a utilizar = 8
    */

    //asignarlo a una lista de particiones de tamaño variado
    int nBloques = encontrarCantidadDeBloques(nodo->peso);

    if(nBloques == 1){//4bits - 1 bloques
        //insertar en listaPFVT_4
        nodo->listaPFVT = 4;
        nodo->numBloques = 1;
        //printf("\nlistaPFVT_4 %d\n", nodo->listaPFVT);

    }else if(nBloques == 2){//8bits - 2 bloques
        //insertar en listaPFVT_8
        nodo->listaPFVT = 8;
        nodo->numBloques = 2;
        //printf("\nlistaPFVT_8 %d\n", nodo->listaPFVT);

    }else if(nBloques > 2 && nBloques <= 4){//16bits - 4 bloques
        //insertar en listaPFVT_16
        nodo->listaPFVT = 16;
        nodo->numBloques = 4;

        //printf("\nlistaPFVT_16 %d\n", nodo->listaPFVT);

    }else if(nBloques > 4 && nBloques <= 8){//32bits - 8 bloques
        //insertar en listaPFVT_32
        nodo->listaPFVT = 32;
        nodo->numBloques = 8;
        //printf("\nlistaPFVT_32 %d\n", nodo->listaPFVT);
    }

  //  printf("\nNumero de bloques: %d\n", nodo->numBloquesJeff);
}

int calcularDesperdicioInterno(NodoProceso *procesoEvaluar){
   // return (encontrarCantidadDeBloques(procesoEvaluar->peso)*4)-procesoEvaluar->peso;

    return (procesoEvaluar->numBloques * 4) - procesoEvaluar->peso;
}

int calcularDesperdicioInternoTotal(ListaProcesos *listaContenedor){
    int desperdicioInternoTotal = 0;
    NodoProceso *aux = listaContenedor->primero;
    printf("\n\n");
    while(aux != NULL){
        desperdicioInternoTotal+= calcularDesperdicioInterno(aux);
        // sleep(1);
        aux = aux->siguiente;
    }
    return desperdicioInternoTotal;
}

int calcularDesperdicioExternoVector(struct Bloque matriz[8][8]) {
    int desperdicioExterno = 0;
    int vector[64];
    int contador = 0;
    int cerosDesocupados = 0;

    // Convertir matriz en vector
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            vector[contador] = matriz[i][j].disponible;
            contador++;
        }
    }

    // Hacer el cálculo de desperdicio externo en el vector
    for (int i = 0; i < 64; i++) {
        if (vector[i] == 1) {
            int j = i + 1;
            while (j < 64 && vector[j] == 0) {
                cerosDesocupados++;
                j++;
            }
            if (cerosDesocupados < 8) {
                desperdicioExterno += cerosDesocupados;
            }
            i = j - 1;
            cerosDesocupados = 0;
        }
    }
    return desperdicioExterno;
}

#endif //QUIZ_SO_LOGICA_H

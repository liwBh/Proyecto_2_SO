//
// Created by jurguen on 18/04/23.
//

#ifndef PROYECTO_2_SO_LOGICA_H
#define PROYECTO_2_SO_LOGICA_H

#include <ctype.h>
#include "../Listas/Lista.h"
#include "../MatrizMemoria/Matriz.h"
#include "../ListaLigadas/ListasLigadas.h"
#include "../Socios/Socios.h"
#include <math.h>

//============================ Administracion de procesos FCFS ============================//
void insertarUnProceso(ListaProcesos *lista, NodoProceso *nodo){
    insertar(lista, nodo);
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

    //resetear la lista
    nodo->listaPosicion->primero = NULL;
    nodo->listaPosicion->ultimo = NULL;

}

//Metodo que trabaja los procesos que estan en la lista de espera
void continuarProcesosEspera(ListaProcesos *listaEspera, ListaProcesos *listaListos, int id) {

    //validar que lista de espera no este vacia
    if (!listaVacia(listaEspera)) {

        //recorre toda la lista de espera
        NodoProceso *aux = listaEspera->primero;
        while (aux != NULL) {

            //si no es el mismo proceso que estaba en ejecucion
            if (aux->id != id) {

                aux->tiempoE_S -= 1; // Restar 1 al tiempo de espera
                //mostrarNodoProceso(aux);
                //aux->nEspera++;

                //si el tiempo de espera se acabo
                if (aux->tiempoE_S == 0) {

                    //generar un nuevo tiempo de espera
                    int nuevoTiempo = (rand() % 3) + 1;
                    aux->tiempoE_S = nuevoTiempo;
                    //aumentar el tiempo que debera permanecer en espera
                    aux->sumTiempoES += (0.000010 * nuevoTiempo);

                    //pasar el nodo a la lista listo
                    NodoProceso *nodoClon = clonarNodo(aux);
                    insertar(listaListos, nodoClon);

                    //eliminar el nodo de la lista de espera
                    eliminarNodo(listaEspera, aux->id);
                    break; // Salir del ciclo while después de procesar un nodo
                }
            }

            aux = aux->siguiente;
        }
    }

    //evitar que no queden procesos en la lista de listos, terminacion prematura del programa
    if (listaVacia(listaListos)) {
        //Agregar de forma obligatoria un proceso de listaEspera a listaListo
        NodoProceso *nodoClon = clonarNodo(listaEspera->primero);
        insertar(listaListos, nodoClon);
        //eliminar el nodo de la lista de espera
        eliminarNodo(listaEspera, nodoClon->id);
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


//============================ Rendimiento memoria ============================//
int encontrarCantidadDeBloques(int n) {
    int resultado = n / 4;
    if (n % 4 != 0) {
        resultado += 1;
    }
    return resultado;
}

int calcularDesperdicioInterno(NodoProceso *procesoEvaluar) {
    int nbloques = procesoEvaluar->numBloques;
    int peso = procesoEvaluar->peso;
    int resultado = (nbloques * 4) - peso;

    return resultado;
}

int calcularDesperdicioInternoTotal(ListaProcesos *listaContenedor){
    int desperdicioInternoTotal = 0;
    NodoProceso *aux = listaContenedor->primero;
    printf("\n\n");
    while(aux != NULL){
        desperdicioInternoTotal+= calcularDesperdicioInterno(aux);
        aux = aux->siguiente;
    }
    return desperdicioInternoTotal;
}

int calcularDesperdicioExternoVector(struct Bloque matriz[8][8]) {
    int desperdicioExterno = 0;
    int vector[64];
    int contador = 0;

    // Convertir matriz en vector
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            vector[contador] = matriz[i][j].disponible;
            contador++;
        }
    }

    // Hacer el cálculo de desperdicio externo en el vector
    for (int i = 0; i < 64; i++) {
        if (vector[i] == 0) {
            desperdicioExterno++;
        }
    }

    return desperdicioExterno * 4;
}

int generarCreacimientoP(){
    int crecimientoP[20];

    // Llenar el array con 15 ceros
    for (int i = 0; i < 20; i++) {
        crecimientoP[i] = 0;
    }

    for (int i = 0; i < 5; i++) {
        int pos = rand() % 20;
        crecimientoP[pos] = (rand() % 10) + 1;
    }

    int posAleatoria = rand() % 20;

    return  crecimientoP[posAleatoria];
}

//============================ Politicas de administracion memoria ============================//

void asignarEspacioMemoria(struct Bloque matriz[8][8], NodoProceso *nodo){
    //recorer la lista y asinar bloques en la memoria
    NodoPosicion *aux = nodo->listaPosicion->primero;
    while (aux != NULL) {
        //Asignado en matriz
        matriz[aux->i][aux->j].disponible = 1;
        matriz[aux->i][aux->j].idProceso = nodo->id;

        aux = aux->siguiente;
    }

    mostrarMatriz(matriz);
    printf("\033[0;32mEl numero de bloques: %d para el proceso: %d\n\033[0m", nodo->numBloques, nodo->id);
}

void desfragmentarMemoria(struct Bloque matriz[8][8], ListaProcesos *listaContenedor){
    printf("\033[0;31m%s\033[0m", "\n\nDesfragmentando la memoria \n\033[0m");

    //limpiar posicion en matriz
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            matriz[i][j].disponible = 0;
            matriz[i][j].idProceso = 0;
        }
    }
    mostrarMatriz(matriz);

    //recorrer lista
    NodoProceso *aux = listaContenedor->primero;
    while (aux != NULL){
        //resetea la lista de posiciones de cada proceso
        aux->listaPosicion = crearListaPosicion();
        //determinar el numero de bloques
        int nBloques = aux->numBloques;
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

    printf("\033[0;32m%s\033[0m", "\n\nLa memoria terminó de ser desfragmentada\n\033[0m");

    mostrarMatriz(matriz);
}

//metodo que llena la lista de posiciones en memoria de un nodo, si estan contiguos -> solo politica PFVT
void buscarEspacioDisponiblePFVT(struct Bloque matriz[8][8], NodoProceso *nodo) {
    int nBloques = encontrarCantidadDeBloques(nodo->peso);

    if(nBloques > 2 && nBloques <= 4){//16bits - 4 bloques
        nBloques = 4;
    }else if(nBloques > 4 && nBloques <= 8){//32bits - 8 bloques
        nBloques = 8;
    }

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

//metodo que llena la lista de posiciones en memoria de un nodo, si estan contiguos -> Generico, no modificar
void buscarEspacioDisponible(struct Bloque matriz[8][8], NodoProceso *nodo) {
    int nBloques = encontrarCantidadDeBloques(nodo->peso);

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
                //resetear el numero de bloques
                nBloques =  encontrarCantidadDeBloques(nodo->peso);
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

//Metodo que administra memoria aplicando politica de particiones fijas de tamaño variable
void asignarEspacioDisponiblePFVT(struct Bloque matriz[8][8], NodoProceso *nodo, ListaProcesos *listaContenedor,ListaProcesos *listaListos, ListaProcesos *listaPeticion) {

    //asignarlo a una lista de particiones de tamaño variado
    int nBloques = encontrarCantidadDeBloques(nodo->peso);

    if(nBloques == 1){//4bits - 1 bloques
        //insertar en listaPFVT_4
        nodo->listaPFVT = 4;
        nodo->numBloques = 1;

    }else if(nBloques == 2){//8bits - 2 bloques
        //insertar en listaPFVT_8
        nodo->listaPFVT = 8;
        nodo->numBloques = 2;

    }else if(nBloques > 2 && nBloques <= 4){//16bits - 4 bloques
        //insertar en listaPFVT_16
        nodo->listaPFVT = 16;
        nodo->numBloques = 4;

    }else if(nBloques > 4 && nBloques <= 8){//32bits - 8 bloques
        //insertar en listaPFVT_32
        nodo->listaPFVT = 32;
        nodo->numBloques = 8;
    }

    buscarEspacioDisponiblePFVT(matriz, nodo);

    //si tiene hueco en memoria para el proceso
    if (!estaVacia(nodo->listaPosicion)) {

        asignarEspacioMemoria(matriz, nodo);

    } else {
        //defragmentar
        printf("\033[1;31m\n\n*****Es necesario desfragmentar memoria*****\n\033[0m");
        desfragmentarMemoria(matriz, listaContenedor);

        //Meterlo en memoria
        buscarEspacioDisponiblePFVT(matriz, nodo);

        //validar si realmente luego de desfragmentar hay espacio en memoria
        if(!estaVacia(nodo->listaPosicion)){
            //recorer la lista y asinar bloques en la memoria
            asignarEspacioMemoria(matriz, nodo);
        }else{
            printf("\033[1;31m\n\n*****No hay espacio en memoria para el proceso regresar a Lista de peticiones*****\n\033[0m");
            salirContextoEjecucion(listaContenedor, listaListos, nodo);
            insertarUnProceso(listaPeticion, nodo);
        }
    }

}

//Metodo que administra memoria aplicando politica mapa de bits
void asignarEspacioDisponibleMB(struct Bloque matriz[8][8], NodoProceso *nodo, ListaProcesos *listaContenedor,ListaProcesos *listaListos, ListaProcesos *listaPeticion){
    //definir el numero de bloques
    nodo->numBloques = encontrarCantidadDeBloques(nodo->peso);

    buscarEspacioDisponible(matriz, nodo);

    //si tiene hueco en memoria para el proceso
    if (!estaVacia(nodo->listaPosicion)) {

        asignarEspacioMemoria(matriz, nodo);

    }else{

        //defragmentar
        printf("\033[1;31m\n\n*****Es necesario desfragmentar memoria*****\n\033[0m");
        desfragmentarMemoria(matriz, listaContenedor);

        //Meterlo en memoria
        buscarEspacioDisponible(matriz, nodo);

        //validar si realmente luego de desfragmentar hay espacio en memoria
        if(!estaVacia(nodo->listaPosicion)){
            //recorer la lista y asinar bloques en la memoria
            asignarEspacioMemoria(matriz, nodo);
        }else{
            printf("\033[1;31m\n\n*****No hay espacio en memoria para el proceso regresar a Lista de peticiones*****\n\033[0m");
            salirContextoEjecucion(listaContenedor, listaListos, nodo);
            insertarUnProceso(listaPeticion, nodo);
        }
    }

}

int reasignacionMemoriaXpolitica(int tipoPolitica, struct Bloque matriz[8][8], NodoProceso *nodoProceso, ListaProcesos *listaContenedor,ListaProcesos *listaListos, ListaProcesos *listaPeticion){

    switch (tipoPolitica) {
        case 1:
            //asignarle espacio en memoria en base a PFVT, al proceso entrante al contexto de ejecucion
            printf("\033[1;31m\n----------PARTICIONES FIJAS CON VARIOS TAMAÑOS----------\n\n\033[0m");
            asignarEspacioDisponiblePFVT(matriz,nodoProceso,listaContenedor,listaListos, listaPeticion);

            break;
        case 2:
            //asignarle espacio en memoria en base a mapa de bits, al proceso entrante al contexto de ejecucion
            printf("\033[1;31m\n----------MAPA DE BITS----------\n\n\033[0m");
            asignarEspacioDisponibleMB(matriz,nodoProceso,listaContenedor,listaListos, listaPeticion);
            break;

        case 3:

            //asignarle espacio en memoria en base a Lista Ligadas, al proceso entrante al contexto de ejecucion
            printf("\033[1;31m\n----------LISTAS LIGADAS----------\n\033[0m");
            nodoProceso->numBloques = encontrarCantidadDeBloques(nodoProceso->peso);
            asignarEspacioDisponibleLL(nodoProceso->id);
            break;

        case 4:

            //asignarle espacio en memoria en base a socios, al proceso entrante al contexto de ejecucion
            printf("\033[1;31m\n----------Socios----------\n\033[0m");
            nodoProceso->numBloques = nuevoEspacio(nodoProceso->peso);
            asignarEspacioDisponibleS(nodoProceso);

            break;

        default:
            printf("\033[1;31m\nOcurrio un error al aplicar politica\n\033[0m");
            printf("\nCalmaaaaaaaaaaaa! HP Calmaaaaaaaaaaaa! Aquí deberia ir la porlitica que no ha programado! >_<\n");
            return  1;
    }

    return 0;

}


#endif //PROYECTO_2_SO_LOGICA_H

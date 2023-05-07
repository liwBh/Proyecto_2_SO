#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <threads.h>
#include <string.h>
#include <pthread.h>
#include "Listas/Lista.h"
#include "Listas/Lista_E_S.h"
#include "MatrizMemoria/Matriz.h"
#include "Logica/Logica.h"

pthread_mutex_t mutex_turno = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_turno = PTHREAD_COND_INITIALIZER;
struct Bloque matriz[8][8];


//bandera para la finalizacion de la simulacion
int banderaFinalizacion = 0;
//numero de procesos
int nProcesos = 0;
//hilo planificador - hilo administrador 2
pthread_t administrador;
pthread_t planificador;
//vector de hilos - procesos

pthread_t *procesos;

//Generales del simulador
//lista programas para ser atendidos
ListaProcesos *listaPeticion;
//lista programas contenedor
ListaProcesos *listaContenedor;

//FCFS
//lista en espera -> lista de dispositivos: pantalla, mouse, teclado
Lista_E_S  *lista_E_S;
ListaProcesos *listaEspera;
//lista de listos
ListaProcesos *listaListos;

void crearListas();
void llenarListaProcesosEsperando();
void llenarMemoriaInicio();
void *administrarProcesos(void *args);
void *iniciarPlanificador(void *args);

int main() {

    printf("------------- Emulador de memoria Particiones Fijas y Variables ------------\n");

    printf("\nTamaño de memoria = 256kb\n");
    printf("Tamaño del bloque = 4kb\n");

    //creando las listas del emulador
    crearListas();

    //creando los procesos del emulador
    llenarListaProcesosEsperando();

    printf("\nInicializando estructura de Memoria\n");
    //crear aquitectura de memoria
    llenarMatriz( matriz);

    //Imprimir matriz
    mostrarMatriz( matriz );

    //llenar lista de contenedor -> en base a la capacidad de la memoria
    llenarMemoriaInicio();

    //Imprimir matriz
    mostrarMatriz( matriz );

    //mostra listas
    printf("\nProcesos restantes en la lista de solicitudes\n");
    mostrarListaProcesos(listaPeticion);
    printf("\nProcesos agregados en la lista de contexto de ejecucion\n");
    mostrarListaProcesos(listaContenedor);


    exit(0);



    //FCFS
    //Nodo proceso: id, nombre, peso, listaDirecciones, nIteraciones, ejecucion, nombreE_S, TiempoE_S -> Jurguern

    //crear la listas

    //Funcion que cree procesos de forma dinamica -> Wil - Jeff

    //Generar una funcion que mantenga un proceso en la lista de espera y luego lo pase a lista de listos -> Elmer

    //Estructura de trabajo en hilos -> Wil - Jeff

    //Iniciar planificador
    pthread_create(&(planificador), NULL, &iniciarPlanificador, NULL);

    //inicia la simulaicon del hilo
    pthread_join(planificador, NULL);


    printf("\n--------{El programa ha Finalizado su Ejecucion!}---------\n");
    return 0;
}

void crearListas(){
    //Listas del emulador
    listaContenedor = crearListaProcesos();
    listaPeticion = crearListaProcesos();

    //FCFS
    lista_E_S = crearLista_E_S();
    listaEspera = crearListaProcesos();
    listaListos = crearListaProcesos();
}

void llenarListaProcesosEsperando(){

    printf("\n---------Creando procesos ---------\n");

    srand(time(NULL));
    nProcesos = rand() % (30 - 25 + 1) + 25;

    //inicializar array de procesos
    //se retorna un puntero a la primer pos del vector de procesos
    procesos = (pthread_t*)malloc(sizeof (pthread_t)*nProcesos);

    for (int i = 0; i < nProcesos; ++i) {
        char str[10];
        char nombre[10] = "P-";
        int peso = rand() % 40 + 1 ;
        int iteraciones = rand() % 10 + 1;
        int indice_aleatorio = rand() % 3;
        int tiempo = (rand() % 5) + 1;
        char *dispositivos[] = {"mouse", "teclado", "pantalla"};

        sprintf(str, "%d",(i+1));
        strcat(nombre,  str);

        //crear peticion de proceso
        NodoProceso *nodo = crearNodoProceso(i,nombre,peso,iteraciones,0,dispositivos[indice_aleatorio],tiempo);
        //insertar la peticion del proceso en la lista
        insertar(listaPeticion,nodo);

    }

    //imprimiendo lista de procesos
    mostrarListaProcesos(listaPeticion);
}

void llenarMemoriaInicio(){

    int deterner = 0;

    while( deterner == 0) {

        //asignarle espacio en memoria
        asignarEspacioDisponible(matriz, listaPeticion->primero);

        //inserta el primero en lista
        insertar(listaListos,listaPeticion->primero);

        //mover un proceso de lista peticion a lista contenedor y asinale un espacio en memoria
        pasarProcesoContenedor(listaPeticion, listaContenedor);

        //mientras la matriz no se llene mas del 60%
        if( validarLlenado(matriz) ) {
            deterner = 1;
        }
    }


}

void *administrarProcesos(void *args){

    //recibir parametro de nodo
    NodoProceso *nodoProceso = (NodoProceso *) args;

    printf("\nNombre del proceso %s\n", nodoProceso->nombre);

    if(nodoProceso->id == 4){
        banderaFinalizacion=1;
    }


    //iniciar el contexto
    //conforme llega un proceso metelo en lista de listo

    //bloqueo los hilos


    //ir atendiendo por orden, solo los hilo que estan en el cotexto - listacontenedor

    //iterar el proceso

    //solo un proceso va estar en ejecucion x tiempo

    //pasa a lista de E/S

    //pasa a lista de listos

    //pasa a ejecucion

    //validar si el proceso aun tiene iteracion

    //si no tiene iteracion sale de listaContenedor y se elimina el hilo

    //si sale un hilo se mete otro en listaContenedor


}

void *iniciarPlanificador(void *args){


    while (banderaFinalizacion==0){

        //identificar el proceso entrante
        NodoProceso *nodoProceso = listaPeticion->primero;

        //tomar un proceso de la lista de peticion y enviarlo
        if (pthread_create( &(procesos[nodoProceso->id]), NULL, &administrarProcesos,(void *)nodoProceso) != 0){
            printf("Error al crear hilos\n");
        }

        pthread_join(procesos[nodoProceso->id], NULL);


        //verificar si el llenado es igual o mayor al 65%, 1 = Si, 0 = No
//       if( validarLlenado(matriz) ) {
//            printf("\nLa memoria ha llegado aun 60%% de su capacidad\n");
//
//            NodoProceso *nodoEliminar = seleccionarNodoAleatorio(listaContenedor); // Almacenar nodo a eliminar
//            printf("\nLiberando de memoria %d bloques del proceso: %d",(nodoEliminar->peso/4), nodoEliminar->id);
//            //liberarProceso(nodoEliminar->id, matriz); // Liberar memoria
//            eliminarProcesoEsperando(listaContenedor, nodoEliminar); // Eliminar proceso de la lista
//            //mostrarMatriz(matriz); // Imprimir matriz
//
//        }

//ciclo
// eliminar nodo de listos
//sleep -> el proceso esta ejcucion
//iteraciones -1
//generar crecimiento memoria
//metodo en lista espara
//slepp -> el proceso esta esperando E/S
//veificar si el proceso aun tiene iteraciones
//si un proceso sale, se elimina un proceso de lista peticion, y se agrega en lista contenedor

        //cuando la lista de esperando se quede vacia
        if(listaPeticion->primero ==NULL){
            banderaFinalizacion = 1;

            printf("\n{---- Simulacion Finalizada -----}\n");
        }

        sleep(2); //con esto pueden alterar la velocidad con que muestran las cosas
    }

/*
    while( listaPeticion->primero != NULL && banderaFinalizacion==0){
        
        //recorrer la lista de contenedor - serian los procesos en el contexto de ejecucion
        NodoProceso *aux = listaContenedor->primero;
        while(aux != NULL) {
            // verificar si el hilo fue iniciado
            if (aux->contexto == false) {
                printf("\nEntrooooo!\n");

                //iniciar hilo por id
                int index = (aux->id - 1);
                pthread_create(&procesos[index], NULL, administrarProcesos, (void *) aux);

                aux->contexto = true;
            }
            aux = aux->siguiente;
        }

    }*/
}

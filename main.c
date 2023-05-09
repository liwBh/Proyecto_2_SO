#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "Listas/Lista.h"
#include "MatrizMemoria/Matriz.h"
#include "Logica/Logica.h"

struct Bloque matriz[8][8];
//bandera para la finalizacion de la simulacion
int banderaFinalizacion = 0;
//numero de procesos
int nProcesos = 0;
//hilo planificador - hilo administrador 2
pthread_t planificador;
pthread_t proceso;

//lista programas para ser atendidos
ListaProcesos *listaPeticion;
//lista programas contenedor
ListaProcesos *listaContenedor;

//FCFS
//lista en espera -> lista de dispositivos: pantalla, mouse, teclado
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
    //llenarMatriz( matriz);

    //Imprimir matriz
    //mostrarMatriz( matriz );

    //llenar lista de contenedor -> en base a la capacidad de la memoria
    llenarMemoriaInicio();

    //Imprimir matriz
    //mostrarMatriz( matriz );

    //mostra listas
    printf("\nProcesos restantes en la lista de solicitudes\n");
    mostrarListaProcesos(listaPeticion);
    printf("\nProcesos agregados en la lista de contexto de ejecucion\n");
    mostrarListaProcesos(listaContenedor);
    printf("\nProcesos agregados en la lista de listos\n");
    mostrarListaProcesos(listaListos);

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
    listaEspera = crearListaProcesos();
    listaListos = crearListaProcesos();
}

void llenarListaProcesosEsperando(){

    printf("\n---------Creando procesos ---------\n");

    srand(time(NULL));
    nProcesos = rand() % (30 - 25 + 1) + 25;

    //inicializar array de procesos
    //se retorna un puntero a la primer pos del vector de procesos

    for (int i = 0; i < nProcesos; ++i) {
        char str[10];
        char nombre[10] = "P-";
        int peso = rand() % 30 + 1 ;
        int iteraciones = rand() % 5 + 1;
        int indice_aleatorio = rand() % 3;
        int tiempo = (rand() % 3) + 1;
        char *dispositivos[] = {"mouse", "teclado", "pantalla"};

        sprintf(str, "%d",i);
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
        NodoProceso *nodo = clonarNodo(listaPeticion->primero);
        insertar(listaListos,nodo);

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

    printf("\n*************** Enviando proceso a Ejecucion *******************\n");
    printf("\nDatos del proceso: ID %d, Nombre %s\n",nodoProceso->id, nodoProceso->nombre);

    // eliminar nodo de listos
    eliminarNodo(listaListos,nodoProceso->id);

    //tiempo en ejecucion
    int tiempoEjecucion = (rand() % 3) + 1;
    printf("\nTiempo de Ejecucion %d segundos", tiempoEjecucion);
    sleep(tiempoEjecucion);

    //restar una iteraciones
    nodoProceso->nIteraciones = nodoProceso->nIteraciones - 1;
    printf("\nIteraciones restantes: %d",nodoProceso->nIteraciones );

    //******** generar crecimiento memoria *************

    //veificar si el proceso aun tiene iteraciones
    if(nodoProceso->nIteraciones == 0){
        printf("\n------{ El proceso: ID %d, Nombre %s ha terminado su ejecucion! } ------",nodoProceso->id, nodoProceso->nombre);
        //librerar bloques de memoria
        //liberarMemoria(nodoProceso,matriz);
        //printf("\nLiberando Memoria utilizada por el proceso");
        //mostrarMatriz(matriz);

        //printf("\nDireciones de Memoria a Liberar:  ");
        //mostrarListaPosiciones(nodoProceso->listaPosicion);
        //printf("\n");

        //agregar un proceso nuevo a contexto de ejecucion
        entrarContextoEjecucion(listaPeticion,listaContenedor,listaListos);

        //sacar de contexto un proceso sin iteraciones
        salirContextoEjecucion(listaContenedor,listaListos,nodoProceso);

        printf("\nProcesos restantes en la lista de Peticiones!");
        mostrarListaProcesos(listaPeticion);

        printf("\nProcesos restantes en la lista de Contenedor!");
        mostrarListaProcesos(listaContenedor);

    }else{
        //Agregar en lista espera
        insertar(listaEspera,nodoProceso);
    }

    //Descontar tiempo de espera de los procesos en lista espera de E/S
    continuarProcesosEspera(listaEspera,listaListos, nodoProceso->id);

    //mostrar procesos restantes en lista de listos
    printf("\n\nProcesos restantes en la lista de listos!");
    mostrarListaProcesos(listaListos);

    printf("\nProcesos restantes en la lista de E/S!");
    mostrarListaProcesos(listaEspera);

    if( listaPeticion->primero == NULL || listaListos->primero == NULL){
        printf("\n¡Condicion de finalizacion!");
        //indicar que la simulacion ha terminado
        banderaFinalizacion = 1;
    }

    return NULL;
}

void *iniciarPlanificador(void *args) {

    //recorrer la lista de listos - serian los procesos listos en el contexto de ejecucion
    NodoProceso *aux = listaListos->primero;
    while (banderaFinalizacion==0) {


        if (pthread_create(&(proceso), NULL, &administrarProcesos, (void *) listaListos->primero) != 0) {
            printf("Error al crear hilo para el proceso ID: %d\n", aux->id);
            break;
        }
        pthread_join(proceso, NULL);


        // Avanzamos al siguiente nodo
        aux = aux->siguiente;

        // Si llegamos al final de la lista, volvemos al principio
        if (aux == NULL) {
            aux = listaListos->primero;
        }

    }


}
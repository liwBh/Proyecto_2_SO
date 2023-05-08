#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "Listas/Lista.h"
#include "MatrizMemoria/Matriz.h"
#include "Logica/Logica.h"

pthread_mutex_t mutex_turno = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_turno = PTHREAD_COND_INITIALIZER;
int ordenEjecucion = 0;
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
    printf("\nProcesos agregados en la lista de listos\n");
    mostrarListaProcesos(listaListos);


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
        int peso = rand() % 30 + 1 ;
        int iteraciones = rand() % 10 + 1;
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
    //inciar con el primero id de la lista de listos
    //se valida el turno del proceso a ejecutarse
    if(ordenEjecucion == 0) {
        ordenEjecucion = listaListos->primero->id;
    }

    while(1){//mientras que no haya terminado la simulacion

        pthread_mutex_lock(&mutex_turno);//se bloquea el mutex para la validacion correcta del turno

        while(ordenEjecucion!=nodoProceso->id){
            if(banderaFinalizacion==1){
                pthread_mutex_unlock(&mutex_turno);
                return NULL;
            }
            //esperar la señal del siguiente turno
           pthread_cond_wait(&cond_turno, &mutex_turno);
        }

        if(nodoProceso->id == ordenEjecucion){
            printf("\n*************** Enviando proceso a Ejecucion *******************\n");
            printf("\nDatos del proceso: ID %d, Nombre %s",nodoProceso->id, nodoProceso->nombre);

            // eliminar nodo de listos
            NodoProceso *nodoEliminar = clonarNodo(nodoProceso);
            eliminarProcesoEsperando(listaListos,nodoEliminar);
            mostrarListaProcesos(listaListos);// Si es eliminado - aqui no se inserta de nuevo!

            //tiempo en ejecucion
            int tiempoEjecucion = (rand() % 3) + 1;
            printf("\nTiempo de Ejecucion %d segundos", tiempoEjecucion);
            sleep(tiempoEjecucion);

            //restar una iteraciones
            nodoProceso->nIteraciones = nodoProceso->nIteraciones - 1;
            printf("\nIteraciones restantes: %d",nodoProceso->nIteraciones );
            //mostrarListaProcesos(listaListos);// Si es eliminado - aqui no se inserta de nuevo!

            //******** generar crecimiento memoria *************

            //veificar si el proceso aun tiene iteraciones
            if(nodoProceso->nIteraciones == 0){
                printf("\n------{ El proceso: ID %d, Nombre %s ha terminado su ejecucion! } ------",nodoProceso->id, nodoProceso->nombre);
                //librerar bloques de memoria
                liberarMemoria(nodoProceso,matriz);
                printf("\nLiberando Memoria utilizada por el proceso");
                mostrarMatriz(matriz);
                printf("\nDireciones de Memoria a Liberar:  ");
                mostrarListaPosiciones(nodoProceso->listaPosicion);
                printf("\n");

                //eliminar de lista de listos
                eliminarProcesoEsperando(listaListos,nodoEliminar);

                //eliminar de lista de contenedor
                eliminarProcesoEsperando(listaContenedor,nodoEliminar);

                //si un proceso sale, se elimina un proceso de lista peticion, y se agrega en lista contenedor
                pasarProcesoDePeticionListos(listaPeticion, listaContenedor, listaListos, matriz);
                //pasarProcesoContenedor(listaPeticion,listaContenedor);

                printf("\nProcesos restantes en la lista de Contenedor!");
                mostrarListaProcesos(listaContenedor);
            }else{
                //Agregar en lista espera
                insertar(listaEspera,nodoProceso);
            }

            //mostrarListaProcesos(listaListos);// Si es eliminado

            //Descontar tiempo de espera de los procesos en lista espera de E/S
            continuarProcesosEspera(listaEspera,listaListos, nodoProceso->id);

            //mostrar procesos restantes en lista de listos
            printf("\n\nProcesos restantes en la lista de listos!");
            mostrarListaProcesos(listaListos);

            printf("\nProcesos restantes en la lista de E/S!");
            mostrarListaProcesos(listaEspera);

            if(nodoProceso->id==5){
                //if( listaPeticion->primero == NULL){
                printf("\n¡Condicion de finalizacion!");
                //si el mae llega aqui es porque ya se aseguro de que sea el turno correspondiente

                // Se establece la variable 'bandera finalizacioede poner, tampoco comer!!!!\n' en 1 para indicar que la simulacion ha terminado
                banderaFinalizacion = 1;
                // Se establece 'turno_actual' en -1 para detener el juego y evitar que se sigan ejecutando turnos adicionales
                ordenEjecucion = -1;
                // Se utiliza 'pthread_cond_broadcast(&cond_turno)' para notificar a todos los hilos
                // que el juego ha terminado y que deben terminar su ejecución
                pthread_cond_broadcast(&cond_turno);
                // Se libera el mutex
                pthread_mutex_unlock(&mutex_turno);
                // Se sale del ciclo 'while' usando 'break' ya que no hay más movimientos posibles
                break;

            }


            //si sale un hilo se mete otro en listaContenedor
            //ordenEjecucion = identificarOrden(listaListos, ordenEjecucion);
            ordenEjecucion = listaListos->primero->id;
            printf("\nEl proceso siguiente es: ------------------ %d",ordenEjecucion);
            pthread_cond_broadcast(&cond_turno);
            pthread_mutex_unlock(&mutex_turno);
        }

    }
    return NULL;
}

void *iniciarPlanificador(void *args){

    while(banderaFinalizacion == 0){
        //recorrer la lista de contenedor - serian los procesos en el contexto de ejecucion
        NodoProceso *aux = listaListos->primero;

        while(aux != NULL && banderaFinalizacion == 0) {
            // verificar si el hilo fue iniciado
            if (aux->contexto == false) {
                //iniciar hilo por id
                int index = (aux->id - 1);
                pthread_create(&procesos[index], NULL, administrarProcesos, (void *) aux);
                aux->contexto = true;
            }
            aux = aux->siguiente;
        }
    }

    printf("\nTermino while");

    // esperar a que terminen los hilos
    for (int i = 0; i < nProcesos; i++) {
        // se espera a que cada hilo termine su ejecución para que el programa no quede en un bulce infintito y se cierre el uso de los hilos
        pthread_join(procesos[i], NULL);
    }

    printf("\nTermino For");
    printf("\nTermino Hilo Planificador");

}
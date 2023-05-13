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
//variable para evaluar desperdicio interno de cada proceso
int desperdicioInternoTotal = 0;
//variable para evaluar desperdicio externo dentro del algoritmo de planificacion
int desperdicioExterno= 0;

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

    printf("\033[1;31m------------- Emulador de memoria Particiones Fijas y Variables ------------\033[0m\n");

    printf("\nTamaño de memoria = 256kb\n");
    printf("Tamaño del bloque = 4kb\n");

    //creando las listas del emulador
    crearListas();

    //creando los procesos del emulador
    llenarListaProcesosEsperando();

    printf("\033[1;33mInicializando estructura de Memoria\033[0m\n");

    //crear aquitectura de memoria
    llenarMatriz( matriz);

    //Imprimir matriz
    mostrarMatriz( matriz );

    //Tiempo de inicio simulacion
    time_t startTime = time(NULL);
    printf("\033[1;32m\nEl tiempo de inicio del programa es: %s\033[0m\n", ctime(&startTime));


    //llenar lista de contenedor -> en base a la capacidad de la memoria
    printf("\033[1;33m\nLlenando memoria aplicando politica de particiones fijas de varios tamaños\n\033[0m");

    llenarMemoriaInicio();

    //Imprimir matriz
    printf("\nProcesos actuales en memoria\n");
    mostrarMatriz( matriz );

    //mostrar las listas aplicando politica PFVT
    mostrarPFVT(listaContenedor);

    //mostra listas
    printf("\033[0;32m\nProcesos restantes en la lista de solicitudes\n\033[0m");

    mostrarListaProcesos(listaPeticion);
    printf("\033[1;32m\nProcesos agregados en la lista de contexto de ejecucion\n\033[0m");

    mostrarListaProcesos(listaContenedor);
    printf("\033[0;32m\nProcesos agregados en la lista de listos\n\033[0m");

    mostrarListaProcesos(listaListos);

    //Iniciar planificador
    pthread_create(&(planificador), NULL, &iniciarPlanificador, NULL);

    //inicia la simulaicon del hilo
    pthread_join(planificador, NULL);


    //Tiempo de finalizacion simulacion
    time_t endTime = time(NULL);
    printf("El tiempo de finalizacion del programa es de: %s\n", ctime(&endTime));

    printf("\033[1;31m--------{El programa ha Finalizado su Ejecucion!}---------\033[0m\n");
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
    //iniciaria con particion fija de varios tamaños
    int deterner = 0;

    while( deterner == 0) {

        //asignarle espacio en memoria
        //asignarEspacioDisponible(matriz, listaPeticion->primero);
        //asignarle espacio en memoria en base a PFVT
        asignarEspacioDisponiblePFVT( matriz,listaPeticion->primero, listaContenedor );

        //inserta el primero en lista de listos
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
  /* */ desperdicioInternoTotal = calcularDesperdicioInternoTotal(listaContenedor);
    desperdicioExterno = calcularDesperdicioExternoVector(matriz);
    printf("\n ----> cantidad de procesos en contexto de ejecucion : %d  <----", listaContenedor->tamanio);
    printf("\n----> desperdicio interno total : %d", desperdicioInternoTotal);
    printf("\n----> desperdicio externo total : %d ", desperdicioExterno);

    printf("\033[0;32m\n*************** Enviando proceso a Ejecucion *******************\n");
    printf("\nDatos del proceso: ID %d, Nombre %s\n",nodoProceso->id, nodoProceso->nombre);

   /**/ printf("\n----> bloques utilizados %d desperdicio Interno del proceso en el contexto: %d",
           encontrarCantidadDeBloques(nodoProceso->peso), calcularDesperdicioInterno(nodoProceso));

    // eliminar nodo de listos
    eliminarNodo(listaListos,nodoProceso->id);

    //tiempo en ejecucion
    int tiempoEjecucion = (rand() % 3) + 1;
    printf("\033[0;32m\nTiempo de Ejecucion %d segundos\033[0m", tiempoEjecucion);

    sleep(tiempoEjecucion);

    //restar una iteraciones
    nodoProceso->nIteraciones = nodoProceso->nIteraciones - 1;
    printf("\nIteraciones restantes: %d",nodoProceso->nIteraciones );

    //******** generar crecimiento memoria *************

    //veificar si el proceso aun tiene iteraciones
    if(nodoProceso->nIteraciones == 0){
        printf("\033[1;33m\n------{ El proceso: ID %d, Nombre %s ha terminado su ejecucion! } ------\033[0m",nodoProceso->id, nodoProceso->nombre);

        //librerar bloques de memoria
        liberarMemoria(nodoProceso,matriz);
        //printf("\nLiberando Memoria utilizada por el proceso");
        mostrarMatriz(matriz);

        printf("\033[1;33mDirecciones de Memoria a Liberar:  \033[0m");

        mostrarListaPosiciones(nodoProceso->listaPosicion);
        printf("\n");

        //asignarle espacio en memoria en base a PFVT, al proceso entrante al contexto de ejecucion
         asignarEspacioDisponiblePFVT(matriz,listaPeticion->primero,listaContenedor);

        //agregar un proceso nuevo a contexto de ejecucion
        entrarContextoEjecucion(listaPeticion,listaContenedor,listaListos);
        
        //sacar de contexto un proceso sin iteraciones del contexto de ejecucion
        salirContextoEjecucion(listaContenedor,listaListos,nodoProceso);

        //mostrar las listas de procesos en el contexto de ejecucion
        printf("\033[1;33m\nProcesos restantes en la lista de Peticiones!\n\033[0m");

        mostrarListaProcesos(listaPeticion);

        printf("\033[0;32m%s\033[0m", "Procesos restantes en la lista de Contenedor!");

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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "Listas/Lista.h"
#include "Listas/Lista_E_S.h"


pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // inicializamos la variable de condición
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // inicializamos el mutex
int num_hilos = 0; // contador de hilos que han llegado

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
void *administrarProcesos(void *args);
void *iniciarPlanificador(void *args);

int main() {

    printf("------------- Emulador de memoria Particiones Fijas y Variables ------------\n");

    //creando las listas del emulador
    crearListas();

    //creando los procesos del emulador
    llenarListaProcesosEsperando();

    exit(0);
    //temporal pasar los primeros 5
    //eliminar de uno en uno de la lista de espera y pasarlos a contenedor

    if(!listaVacia(listaPeticion)){
        printf("\nentro a repartir a lista de contenedor ");
        for(int i = 0; i < 5; i++){
            NodoProceso *nodoProceso = listaPeticion->primero;
            listaPeticion->primero = listaPeticion->primero->siguiente;
            listaPeticion->ultimo->siguiente = listaPeticion->primero;
            listaPeticion->primero->anterior=listaPeticion->ultimo;
            insertar(listaContenedor,nodoProceso);
            printf("\neliminar: %d", i);
            eliminarProcesoEsperando(listaPeticion,nodoProceso);
            printf("\nrepartio nodo %d ", i);
        }
    }else{
        printf("\nYa no hay peticiones de procesos\n");
    }

    printf("\nsalio de repartir a lista de contenedor ");
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

void *administrarProcesos(void *args){
    //recibir parametro de nodo
    NodoProceso *nodoProceso = (NodoProceso *) args;
    printf("\nProceso dentro de contexto: %s id: %d \n", nodoProceso->nombre, nodoProceso->id);

    //iniciar el contexto
    //conforme llega un proceso metelo en lista de listo
    NodoProceso *nodo = clonarNodo(nodoProceso);
    insertar(listaListos, nodo);

    pthread_mutex_lock(&mutex); // bloqueamos el mutex antes de actualizar el contador
    num_hilos++; // incrementamos el contador de hilos
    if (num_hilos == 5) { // si han entrado todos los hilos
        pthread_cond_broadcast(&cond); // enviamos una señal a todos los hilos que están esperando de que ya esten todos en listos
        // llamado al metodo mandar a ejecucion
        printf("\n ya estan todos ");
    } else {
        pthread_cond_wait(&cond, &mutex); // esperamos la señal del resto de hilos
    }
    pthread_mutex_unlock(&mutex); // liberamos el mutex antes de continua


    //bloqueo los hilos, dormir el hilo en la posicion id del arreglo de procesos

    // pthread_cond_wait(&cond_turno, &mutex_turno); // Esperar la señal del siguiente turno

    //lista de listos llena
    //if(nodoProceso->id == listaContenedor->ultimo->id){
        //tomar el primero y pasarlo a ejecucion

//        pthread_cancel(pthread_self());
//        pthread_cond_signal()
    //}

    //ir atendiendo por orden, solo los hilo que estan en el cotexto - listacontenedor


    //iterar el proceso

    //solo un proceso va estar en ejecucion x tiempo

    //pasa a lista de E/S

    //pasa a lista de listos

    //pasa a ejecucion

    //validar si el proceso aun tiene iteracion

    //si no tiene iteracion sale de listaContenedor y se elimina el hilo

    //si sale un hilo se mete otro en listaContenedor

    //if( listaPeticion->primero == NULL && listaContenedor->primero == NULL   ){
    if( nodoProceso->id == 4){
        mostrarListaProcesos(listaListos);

        printf("\n-----------Todos los procesos fueron Atendidos-----------\n");
        banderaFinalizacion= 1;
    }

}

void *iniciarPlanificador(void *args){
    printf("\n inicia el planificador \n");

    //suponiendo que aca la lista de contenedor ya tiene SOLAMENTE los elementos dentro del contexto de ejecucion, sino es asi hacer eso antes

    NodoProceso *aux = listaContenedor->primero;
    while(aux != NULL && banderaFinalizacion==0) {
        printf("\nentro al while\n");

        // verificar si el hilo fue iniciado
        if (aux->contexto == false) {
            printf("\nEnviar hilo al contexto de ejecucion!\n");
            //iniciar hilo por id
            int index = (aux->id - 1);
            //crear el hilo, mandarlo a la cola de listos con el hilo creado pero en pausa y una vez este
            pthread_create(&procesos[index], NULL, administrarProcesos, (void *) aux);
            //agregar
            aux->contexto = true;
        }
        sleep(2);
        aux = aux->siguiente;
    }
}

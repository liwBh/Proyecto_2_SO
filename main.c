#include <stdio.h>
#include <stdlib.h>
#include <time.h>
\

#include <string.h>
#include <pthread.h>
#include "Listas/Lista.h"
#include "Listas/Lista_E_S.h"

pthread_mutex_t mutex_turno = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_turno = PTHREAD_COND_INITIALIZER;

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
            printf("\neliminar", i);
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
        int peso = 0;
        int iteraciones = rand() % 10 + 1;

        do{
            peso = rand() % 40 + 1 ;
        } while (peso % 4 != 0);

        sprintf(str, "%d",(i+1));

        strcat(nombre,  str);


        //crear peticion de proceso
        NodoProceso *nodo = crearNodoProceso(i,nombre,peso,iteraciones,0,"Pantalla",10);
        //insertar la peticion del proceso en la lista
        insertar(listaPeticion,nodo);

    }

    //imprimiendo lista de procesos
    mostrarListaProcesos(listaPeticion);
}

void *administrarProcesos(void *args){

    //recibir parametro de nodo
    NodoProceso *nodoProceso = (NodoProceso *) args;
    printf("\nimprimiendo informacion dentro del metodo administrar procesos del proceso añadido a la cola de listos:"
           "  %s peso: %d id: %d \n", nodoProceso->nombre, nodoProceso->peso, nodoProceso->id);

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
    printf("\n inicia el planificador \n");

    //suponiendo que aca la lista de contenedor ya tiene SOLAMENTE los elementos dentro del contexto de ejecucion, sino es asi hacer eso antes

    NodoProceso *aux = listaContenedor->primero;


    while(aux != NULL&&banderaFinalizacion==0) {
        printf("\nentro al while");

        // verificar si el hilo fue iniciado
        if (aux->contexto == false) {
            printf("\nse envio al contexto de ejecucion!\n");
            //iniciar hilo por id
            int index = (aux->id - 1);
            //crear el hilo, mandarlo a la cola de listos con el hilo creado pero en pausa y una vez este
            // llena la cola de listos llamar al metodo de ejecutar hilo que vaya ejecutando cada proceso de la cola de listos
            pthread_create(&procesos[index], NULL, administrarProcesos, (void *) aux);
            //agregar
            aux->contexto = true;
            aux = aux->siguiente;
        }else{
            //aca llamar al metodo de ejecucion del hilo
            //cola listos a ejecutarse
            //
            printf("\nel proceso ya esta dentro del contexto!\n");
            printf("\n se llego hasta el final de la lista,");
            system("pause");
            banderaFinalizacion = 1;
        }
    }

}

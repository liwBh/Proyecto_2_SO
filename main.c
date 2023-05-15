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
//politica de administracion de memoria
int tipoPolitica = 1;
bool etiquetaC =  false; //etiqueta para habilitar aleatorio de crecimiento

//numero de procesos
int nProcesos = 0;
//hilo planificador - hilo administrador 2
//variable para evaluar desperdicio interno de cada proceso
int desperdicioInternoTotal = 0;
//variable para evaluar desperdicio externo dentro del algoritmo de planificacion
int desperdicioExterno= 0;
//variables para llevar control del promedio de procesos finalizados desde que se inicio el programa
float promedio = 0.0;
time_t inicioPrograma;
int procesosFinalizados =0;


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
    //se toma el tiempo de inicio del programa
    inicioPrograma = time(NULL);
    printf("\033[1;31m------------- Emulador de memoria Particiones Fijas y Variables ------------\033[0m\n");


    printf("\nTamaño de memoria = 256kb\n");
    printf("Tamaño del bloque = 4kb\n");

    //creando las listas del emulador
    crearListas();

    //creando los procesos del emulador
    llenarListaProcesosEsperando();

    printf("\033[1;33mInicializando estructura de Memoria\033[0m\n");
    fflush(stdout);

    //crear aquitectura de memoria
    llenarMatriz( matriz);

    //Imprimir matriz
    mostrarMatriz( matriz );
    fflush(stdout);

    //Tiempo de inicio simulacion
    time_t startTime = time(NULL);
    printf("\033[1;32m\nEl tiempo de inicio del programa es: %s\033[0m\n", ctime(&startTime));


    //llenar lista de contenedor -> en base a la capacidad de la memoria
    printf("\033[1;33m\nLlenando memoria aplicando politica de particiones fijas de varios tamaños\n\033[0m");
    llenarMemoriaInicio();

    //Imprimir matriz
    printf("\nProcesos actuales en memoria\n");
    mostrarMatriz( matriz );
    fflush(stdout);

    //mostrar las listas aplicando politica PFVT
    mostrarPFVT(listaContenedor);
    fflush(stdout);

    //mostra listas
    printf("\033[0;32m\nProcesos restantes en la lista de solicitudes\n\033[0m");
    mostrarListaProcesos(listaPeticion);
    fflush(stdout);

    printf("\033[1;32m\nProcesos agregados en la lista de contexto de ejecucion\n\033[0m");
    mostrarListaProcesos(listaContenedor);
    fflush(stdout);

    printf("\033[0;32m\nProcesos agregados en la lista de listos\n\033[0m");
    mostrarListaProcesos(listaListos);
    fflush(stdout);

    //Iniciar planificador
    pthread_create(&(planificador), NULL, &iniciarPlanificador, NULL);

    //inicia la simulaicon del hilo
    pthread_join(planificador, NULL);

    time_t endTime = time(NULL);
    printf("\033[32m\nEl tiempo de finalizacion del programa es de: %s\033[0m\n", ctime(&endTime));

    //se obtiene el promedio de procesos finalizados por segundo
    time_t fin = time(NULL); // Obtener el tiempo actual
    float tiempo_transcurrido = difftime(fin, inicioPrograma); // Calcular el tiempo transcurrido en segundos
    promedio = ((promedio * (procesosFinalizados - 1)) + tiempo_transcurrido) / procesosFinalizados; // Calcular el nuevo promedio
    printf("\nTotal de procesos finalizados: %d", procesosFinalizados);
    printf("\nTiempo total de ejecucion %f segundos", tiempo_transcurrido);
    printf("\nPromedio de proceso finalizados por unidad de tiempo: %.2f segundos\n", promedio);
    fflush(stdout);

    printf("\033[1;31m\n--------{El programa ha Finalizado su Ejecucion!}---------\033[0m\n");
    fflush(stdout);

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

    printf("\033[33m\n---------Creando procesos ---------\n\033[0m");


    srand(time(NULL));
    nProcesos = rand() % (30 - 25 + 1) + 25;

    //inicializar array de procesos
    //se retorna un puntero a la primer pos del vector de procesos

    for (int i = 0; i < nProcesos; ++i) {
        char str[10];
        char nombre[10] = "P-";
        int peso = rand() % 30 + 1 ;
        int iteraciones = rand() % 5 + 2;
        int indice_aleatorio = rand() % 3;
        int tiempo = (rand() % 3) + 1;
        char *dispositivos[] = {"mouse", "teclado", "pantalla"};

        sprintf(str, "%d",i);
        strcat(nombre,  str);

        //crear peticion de proceso
        NodoProceso *nodo = crearNodoProceso(i,nombre,peso,iteraciones,dispositivos[indice_aleatorio],tiempo);
        nodo->sumTiempoES = (0.000010 * tiempo);
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
    int idProceso = nodoProceso->id;
    desperdicioInternoTotal = calcularDesperdicioInternoTotal(listaContenedor);
    desperdicioExterno = calcularDesperdicioExternoVector(matriz);

    printf("\033[0;32m\n*************** Enviando proceso a Ejecucion *******************\n\033[0m");

    printf("\nDatos del proceso: ID %d, Nombre %s\n",nodoProceso->id, nodoProceso->nombre);

    printf("\nBloques utilizados %d, desperdicio Interno del proceso: %d kb",nodoProceso->numBloques, calcularDesperdicioInterno(nodoProceso));
    fflush(stdout);

    // eliminar nodo de listos
    eliminarNodo(listaListos,nodoProceso->id);

    //tiempo en ejecucion
    clock_t tInicio = clock();
    int tiempoEjecucion = (rand() % 3) + 1;
    sleep(tiempoEjecucion);
    clock_t tFin = clock();
    double tiempo_total = ((double) (tFin - tInicio)) / CLOCKS_PER_SEC;
    nodoProceso->sumTiempoEj += tiempo_total;
    nodoProceso->nEjecucion ++;
    printf("\nTiempo de Ejecucion %f segundos", tiempo_total);

    //restar una iteraciones
    nodoProceso->nIteraciones = nodoProceso->nIteraciones - 1;
    printf("\nIteraciones restantes: %d\n",nodoProceso->nIteraciones );

    printf("\033[33m\nInformacion de rendimiento de memoria\n\033[0m");
    printf("\nCantidad de procesos en contexto de ejecucion : %d", listaContenedor->tamanio);
    printf("\nDesperdicio interno total : %d kb", desperdicioInternoTotal);
    printf("\nDesperdicio externo total : %d kb\n", desperdicioExterno);
    fflush(stdout);

    //******** generar crecimiento memoria *************
    int crecimiento = generarCreacimientoP();
    printf("\nEl crecimiento del proceso es: %d kb\n", crecimiento);
    fflush(stdout);

    //veificar si el proceso aun tiene iteraciones
    if(nodoProceso->nIteraciones == 0){
        printf("\033[1;33m\n------{ El proceso: ID %d, Nombre %s ha terminado su ejecucion! } ------\033[0m",nodoProceso->id, nodoProceso->nombre);

        //librerar bloques de memoria
        liberarMemoria(nodoProceso,matriz);
        //printf("\nLiberando Memoria utilizada por el proceso");
        mostrarMatriz(matriz);
        fflush(stdout);

        printf("\033[1;33m\nDirecciones de Memoria a Liberar:  \033[0m");

        mostrarListaPosiciones(nodoProceso->listaPosicion);
        fflush(stdout);
        printf("\n");

        //Tipo de politicia
        switch (tipoPolitica) {
            case 1:
                //asignarle espacio en memoria en base a PFVT, al proceso entrante al contexto de ejecucion
                asignarEspacioDisponiblePFVT(matriz,listaPeticion->primero,listaContenedor);
                break;
            case 2:
                //asignarle espacio en memoria en base a PFVT, al proceso entrante al contexto de ejecucion
                printf("\nAplicar politica de Mapa de bits\n");
                break;

            default:
                printf("\033[1;31m\nOcurrio un error al aplicar politica\n033[0m");
                break;
        }

        //agregar un proceso nuevo a contexto de ejecucion
        entrarContextoEjecucion(listaPeticion,listaContenedor,listaListos);

        double promedioEj = nodoProceso->sumTiempoEj / nodoProceso->nEjecucion;
        double promedioEs = nodoProceso->sumTiempoES / nodoProceso->nEspera;
        printf("\nEl promedio de ejecucion del proceso es: %f \n",promedioEj);
        printf("\nEl promedio de espera del proceso es: %f \n",promedioEs);
        printf("\n\n");
        fflush(stdout);

        //sacar de contexto un proceso sin iteraciones del contexto de ejecucion
        salirContextoEjecucion(listaContenedor,listaListos,nodoProceso);

        //mostrar las listas de procesos en el contexto de ejecucion
        printf("\033[1;33m\nProcesos restantes en la lista de Peticiones!\033[0m");

        mostrarListaProcesos(listaPeticion);
        fflush(stdout);

        printf("\033[1;33m\nProcesos restantes en la lista de Contenedor!\033[0m");

        mostrarListaProcesos(listaContenedor);
        fflush(stdout);
        //aumentamos la cantidad de procesos finalizados
        procesosFinalizados+=1;

    }else{
        //Agregar en lista espera
        NodoProceso *nodoClon = clonarNodo(nodoProceso);
        insertar(listaEspera,nodoClon);
        nodoProceso->nEspera ++;
    }

    //Descontar tiempo de espera de los procesos en lista espera de E/S
    continuarProcesosEspera(listaEspera, listaListos, idProceso);

    //mostrar procesos restantes en lista de listos
    printf("\033[1;33m\nProcesos restantes en la lista de listos!\033[0m");
    mostrarListaProcesos(listaListos);
    fflush(stdout);

    printf("\033[1;33m\nProcesos restantes en la lista de E/S!\033[0m");
    mostrarListaProcesos(listaEspera);
    fflush(stdout);

    if( listaPeticion->primero == NULL || listaListos->primero == NULL){
        //printf("\n¡Condicion de finalizacion!");
        //indicar que la simulacion ha terminado
        banderaFinalizacion = 1;
    }

    return NULL;
}


void *iniciarPlanificador(void *args) {

    //ciclo infinito para mantener los procesos en ejecucion
    while (banderaFinalizacion==0) {

        if(listaListos->primero != NULL){

            if (pthread_create(&(proceso), NULL, &administrarProcesos, (void *) listaListos->primero) != 0) {
                printf("\033[31mError al crear hilo para el proceso ID: %d\033[0m\n", listaListos->primero->id);
                banderaFinalizacion = 1;
                break;
            }

            pthread_join(proceso, NULL);

        }else{
            printf("\033[31m:\nNo hay pocesos en la lista de listos\033[0m\n");
        }

        /* Nota: si recorro la lista, se debe estar atento de no perde la referencia luego de eliminar de listo el proceso en ejecucion */
    }
}
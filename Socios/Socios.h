//
// Created by liwbh on 21/05/23.
//

#ifndef PROYECTO_2_SO_SOCIOS_H
#define PROYECTO_2_SO_SOCIOS_H


#include <stdlib.h>      /* Para uso de Rand y Srand  */
#include <time.h>		 /* Para uso con Srand y Rand */
#include <signal.h>      /* Funcion que lee CTRL+Z    */
#include <unistd.h>      /* Para uso de Sleep         */
#include <stdio.h>       /* Para Entrada / Salida     */
#include "../Listas/Nodo.h"

#define TAMANO_MEM 256  /* Memoria Total a Utilizar        */
#define FALSE 0          /* Definimos FALSE como 0          */
#define TRUE 1           /* Definimos TRUE como 1           */

// ESTRUCTURAS:
/* Estructura para las Particiones */
typedef struct {
    int id;             /* ID de la Particiión       */
    int tamano;           /* Tamaño de la Partición    */
    int libre;         /* Indica si está libre      */
    NodoProceso *proceso;
} Particion;

/* Estructura para la Memoria */
typedef struct {
    Particion particiones[TAMANO_MEM];  /* Vector de Particiones           */
    int numParticiones;                  /* Cantidad de Particiones Creadas */
    int tamano;                           /* Tamaño de la Memoria            */
} Memoria;

// VARIABLES GLOBALES:
int primeraVez = TRUE;		  /* Indica si es la Primera vez que se Ejecuta el Programa      */
int particionID = 2;		  /* Se utiliza para asignarle un ID a las Particiones           */
int needSpace = 3;			  /* Se utiliza para indicar si un Proceso necesita Espacio      */
Memoria memoria;				  /* Variable que Representa a la Memoria dentro del Programa    */


// FUNCIONES:
void calcularMemoriaUsoSocios(){
    int cantidad=0;
    for (int i = 0; i < TAMANO_MEM; i++) {
        if(memoria.particiones[i].proceso != NULL){
            cantidad += memoria.particiones[i].proceso->numBloques;
        }
    }
    printf("\nLa cantidad de memoria en uso es: %d KB\n", cantidad);
}

void iniciarMemoria() {
    /********************************************************
        Inicializa la Memoria del Programa, creando primero
        una parición Inicial de 256kb (MEMORY_SIZE) y luego
        inicializando el resto de la Memoria en NULL
    *********************************************************/
    memoria.numParticiones = 1;	// Iniciamos con 1 Partición

    // Iniciamos la Partición 1:
    memoria.particiones[0].tamano = TAMANO_MEM;	// Le asignamos 256 KB de Memoria
    memoria.particiones[0].libre = TRUE;			// Indicamos que está Libre
    memoria.particiones[0].proceso = NULL;		// Lo iniciamos en NULL
    memoria.particiones[0].id = 1;				// Le decimos que es la Partición 1

    // Iniciamos el Resto de la Memoria en NULL
    for (int i = 1; i < TAMANO_MEM; i++) {
        memoria.particiones[i].proceso = NULL;	// No tiene Proceso Asignado
        memoria.particiones[i].libre = -1;		// No está Creada
        memoria.particiones[i].tamano = -1;			// No tiene Tamaño
        memoria.particiones[i].id = -1;			// No tiene ID
    }
}

int nuevoEspacio(int tamanoProceso) {
    /******************************************************
        Esta función se encarga de determinar que tamaño
        de Partición es el más indicado para Asignarle al
        proceso, tomando en cuenta el tamaño del Proceso
        y redondéandolo a la Potencia de 2 más cercana:
    *******************************************************/
    int potencias[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};	// Potencias de 2
    for (int i = 0; i < 9; i++) {
        /**********************************************
            Devolvemos el Tamaño del Proceso en caso
            de que este ya sea una Potencia de 2:
        ***********************************************/
        if (tamanoProceso == potencias[i]) { return tamanoProceso; }
            /**********************************************
                En caso contrario devolvemos la Potencia
                de 2 más cercana al Tamaño del Proceso
            ***********************************************/
        else if (tamanoProceso < potencias[i]) { return potencias[i]; }
    }
    return -1; // Retornamos -1 en caso de que haya ocurrido un error
}

void eliminarParticion(int pos) {
    /*************************************************
        Esta función se encarga de eliminar una
        Partición de la Memoria según su Posición,
        corriendo todas las particiones siguientes
        una posición a la Izquierda
    **************************************************/
    for (int i = pos; i < memoria.numParticiones; i++) {
        memoria.particiones[i] = memoria.particiones[i + 1];
    }
    memoria.numParticiones--; // Disminuimos la Cantidad de Particiones
}

void imprimirEstadoMemoria() {
    /****************************************************
        Esta función se encarga de imprimir el Estado
        de la Memoria por Pantalla, mostrando la ID,
        el Tamaño, el Estado y el ID del Proceso asignado
        a esa Partición.
    *****************************************************/
    printf("\n                   ESTADO DE LA MEMORIA\n");
    printf("══════════════════════════════════════════════════════════════\n");
    printf(" #\tPARTICION\tTAMAÑO\t        ESTADO\t   PROCESO\t\n");
    printf("--------------------------------------------------------------\n");
    for (int i = 0; i < memoria.numParticiones; i++) {
        /*************************************************
            Si la Partición posee un Proceso, mostramos
            por pantalla el ID y el Tamaño del mismo
        *************************************************/
        if (memoria.particiones[i].proceso != NULL) {
            printf(" %d\tPartición %d\t%d KB\t\t%s\t   %d (%d KB)\n", i + 1, memoria.particiones[i].id, memoria.particiones[i].tamano, memoria.particiones[i].libre ? "Libre" : "Ocupada", memoria.particiones[i].proceso->id, memoria.particiones[i].proceso->peso);
        }
        else {
            /*******************************************
                En caso contrario, solo mostramos las
                siglas N/A por pantalla
            ********************************************/
            printf(" %d\tPartición %d\t%d KB\t\t%s\t   %s\n", i + 1, memoria.particiones[i].id, memoria.particiones[i].tamano, memoria.particiones[i].libre ? "Libre" : "Ocupada", "N/A");
        }
    }
    printf("══════════════════════════════════════════════════════════════\n\n");
}

int memoriaVacia() {
    /***********************************************
        Esta funcion se encarga de verificar si
        todas las Particiones de la Memoria están
        Libres. De ser así Retorna TRUE
    ************************************************/
    int allFree = TRUE;	// Con esto Verificamos
    for (int i = 0; i < memoria.numParticiones; i++) {
        /**********************************************
            Si existe alguna Partición que no esté
            libre, cambiamos el valor de allFree
            por FALSE
        ***********************************************/
        if (memoria.particiones[i].libre == FALSE) {	allFree = FALSE; break; }
    }
    /************************************************
        Si todas las Particiones están Libres,
        reiniciamos las Particiones de la Memoria
        y retornamos. En caso contrario, mostramos
        un mensaje de Error por pantalla
    *************************************************/
    if (allFree) { return TRUE; }
    else { return FALSE; }
}

void asignarEspacioDisponibleS(NodoProceso *nodoProceso) {


    /******************************************************************
        Esta funcion se encarga de Asignarle Memoria a un Proceso
        pasado por parámetro.

        Lo que se hace aquí es recorrer la Memoria verificando
        si el tamaño del Proceso dado es igual al Tamaño de la
        Partición de Memoria, de ser así se le asigna esa Partición
        al Proceso. En caso contrario se divide la partición hasta
        lograr conseguir el tamaño adecuado para el Proceso.

        En caso de no encontrar una parición libre para el Proceso
        se devuelve un mensaje de Error.
    *******************************************************************/

    for (int i = 0; i < memoria.numParticiones; i++) {
        // Comprobamos que la Partición esté Libre:
        if (memoria.particiones[i].libre) {
            int nuevoTamano = nuevoEspacio(nodoProceso->peso); // Redondeamos el Tamaño del Proceso
            nodoProceso->numBloques = nuevoTamano;
            if (memoria.particiones[i].tamano == nuevoTamano) {
                /******************************************
                    Si el Tamaño del Proceso es Igual al
                    Tamaño de la Partición, asignamos el
                    Proceso a la Misma y retornamos
                *******************************************/
                imprimirEstadoMemoria();
                printf("Asignandole Memoria al Proceso %d (%d KB)...\n", nodoProceso->id, nodoProceso->peso);
                //sleep(1);
                //system("clear");
                // Aquí asignamos los Procesos:
                memoria.particiones[i].libre = FALSE; // Indicamos que la Partición está Ocupada
                NodoProceso *clon = clonarNodo(nodoProceso);
                memoria.particiones[i].proceso = clon;	  // Le asignamos el Proceso a la Partición
                imprimirEstadoMemoria();				  // Imprimimos el Estado de la Memoria
                particionID++;						  // Incrementamos el ID
                printf("Proceso %d (%d KB) asignado a la Partición %d (%d KB)\n", nodoProceso->id, nodoProceso->peso, memoria.particiones[i].id, memoria.particiones[i].tamano);
                return; // Retornamos
            }
            else if(memoria.particiones[i].tamano > nodoProceso->peso) {
                /*****************************************************
                    Si el Tamaño de la Partición es más Grande que
                    la del Proceso, dividimos la Partición en 2 y
                    reiniciamos el contador 'i' para volver a
                    verificar si ya cabe el Proceso en Memoria.
                ******************************************************/
                // Puramente Estético:
                system("clear");
                imprimirEstadoMemoria();
                printf("Dividiendo la Partición %d (%d KB).\n", memoria.particiones[i].id, memoria.particiones[i].tamano);
                sleep(1);
                system("clear");
                imprimirEstadoMemoria();
                // Aquí Dividimos:
                Particion aux;							   // Partición Auxiliar
                aux.tamano = memoria.particiones[i].tamano / 2; // Le asignamos la mitad del Tamaño de la Particion Actual
                aux.proceso = NULL;						   // Le decimos que no tiene Proceso asignado
                aux.libre = TRUE;						   // Le indicamos que está libre
                aux.id = particionID;					   // Le indicamos un ID
                if (primeraVez) {
                    /***********************************************************
                        Si es la primera vez que se corre el programa
                        se va a correr la partición principal 2 posiciones
                        a la derecha, se le va a asignar 2 veces la particion
                        auxiliar a la Memoria y se va a eliminar la primera
                        partición que había (256kb o MEMORY_SIZE
                    ************************************************************/
                    for (int k = 0; k < 2; k++) {
                        // Corremos 2 posiciones:
                        for (int j = memoria.numParticiones; j > i; j--) {
                            memoria.particiones[j] = memoria.particiones[j - 1];
                        }
                    }
                    memoria.particiones[i] = aux; 					// Asignamos la Partición
                    memoria.particiones[i + 1] = aux;				// Volvemos a Asignar
                    memoria.particiones[i + 2].tamano = TAMANO_MEM;	// Acomodamos la Partición 1
                    memoria.numParticiones++;						// Eliminamos la Partición 1
                    primeraVez = FALSE;								// Indicamos que ya no es la Primera Vez
                } else {
                    /*********************************************************
                        Si no es la Primera vez que se corre el Programa
                        se van a correr las Particiones solo 1 posición
                        a la derecha, se va a asignar la partición auxiliar
                        a la posición 'i' y se le va a cambiar el tamaño a
                        la Partición en la Posición 'i + 1'
                    **********************************************************/
                    for (int j = memoria.numParticiones; j > i; j--) {
                        // Corremos las Particiones:
                        memoria.particiones[j] = memoria.particiones[j - 1];
                    }
                    // Asignamos la particion Auxiliar:
                    memoria.particiones[i] = aux;	 // Asignamos Aux
                    memoria.particiones[i + 1] = aux; // Cambiamos el Tamaño de 'i + 1'
                    memoria.numParticiones++;		 // Incrementamos el Número de Particiones
                }
                i = -1;	// Reiniciamos el Contador del for
            }
        }
    }
    // Si no había espacio para asignar el Proceso, se muestra un mensaje de Error:
    printf("No se le pudo asignar la Memoria al Proceso %d (%d KB) por Falta de Espacio...\n", nodoProceso->id, nodoProceso->peso);
    needSpace = TRUE; // Indicamos que se Necesita Espacio
    return;
}

void liberarMemoriaSocios(int processID) {
    /****************************************************************************
        Esta funcion se encarga de Liberar la Memoria que posee asignado
        un Proceso.

        Lo que hacemos es verificar si las Particiones a la Derecha de la Actual
        tienen el mismo ID y si estas están o no libres. De estar libres, se
        libera la memoria de la Partición que posee al Proceso, se elimina la
        Partición libre y se le cambia el tamaño a la Particion multiplicando
        el tamaño actual por 2.
    ******************************************************************************/
    // Siempre y cuando la Memoria no esté Vacía:
    if (!memoriaVacia()) {
        int deleted = FALSE; // Para indicar si una Partición se Liberó
        for (int i = 0; i < memoria.numParticiones; i++) {
            // Verificamos Si la Partición posee un Proceso:
            if (memoria.particiones[i].proceso != NULL) {
                // Verificamos que el ID sea igual al del Proceso:
                if (memoria.particiones[i].proceso->id == processID) {

                    imprimirEstadoMemoria();
                    printf("Liberando la Memoria del Proceso %d (%d KB)..\n", memoria.particiones[i].proceso->id, memoria.particiones[i].proceso->peso);

                    // Aquí Liberamos la Memoria:
                    /************************************************************
                        Si la siguiente Partición está Libre, y el tamaño
                        de esta es igual al de la Partición Actual, eliminamos
                        la siguiente y le duplicamos el Tamaño a la Primera
                    *************************************************************/
                    if (memoria.particiones[i + 1].libre) {
                        if (memoria.particiones[i].id == memoria.particiones[i + 1].id) {
                            if (memoria.particiones[i].tamano == memoria.particiones[i + 1].tamano) {
                                memoria.particiones[i].tamano *= 2; // Duplicamos su Tamaño
                                eliminarParticion(i + 1);             // Eliminamos la Particion Sobrante
                                deleted = TRUE;					 // Indicamos que se Liberó
                            }
                        }
                    }
                    memoria.particiones[i].proceso = NULL; // Eliminamos el Proceso
                    memoria.particiones[i].libre = TRUE;  // Liberamos la Memoria
                    deleted = TRUE;						  // Indicamos que se Liberó
                    // Estético:
                    system("clear");
                    imprimirEstadoMemoria();
                    printf("Memoria Liberada...\n");
                    break;
                }
            }
            /******************************************************
                Con el siguiente ciclo, verificamos si existen
                Particiones Liberadas del mismo tamaño que no
                se llegaron a unir. De ser así se intentan unir:
            *******************************************************/
            for (int j = 0; j < memoria.numParticiones; j++) {
                if (memoria.particiones[j].libre && memoria.particiones[j + 1].libre) {
                    if (memoria.particiones[j].tamano == memoria.particiones[j + 1].tamano) {
                        memoria.particiones[j].tamano *= 2; // Duplicamos su Tamaño
                        eliminarParticion(j + 1); 			 // Eliminamos la Particion Sobrante
                    }
                }
            }
        }
        if (memoriaVacia()) {
            /*********************************************
                Si la memoria está vacía, se reinician
                todas las variables de la Memoria
            **********************************************/
            system("clear");     // Limpiamos Pantalla de la Consola
            iniciarMemoria();        // Reiniciamos las Particiones de la Memoria
            imprimirEstadoMemoria(); // Imprimimos el Estado de la Memoria
            printf("Toda la Memoria ha sido Liberada...\n");
        }
        /*****************************************************
            Si 'deleted' es igual a 'FALSE' significa que no
            se pudo Liberar la Memoria, por lo tanto se
            manda un mensaje de Error.
        ******************************************************/
        if (!deleted) { printf("No se pudo Liberar la Memoria del Proceso %d\n", processID); }
        return; // Retornamos
    }
}

void iniciarSocios(ListaProcesos *listaContenedor){
    iniciarMemoria();

    NodoProceso *aux = listaContenedor->primero;
    while (aux != NULL) {

        // Asignamos Memoria al Proceso:
        asignarEspacioDisponibleS(aux);

        calcularMemoriaUsoSocios();

        aux = aux->siguiente;
    }
}

int calcularDesperdicioExternoSocios(){
    int cantidad=0;
    for (int i = 0; i < TAMANO_MEM; i++) {
        if(memoria.particiones[i].proceso != NULL){
            cantidad += memoria.particiones[i].proceso->numBloques;
        }
    }

    return 256 - cantidad;
}

int calcularDesperdicioInternoSocios(NodoProceso *procesoEvaluar){
    int memoriaUsado = procesoEvaluar->numBloques;
    int peso = procesoEvaluar->peso;
    int resultado = memoriaUsado - peso;

    return resultado;
}

int calcularDesperdicioInternoTotalSocios(ListaProcesos *listaContenedor){
    int desperdicioInternoTotal = 0;
    NodoProceso *aux = listaContenedor->primero;
    printf("\n\n");
    while(aux != NULL){
        desperdicioInternoTotal+= calcularDesperdicioInternoSocios(aux);
        aux = aux->siguiente;
    }
    return desperdicioInternoTotal;
}


#endif //PROYECTO_2_SO_SOCIOS_H

//// INCLUDE / DEFINE:
//#include <semaphore.h>   /* Para uso de Semáforos     */
//#include <pthread.h>     /* Para uso de los Hilos     */
//#include <stdlib.h>      /* Para uso de Rand y Srand  */
//#include <time.h>		 /* Para uso con Srand y Rand */
//#include <signal.h>      /* Funcion que lee CTRL+Z    */
//#include <unistd.h>      /* Para uso de Sleep         */
//#include <stdio.h>       /* Para Entrada / Salida     */
//#include "Listas/Lista.h"
//#include "Logica/Logica.h"
//#include "Socios/Socios.h"
//
//
////lista programas para ser atendidos
//ListaProcesos *listaPeticion;
////lista programas contenedor
//ListaProcesos *listaContenedor;
////Lista socio
//ListaSocio  *listaSocio;
////numero de procesos
//int nProcesos = 0;
//bool iniciarSocio = false;
////bandera para la finalizacion de la simulacion
//int banderaFinalizacion = 0;
//int tipoPolitica = 7;
//
//
//
//// FUNCIONES A UTILIZAR:
////void freeMemory(int processID);		/* Liberar la Memoria Usada por un Proceso        */
////int newSpace(int processSize);		/* Determina la Potencia de 2 más cercana         */
////void allocMemory(Process *p);		/* Le Asigna Memoria a un Proceso específico      */
////void sigint_handler(int sig);       /* Se ejecuta al presionar las teclas CTRL+C      */
////void delPartition(int pos);			/* Elimina una Partición de la Memoria            */
////void socios(Memory *main);			/* Función que se Ejecuta en el Main              */
////void printMemoryStatus();			/* Imprime el Estado de la Memoria en Pantalla    */
////void initMemory();					/* Inicializa la Partición Inicial de Memoria     */
////int memoryEmpty();					/* Verifica si Todas las Particiones están Libres */
////void sysPause();                    /* Se encarga de Simular una Pausa en el Programa */
//
//
//void crearListas();
//void llenarListaProcesosEsperando();
//void llenarMemoriaInicio();
//
//
//// HILOS:
//sem_t mutex;				  /* Administrador de los Hilos en Sistemas Socios     */
//void *startSocios(void *arg); /* Función que se va a Encargar de manejar los Hilos */
//
//void *startSocios(void *arg) {
//    /***********************************************
//        Esta función es la que se encarga de la
//        Funcionalidad de cada uno de los Hilos.
//
//        Empezamos asigandole Memoria a un Proceso,
//        y aumentamos la Variable 'needSpace' que
//        utilizamos para simular que un Proceso ya
//        Termino y/o que necesita Espacio.
//
//        Cuando 'needSpace' sea divisible entre 2
//        se va a Liberar la Memoria de un Proceso.
//    ************************************************/
//    // Recibimos la ID del Proceso:
//    int ID = *(int *)arg;
//
//    // Bloqueamos el Semáforo:
//    sem_wait(&mutex);
//
//    // Asignamos Memoria al Proceso:
//    // allocMemory(&list[ID]);
//    sysPause();
//
//    // Liberamos la Memoria del Proceso:
//    if (needSpace % 2 == 0 || needSpace == TRUE) {
//        liberarMemoria(ID + 1);
//        sysPause();
//    }
//
//    // Incrementamos needSpace:
//    needSpace++;
//
//
//
//    // Liberamos el Semáforo:
//    sem_post(&mutex);
//
//    // Indicamos que ya terminó el Hilo:
//    pthread_exit(NULL);
//}
//int main() {
//
//    printf("\033[1;31m------------- Emulador de memoria Particiones Fijas y Variables ------------\033[0m\n");
//
//    printf("\nTamaño de memoria = 256kb\n");
//    printf("Tamaño del bloque = 4kb\n");
//
//    //creando las listas del emulador
//    crearListas();
//
//    //creando los procesos del emulador
//    llenarListaProcesosEsperando();
//
//    //llenar lista de contenedor -> en base a la capacidad de la memoria
//    printf("\033[1;33m\nLlenando memoria aplicando politica de Socios tamaños\n\033[0m");
//    llenarMemoriaInicio();
//
//    printf("\033[0;32m\nProcesos agregados en la lista de listos\n\033[0m");
//    mostrarListaProcesos(listaPeticion);
//
//    printf("\033[1;32m\nProcesos agregados en la lista de contexto de ejecucion\n\033[0m");
//    mostrarListaProcesos(listaContenedor);
//
//
//    if(tipoPolitica == 7 && iniciarSocio == false){
//
//        // Creamos e Inicializamos la Memoria:
//        iniciarMemoria();
//        imprimirEstadoMemoria();
//
//        iniciarSocio = true;
//    }
//
//    //lista procesos en contexto
//    NodoProceso *aux = listaContenedor->primero;
//    while (aux != NULL) {
//
//        // Asignamos Memoria al Proceso:
//        asignarMemoria( aux );
//        validarLlenado();
//        srand(time(NULL));
//        int processID = rand() % 10 + 1;
//        printf("\n\n==============================Liberar=====================: %d\n\n", processID);
//        liberarMemoria(processID);
//
//        //sysPause();
//
//        // Liberamos la Memoria del Proceso:
////        if (needSpace % 2 == 0 || needSpace == TRUE) {
////            freeMemory(aux->id);
////            sysPause();
////        }
//
//
//        // Incrementamos needSpace:
//        // needSpace++;
//
//
//        aux = aux->siguiente;
//    }
//
//
//    exit(0);
//
////    // Limpiamos Pantalla:
////    system("clear");
////
////    // Creamos e Inicializamos la Memoria:
////    initMemory();
////    printMemoryStatus();
////
////    // Creamos los Hilos y el Semáforo:
////    pthread_t hilos[NUM_PROCESS];
////    sem_init(&mutex, 0, 1);
////
////    /*************************************************************
////        Esto permite terminar el Programa en Cualquier Momento
////        con solo presionar la combinación de Teclas Ctrl + C:
////    *************************************************************/
////
////    // Registra el manejador de señal para SIGINT
////    signal(SIGINT, sigint_handler);
////    printf("Presiona Ctrl-C para Terminar el Programa en Cualquier Momento.\n");
////    sysPause();
////
////    while(1) {
////        // Creamos los Procesos:
////        int powers[9] = {2, 3, 5, 7, 28, 64, 128, 14, 9};
////        srand(time(NULL));
////        for (int i = 0; i < NUM_PROCESS; i++) {
////            int size = rand() % 9;
////            list[i].size = powers[size];
////            list[i].id = i + 1;
////        }
////
////        // Iniciamos los Hilos:
////        for (int i = 0; i < NUM_PROCESS; i++) {
////            pthread_create(&hilos[i], NULL, startSocios, &i);
////            pthread_join(hilos[i], NULL);
////        }
////    }
//
//    // Destruimos el Semáforo:
//    sem_destroy(&mutex);
//    return 0;
//}
//
//
//
//void crearListas(){
//    //Listas del emulador
//    listaContenedor = crearListaProcesos();
//    listaPeticion = crearListaProcesos();
//
//    listaSocio = crearListaSocio();
//
//}
//
//void llenarListaProcesosEsperando(){
//
//    printf("\033[33m\n---------Creando procesos ---------\n\033[0m");
//
//
//    srand(time(NULL));
//    nProcesos = rand() % (30 - 25 + 1) + 25;
//
//    //inicializar array de procesos
//    //se retorna un puntero a la primer pos del vector de procesos
//
//    for (int i = 0; i < nProcesos; ++i) {
//        char str[10];
//        char nombre[10] = "P-";
//        int peso = rand() % 30 + 1 ;
//        int iteraciones = rand() % 5 + 2;
//        int indice_aleatorio = rand() % 3;
//        int tiempo = (rand() % 3) + 1;
//        char *dispositivos[] = {"mouse", "teclado", "pantalla"};
//
//        sprintf(str, "%d",i);
//        strcat(nombre,  str);
//
//        //crear peticion de proceso
//        NodoProceso *nodo = crearNodoProceso(i,nombre,peso,iteraciones,dispositivos[indice_aleatorio],tiempo);
//        nodo->sumTiempoES = (0.000010 * tiempo);
//        //insertar la peticion del proceso en la lista
//        insertar(listaPeticion,nodo);
//
//    }
//
//    //imprimiendo lista de procesos
//    mostrarListaProcesos(listaPeticion);
//}
//
//void llenarMemoriaInicio(){
//    //iniciaria con particion fija de varios tamaños
//    int detener = 0;
//
//    int contador = 0;
//
//    while( detener == 0) {
//
//        //reasignacion de memoria, en base a la politica actual
//        banderaFinalizacion = reasignacionMemoriaXpolitica(tipoPolitica, listaPeticion->primero, listaContenedor, listaPeticion);
//
//        //agregar un proceso nuevo a contexto de ejecucion
//        entrarContextoEjecucion(listaPeticion,listaContenedor);
//        fflush(stdout);
//
//
//        contador++;
//
//
//        if( listaPeticion->primero == NULL) {
//            detener = 1;
//        }
//
//    }
//
//}
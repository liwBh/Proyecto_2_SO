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
                imprimirEstadoMemoria();
                printf("Dividiendo la Partición %d (%d KB).\n", memoria.particiones[i].id, memoria.particiones[i].tamano);
                sleep(1);
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

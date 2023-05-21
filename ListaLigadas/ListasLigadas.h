#ifndef PROYECTO_2_SO_LISTASLIGADAS_H
#define PROYECTO_2_SO_LISTASLIGADAS_H

#include <malloc.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
//variables globales que vamos a necesitar
int memoriaOcupada = 0;
int memoriaTotal = 256;
double porcentajeOcupado = 0.0;
struct Nodo* cabeza = NULL;
int ultimoProcesoIngresado = 0;
int ajusteListaLigada= 1;

// Estructura de nodo para la lista ligada
struct Nodo {
    int idProceso;//para identificar el proceso en lista
    int inicio;//donde empieza el proceso en memoria
    int tamano;//tamaño total del proceso
    bool asignado;//si esta asignado es un proceso, si esta libre es un hueco
    struct Nodo* siguiente;
};

// Función para insertar un nodo en la lista ligada
void crearMemoria() {
    struct Nodo* nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo));
    nuevoNodo->inicio = 0;
    nuevoNodo->tamano = memoriaTotal;
    nuevoNodo->asignado = false;
    nuevoNodo->idProceso= -1;
    nuevoNodo->siguiente = NULL;
    cabeza = nuevoNodo;
    porcentajeOcupado=0.0;
    printf(" ---> se ha creado la memoria tamanio %d kb <---\n\n", memoriaTotal);
}

//================================ asignacion usando el peor ajuste ================================
void asignarSegmentoPeorAjuste(int procesoID) {
    int tamanoProceso = (rand() % 32) + 1;  // Generar tamaño de proceso aleatorio entre 1 y 32
    struct Nodo* nodoActual = cabeza;
    struct Nodo* nodoPeorAjuste = NULL;
    int desperdicioMaximo = 0;

    // Recorrer la lista de nodos
    while (nodoActual != NULL) {
        // Verificar si el nodo no está asignado y tiene un tamaño suficiente para el proceso
        if (!nodoActual->asignado && nodoActual->tamano >= tamanoProceso) {
            int desperdicio = nodoActual->tamano - tamanoProceso;
            // Verificar si el desperdicio calculado es mayor que el desperdicio maximo encontrado hasta el momento
            if (desperdicio > desperdicioMaximo) {
                // Actualizar el nodo de peor ajuste y el desperdicio mínimo
                nodoPeorAjuste = nodoActual;
                desperdicioMaximo = desperdicio;
            }
        }
        nodoActual = nodoActual->siguiente;
    }
    // Verificar si se encontró un nodo de peor ajuste
    if (nodoPeorAjuste != NULL) {
        nodoPeorAjuste->asignado = true;
        // Verificar si el nodo de peor ajuste tiene un tamaño mayor al tamaño del proceso, por cualquier vara
        if (nodoPeorAjuste->tamano >= tamanoProceso) {
            // Insertar un nuevo nodo después del nodo de peor ajuste con el tamaño y ubicación adecuados
            struct Nodo* nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo));
            nuevoNodo->inicio = nodoPeorAjuste->inicio + tamanoProceso;//este va a ser inicio del hueco
            nuevoNodo->tamano = nodoPeorAjuste->tamano - tamanoProceso;//este va a ser el tamanio del hueco
            nuevoNodo->asignado = false;//se pone como hueco
            nuevoNodo->siguiente = nodoPeorAjuste->siguiente;
            nodoPeorAjuste->siguiente = nuevoNodo;
            nodoPeorAjuste->idProceso=procesoID;
        }
        // Actualizar el tamaño del nodo de peor ajuste con el tamaño del proceso
        nodoPeorAjuste->tamano = tamanoProceso;
        printf("El proceso %d ha sido asignado con tamano %d\n\n", procesoID, tamanoProceso);
        //luego que se inserta tengo que actualizar los valores de la memoria ocupada
        memoriaOcupada += tamanoProceso;
        porcentajeOcupado = (double) memoriaOcupada / memoriaTotal;
    } else {
        printf("No hay suficiente espacio para asignar el proceso %d con tamano %d\n\n", procesoID, tamanoProceso);
    }
}

//================================ asignacion usando el primer ajuste ================================
void asignarSegmentoPrimerAjuste(int procesoID) {
    int tamanoProceso = (rand() % 32) + 1;  // Generar tamaño de proceso aleatorio entre 1 y 32
    struct Nodo* nodoActual = cabeza;
    struct Nodo* nodoPrimerAjuste = NULL;

    // Recorrer la lista de nodos
    while (nodoActual != NULL) {
        // Verificar si el nodo no está asignado y tiene un tamaño suficiente para el proceso, aca deberia salirse porque es la esencia del primer ajuste
        if (!nodoActual->asignado && nodoActual->tamano >= tamanoProceso) {
            nodoPrimerAjuste = nodoActual;//guardo la referencia del hueco para luego poder avanzar al siguiente nodo
            break;
        }
        nodoActual = nodoActual->siguiente;
    }

    // Verificar si se encontró un nodo para insertar usando el primer ajuste
    if (nodoPrimerAjuste != NULL) {
        nodoPrimerAjuste->asignado = true;
        // Verificar si el nodo del primer ajuste tiene un tamaño mayor al tamaño del proceso, por cualquier vara
        if (nodoPrimerAjuste->tamano >= tamanoProceso) {
            // Insertar un nuevo nodo después del nodo del primer ajuste con el tamaño y ubicación adecuados
            struct Nodo* nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo));
            nuevoNodo->inicio = nodoPrimerAjuste->inicio + tamanoProceso;//este va a ser inicio del hueco
            nuevoNodo->tamano = nodoPrimerAjuste->tamano - tamanoProceso;//este va a ser el tamanio del hueco
            nuevoNodo->asignado = false;//se pone como hueco
            nuevoNodo->siguiente = nodoPrimerAjuste->siguiente;
            nodoPrimerAjuste->siguiente = nuevoNodo;
            nodoPrimerAjuste->idProceso=procesoID;
        }
        // Actualizar el tamaño del nodo de peor ajuste con el tamaño del proceso
        nodoPrimerAjuste->tamano = tamanoProceso;
        printf("El proceso %d ha sido asignado con tamano %d\n\n", procesoID, tamanoProceso);
        //luego que se inserta tengo que actualizar los valores de la memoria ocupada
        memoriaOcupada += tamanoProceso;
        porcentajeOcupado = (double) memoriaOcupada / memoriaTotal;
    } else {
        printf("No hay suficiente espacio para asignar el proceso %d con tamano %d\n\n", procesoID, tamanoProceso);
    }
}

//================================ asignacion usando el siguiente ajuste ================================
void asignarSegmentoSiguienteAjuste(int procesoID) {
    int tamanoProceso = (rand() % 32) + 1;  // Generar tamaño de proceso aleatorio entre 1 y 32
    struct Nodo* nodoActual = cabeza;
    struct Nodo* nodoSiguienteAjuste = NULL;

    while (nodoActual != NULL) {
        //si estoy en el ultimo proceso ingresado
        if(nodoActual->inicio==ultimoProcesoIngresado){
            // de aca para adelante empezar a recorrer buscando el primer hueco donde quepa el proceso
            while (nodoActual != NULL) {
                // Verificar si el nodo no está asignado y tiene un tamaño suficiente para el proceso
                if (!nodoActual->asignado && nodoActual->tamano >= tamanoProceso) {
                    nodoSiguienteAjuste = nodoActual;//guardo la referencia del hueco para luego poder avanzar al siguiente nodo
                    break;
                }
                nodoActual = nodoActual->siguiente;
            }
            //Si sale del while y no ha encontrado un hueco disponible y suficiente para el proceso hay que volver a recorrer
            //la lista desde el principio hasta llegar al ultimo nodo ingresado
            if(nodoActual==NULL){
                //me voy al principio de la lista y empiezo a recorrer de nuevo, hasta llegar al ultimo nodo ingresado
                nodoActual = cabeza;
                while(nodoActual->inicio!=ultimoProcesoIngresado){
                    //return y desfragmentar
                    if (!nodoActual->asignado && nodoActual->tamano >= tamanoProceso) {
                        nodoSiguienteAjuste = nodoActual;//guardo la referencia del hueco para luego poder avanzar al siguiente nodo
                        break;
                    }
                    nodoActual = nodoActual->siguiente;
                }
            }
            break;
        }else{
            nodoActual = nodoActual->siguiente;
        }
    }

    // Verificar si se encontró un nodo para insertar usando el siguiente ajuste
    if (nodoSiguienteAjuste != NULL) {
        nodoSiguienteAjuste->asignado = true;
        // Verificar si el nodo del primer ajuste tiene un tamaño mayor al tamaño del proceso, por cualquier vara
        if (nodoSiguienteAjuste->tamano >= tamanoProceso) {
            // Insertar un nuevo nodo después del nodo del primer ajuste con el tamaño y ubicación adecuados
            struct Nodo* nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo));
            nuevoNodo->inicio = nodoSiguienteAjuste->inicio + tamanoProceso;//este va a ser inicio del hueco
            nuevoNodo->tamano = nodoSiguienteAjuste->tamano - tamanoProceso;//este va a ser el tamanio del hueco
            nuevoNodo->asignado = false;//se pone como hueco
            nuevoNodo->siguiente = nodoSiguienteAjuste->siguiente;
            nodoSiguienteAjuste->siguiente = nuevoNodo;
            nodoSiguienteAjuste->idProceso=procesoID;
        }
        // Actualizar el tamaño del nodo del siguiente ajuste con el tamaño del proceso
        nodoSiguienteAjuste->tamano = tamanoProceso;
        printf("El proceso %d ha sido asignado con tamano %d\n\n", procesoID, tamanoProceso);
        //luego que se inserta tengo que actualizar los valores de la memoria ocupada
        memoriaOcupada += tamanoProceso;
        porcentajeOcupado = (double) memoriaOcupada / memoriaTotal;

        //guardo la posicion del inicio del ultimo proceso ingresado
        ultimoProcesoIngresado = nodoSiguienteAjuste->inicio;
    } else {
        printf("No hay suficiente espacio para asignar el proceso %d con tamano %d\n\n", procesoID, tamanoProceso);
    }
}

//================================ mejor usando el siguiente ajuste ================================
void asignarSegmentoMejorAjuste(int procesoID) {
    int tamanoProceso = (rand() % 32) + 1;  // Generar tamaño de proceso aleatorio entre 1 y 32
    struct Nodo* nodoActual = cabeza;
    struct Nodo* nodoMejorAjuste = NULL;
    int desperdicioMinimo = INT_MAX;

    // Recorrer la lista de nodos
    while (nodoActual != NULL) {
        // Verificar si el nodo no está asignado y tiene un tamaño suficiente para el proceso
        if (!nodoActual->asignado && nodoActual->tamano >= tamanoProceso) {
            int desperdicio = nodoActual->tamano - tamanoProceso;
            // Verificar si el desperdicio calculado es menor que el desperdicio mínimo encontrado hasta el momento
            if (desperdicio < desperdicioMinimo) {
                // Actualizar el nodo de mejor ajuste y el desperdicio mínimo
                nodoMejorAjuste = nodoActual;
                desperdicioMinimo = desperdicio;
            }
        }
        nodoActual = nodoActual->siguiente;
    }
    // Verificar si se encontró un nodo de mejor ajuste
    if (nodoMejorAjuste != NULL) {
        nodoMejorAjuste->asignado = true;
        // Verificar si el nodo de mejor ajuste tiene un tamaño mayor al tamaño del proceso, por cualquier vara
        if (nodoMejorAjuste->tamano >= tamanoProceso) {
            // Insertar un nuevo nodo después del nodo de mejor ajuste con el tamaño y ubicación adecuados
            struct Nodo* nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo));
            nuevoNodo->inicio = nodoMejorAjuste->inicio + tamanoProceso;//este va a ser inicio del hueco
            nuevoNodo->tamano = nodoMejorAjuste->tamano - tamanoProceso;//este va a ser el tamanio del hueco
            nuevoNodo->asignado = false;//se pone como hueco
            nuevoNodo->siguiente = nodoMejorAjuste->siguiente;
            nodoMejorAjuste->siguiente = nuevoNodo;
            nodoMejorAjuste->idProceso=procesoID;
        }
        // Actualizar el tamaño del nodo de mejor ajuste con el tamaño del proceso
        nodoMejorAjuste->tamano = tamanoProceso;
        printf("El proceso %d ha sido asignado con tamano %d\n\n", procesoID, tamanoProceso);
        //luego que se inserta tengo que actualizar los valores de la memoria ocupada
        memoriaOcupada += tamanoProceso;
        porcentajeOcupado = (double) memoriaOcupada / memoriaTotal;
    } else {
        printf("No hay suficiente espacio para asignar el proceso %d con tamano %d\n\n", procesoID, tamanoProceso);
    }
}

// Función para liberar un segmento de memoria asignado a un proceso
void liberarSegmento(int procesoID){
    struct Nodo *nodoActual = cabeza;
    struct Nodo *nodoAnterior = NULL;

    while (nodoActual != NULL) {
        //si el nodo actual esta asignado y el nodo actual es el id generado por el random entonces liberar esa memoria
        if (nodoActual->asignado && nodoActual->idProceso == procesoID) {
            nodoActual->asignado = false;
            printf("Se ha liberado el segmento asignado al proceso %d\n\n", procesoID);
            //se libera el porcentaje de memoria que estaba ocupando e proceso
            printf("memoria ocupada antes de liberar el proceso %d: %f\n\n", procesoID,porcentajeOcupado*100);
            memoriaOcupada -= nodoActual->tamano;
            porcentajeOcupado = (double)memoriaOcupada / memoriaTotal;
            printf("Se ha liberado memoria ocupada del proceso %d\n\n", procesoID);
            printf("memoria ocupada despues de liberar el proceso %d: %f\n\n", procesoID,porcentajeOcupado*100);
            // si se logra encontrar huecos contiguos entonces se combinan
            if (nodoAnterior != NULL && !nodoAnterior->asignado) {
                nodoAnterior->tamano += nodoActual->tamano;
                nodoAnterior->siguiente = nodoActual->siguiente;
                free(nodoActual);
                nodoActual = nodoAnterior;
            }
            if (nodoActual->siguiente != NULL && !nodoActual->siguiente->asignado) {
                nodoActual->tamano += nodoActual->siguiente->tamano;
                struct Nodo *nodoSiguiente = nodoActual->siguiente;
                nodoActual->siguiente = nodoSiguiente->siguiente;
                free(nodoSiguiente);
            }
            return;
        }else if(!nodoActual->asignado && nodoActual->idProceso == procesoID){
            printf("el proceso %d ya ha sido liberado!!!!\n\n", procesoID);
            return;
        }else{
            nodoAnterior = nodoActual;
            nodoActual = nodoActual->siguiente;
        }

    }
}

// Método para liberar la memoria ocupada por la lista de nodos
void liberarMemoriaLL() {
    //se libera memoria de todos los nodos de la lista
    struct Nodo* nodoActual = cabeza;
    while (nodoActual != NULL) {
        struct Nodo* nodoSiguiente = nodoActual->siguiente;
        free(nodoActual);
        nodoActual = nodoSiguiente;
    }
}

// Función para imprimir el estado actual de la memoria
void imprimir() {
    printf("Estado actual de la memoria:\n");
    struct Nodo *nodoActual = cabeza;

    while (nodoActual != NULL) {
        if (nodoActual->asignado) {
            printf("[Proceso %d, Inicio %d, Tamano %d] -> ", nodoActual->idProceso, nodoActual->inicio,
                   nodoActual->tamano);
        } else {
            printf("[Hueco, Inicio %d, Tamano %d] -> ", nodoActual->inicio, nodoActual->tamano);
        }
        nodoActual = nodoActual->siguiente;
    }

    printf("\n");
}

// Método para insertar en una nueva lista cuando vamos a desframegntar la memoria
void insertarLL(struct Nodo** cabezaNueva, int inicio, int tamano, bool asignado, int procesoID) {
    //asigno memoria para el nuevo nodo
    struct Nodo* nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo));
    nuevoNodo->inicio = inicio;
    nuevoNodo->tamano = tamano;
    nuevoNodo->asignado = asignado;
    nuevoNodo->idProceso = procesoID;
    nuevoNodo->siguiente = NULL;

    if (*cabezaNueva == NULL) {
        *cabezaNueva = nuevoNodo;
    } else {
        //simplemente se inserta de ultimo en la nueva lista
        struct Nodo* nodoActual = *cabezaNueva;
        while (nodoActual->siguiente != NULL) {
            nodoActual = nodoActual->siguiente;
        }
        nodoActual->siguiente = nuevoNodo;

        if(procesoID!=-1){//si el id es -1 es pq se esta agregando el hueco final
            //luego que se inserta en la nueva lista tengo que actualizar los valores de la memoria ocupada
            memoriaOcupada += tamano;
            porcentajeOcupado = (double) memoriaOcupada / memoriaTotal;
        }
    }
}

void desfragmentarMemoriaLL() {
    // Crear una nueva lista para almacenar los bloques de memoria contiguos y realizar la desfragmentacion
    printf("\n------->INICIANDO PROCESO DE DESFRAGMENTACION DE MEMORIA<------\n");
    struct Nodo* nuevaCabeza = NULL;
    //reinicio a 0 las variables del porcentaje de memoria ocupado
    memoriaOcupada = 0;
    porcentajeOcupado = 0.0;
    //variable para llevar cuenta de las nuevas posiciones de inicio luego de desfragmentar la memoria
    int inicio = 0;
    int inicioHuecoFinal = 0;
    int tamanioHuecoFinal = 0;
    // Recorrer la lista original
    struct Nodo* nodoActual = cabeza;
    while (nodoActual != NULL) {
        // Verificar si el nodo está asignado
        if (nodoActual->asignado) {
            // Insertar el nodo asignado en la nueva lista
            insertarLL(&nuevaCabeza, inicio, nodoActual->tamano, true, nodoActual->idProceso);
            //actualizo la variable inicio para que el siguiente proceso sepa donde va a iniciar
            inicio += nodoActual->tamano;
            inicioHuecoFinal+=nodoActual->tamano;
        }else{
            //si no est asignado quiere decir que es un hueco, no lo agrego a la lista, sino que lo voy guardando en el hueco final
            tamanioHuecoFinal+=nodoActual->tamano;
        }
        nodoActual = nodoActual->siguiente;
    }
    //inserto el ultimo hueco a la lista
    insertarLL(&nuevaCabeza, inicioHuecoFinal, tamanioHuecoFinal, false,-1);
    // Liberar la memoria ocupada por la lista original y ya teniendo una nueva lista de la memoria pero desfragmentada
    liberarMemoriaLL();
    // Asignar la nueva lista como la cabeza de la lista de memoria
    cabeza = nuevaCabeza;
    printf("La memoria ha sido desfragmentada.\n");
}

void imprimirAjuste(){
    switch (ajusteListaLigada) {
        case 1:
            printf(" \n el ajuste de asignacion de memoria es PEOR AJUSTE\n ");
            break;
        case 2:
            printf(" \n se ha cambiado el ajuste de asignacion de memoria, el ajuste sera PRIMER AJUSTE\n");
            break;
        case 3:
            printf(" \n se ha cambiado el ajuste de asignacion de memoria, el ajuste sera SIGUIENTE AJUSTE\n");
            break;
        case 4:
            printf(" \n se ha cambiado el ajuste de asignacion de memoria, el ajuste sera MEJOR AJUSTE\n");
            break;
        default : break;
    }
}

void iniciarListasLigadas(ListaProcesos *listaContenedor){
    //inicializa la estructura de la memoria
    crearMemoria();
    //recorrer la lista e insertar proceso a proceso en la cabeza de la lista ligada
    printf("\n------->INICIANDO PROCESO DE TRASLADO LISTAS LIGADAS<------\n");
    //variables para llevar cuenta de las nuevas posiciones de inicio al insertar nuevos procesos
    int inicio = 0;
    int inicioHuecoFinal = 0;
    int tamanioHuecoFinal = 0;
    NodoProceso *procesoActual = listaContenedor->primero;

    while(procesoActual!=NULL){
        insertarLL(&cabeza, inicio, procesoActual->peso, true, procesoActual->id);
        //actualizo la variable inicio para que el siguiente proceso sepa donde va a iniciar
        inicio += procesoActual->peso;
        inicioHuecoFinal+=procesoActual->peso;
        tamanioHuecoFinal+=procesoActual->peso;
        procesoActual= procesoActual->siguiente;
    }
    //inserto el ultimo hueco a la lista
    insertarLL(&cabeza, inicioHuecoFinal, tamanioHuecoFinal, false,-1);
    printf(" TRASLADO A LISTAS LIGADAS CONCRETADO .\n");
    imprimir();
}

void asignarEspacioDisponibleLL(int idProceso){
    //cuando supere el 0.7 desfragmentar
    //cambio de politica
    do {
        //politica de adminsitracion lista ligadas
        switch (ajusteListaLigada) {
            case 1:
                asignarSegmentoPeorAjuste(idProceso);
                break;
            case 2:
                asignarSegmentoPrimerAjuste(idProceso);
                break;
            case 3:
                asignarSegmentoSiguienteAjuste(idProceso);
                break;
            case 4:
                asignarSegmentoMejorAjuste(idProceso);
                break;
            default : break;
        }

        //Imprimir el esta de la memoria actual
        imprimir();

        /*if ((procesosAsignados % 5) == 0) {
            for(int i = 0; i<3; i++){
                int procesoAEliminar = (rand() % procesosAsignados) + 1;
                liberarSegmento(procesoAEliminar);
            }
            imprimir();
        }*/

        //Enviar una señal para liberar memoria de proceso saliente


        //procesosAsignados++;


        printf("\nporcentaje de memoria ocupado hasta el momento %f \n", porcentajeOcupado*100);
        //si el porcentaje ocupado es mayor que el 70 se desfragmenta y se cambia de ajuste
        if(porcentajeOcupado>=0.7){
            if(ajusteListaLigada>4){
                return ;
            }
            //se desfragmenta, una vez desfragmentada se imprime y se cambia de ajuste
            desfragmentarMemoriaLL();
            imprimir();
            ajusteListaLigada++;
            imprimirAjuste();
            //elimino procesos para seguir la simulacion
            /*
             for(int i = 0; i<4; i++){
                int procesoAEliminar = (rand() % procesosAsignados) + 1;
                liberarSegmento(procesoAEliminar);
             }
             */
            //Enviar una señal para liberar memoria de proceso saliente
        }

        system("pause");

    }while (porcentajeOcupado < 0.7);
}

/*int main() {
    // Inicializar la lista ligada con un hueco que representa toda la memoria disponible
    crearMemoria(&cabeza, 0, 256, false,0);


    srand(time(NULL));  // Inicializar la semilla para la generación de números aleatorios
    imprimirAjuste();


    //cuando supere el 0.7 desfragmentar
    //cambio de politica
    do {
        switch (ajusteListaLigada) {
            case 1:
                asignarSegmentoPeorAjuste(procesosAsignados);
                break;
            case 2:
                asignarSegmentoPrimerAjuste(procesosAsignados);
                break;
            case 3:
                asignarSegmentoSiguienteAjuste(procesosAsignados);
                break;
            case 4:
                asignarSegmentoMejorAjuste(procesosAsignados);
                break;
            default : break;
        }
        imprimir();
        if ((procesosAsignados % 5) == 0) {
            for(int i = 0; i<3; i++){
                int procesoAEliminar = (rand() % procesosAsignados) + 1;
                liberarSegmento(procesoAEliminar);
            }
            imprimir();
        }
        procesosAsignados++;
        printf("\nporcentaje de memoria ocupado hasta el momento %f \n", porcentajeOcupado*100);
        //si el porcentaje ocupado es mayor que el 70 se desfragmenta y se cambia de ajuste
        if(porcentajeOcupado>=0.7){
            if(ajusteListaLigada>4){
                return 0;
            }
            //se desfragmenta, una vez desfragmentada se imprime y se cambia de ajuste
            desfragmentarMemoria();
            imprimir();
            ajusteListaLigada++;
            imprimirAjuste();
            //elimino procesos para seguir la simulacion
            for(int i = 0; i<4; i++){
                int procesoAEliminar = (rand() % procesosAsignados) + 1;
                liberarSegmento(procesoAEliminar);
            }
        }
        system("pause");
    }while (porcentajeOcupado < 0.7);

    return 0;
}*/
#endif //PROYECTO_2_SO_LISTASLIGADAS_H

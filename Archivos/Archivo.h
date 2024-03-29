//
// Created by liwbh on 13/05/23.
//

#ifndef PROYECTO_2_SO_ARCHIVO_H
#define PROYECTO_2_SO_ARCHIVO_H
#include <stdio.h>
#include "../Listas/Nodo.h"
#include <gtk/gtk.h>
#include <glib.h>
#include "../Logica/Logica.h"

char *obtenerRutaRelativa(char* ruta) {
    char *currentDir = g_get_current_dir(); //ruta del proyecto
    char newFilePath[200]; //variable para depurar la ruta del proyecto
    memset(newFilePath, '\0', sizeof(newFilePath));
    char *subStr = "/cmake-build-debug"; //cadena a eliminar
    char *pos = strstr(currentDir, subStr);

    //manejo de errores al depurar la cadena de la ruta
    if (pos != NULL) {
        strncpy(newFilePath, currentDir, pos - currentDir);
        newFilePath[pos - currentDir] = '\0';
        strcat(newFilePath, pos + strlen(subStr));
    } else {
        strcpy(newFilePath, currentDir);
    }

    //armando la ruta absoluta
    char *rutaAbsoluta = malloc(strlen(newFilePath) + strlen("/Archivos/") + strlen(ruta) + 1);
    strcpy(rutaAbsoluta, newFilePath);
    strcat(rutaAbsoluta, "/Archivos/");
    strcat(rutaAbsoluta, ruta);


    return rutaAbsoluta;
}

int crearArchivo(char* ruta){
    //"playbin uri=file:////home/liwbh/CLionProjects/Proyecto-01-SO/Sonidos/victoria.wav"
    // usando el modo w crea el archivo, si ya hay uno con ese nombre lo sobre escribe "../Archivos/log.txt"


    FILE *archivo = fopen(obtenerRutaRelativa(ruta), "w");

    if(archivo == NULL){
        //mensaje de error
        printf("Ocurrio un error, el archivo no fue creado con exito!\n\n");

        //detener el metodo
        return 0;
    }

    //mensaje de exito
    printf("\nEl archivo a sido creado con exito!\n\n");

    //cerrar el archivo
    fclose(archivo);

    return 1;
}


int escribirArchivo(ListaProcesos *listaListos, ListaProcesos *listaEspera,  int tipoPolitica,char *nombreArchivo, int *encabezadoEscrito) {
    FILE *archivo;

    archivo = fopen(obtenerRutaRelativa(nombreArchivo), "a");

    if (archivo == NULL) {
        printf("Ocurrió un error, el archivo no fue encontrado!\n\n");
        return 0;
    }

    fprintf(archivo,"══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════");
    fprintf(archivo, "\n                                                                    %s\n", nombreArchivo);
    fprintf(archivo,"══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n\n");

    //se escribe el encabezado del archivo

    fprintf(archivo, "ID:       Nombre:          Peso:      Entrada/Salida:  Tiempo de E/S:  Desperdicio Interno:    Tiempo Promedio Espera:  Tiempo Promedio Ejecución:\n");
    fprintf(archivo,"══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    *encabezadoEscrito = 1;


    //se recorre la lista de procesos en ejecucion(lista de listos y lista de espera) y se registra
    NodoProceso *nodo = listaListos->primero;
    while( nodo != NULL ){

        int desperdicioInterno = tipoPolitica < 3 ? calcularDesperdicioInterno(nodo) :  0;


        double tiempoPromedioEjecucion = nodo->nEjecucion!=0 ? nodo->sumTiempoEj / nodo->nEjecucion : 0;
        double tiempoPromedioEspera = nodo->nEspera!=0 && nodo->sumTiempoES!=0 ? nodo->sumTiempoES / nodo->nEspera : 0;

        fprintf(archivo, "%-10d %-18s %-10d %-16s %-16d %-22d %-23f %-25f\n",
                nodo->id, nodo->nombre, nodo->peso, nodo->nombreE_S, nodo->tiempoE_S, desperdicioInterno,  tiempoPromedioEspera, tiempoPromedioEjecucion);
        fprintf(archivo, "----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n");


        nodo = nodo->siguiente;
    }

    nodo = listaEspera->primero;
    while( nodo != NULL ){

        int desperdicioInterno = tipoPolitica < 3 ? calcularDesperdicioInterno(nodo) :  0;

        double tiempoPromedioEjecucion = nodo->nEjecucion!=0 ? nodo->sumTiempoEj / nodo->nEjecucion : 0;
        double tiempoPromedioEspera = nodo->nEspera!=0 && nodo->sumTiempoES!=0 ? nodo->sumTiempoES / nodo->nEspera : 0;

        fprintf(archivo, "%-10d %-18s %-10d %-16s %-16d %-22d %-23f %-25f\n",
                nodo->id, nodo->nombre, nodo->peso, nodo->nombreE_S, nodo->tiempoE_S, desperdicioInterno,  tiempoPromedioEspera, tiempoPromedioEjecucion);
        fprintf(archivo, "----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n");


        nodo = nodo->siguiente;
    }


    fclose(archivo);
    return 1;
}




void agregarBloqueRendimientoGeneral(char *nombreArchivo,double desperdicioExterno, int totalProcesosFinalizados, double tiempoTotalEjecucion, int procesosEjecucion) {
    FILE *archivo;

    archivo = fopen(obtenerRutaRelativa(nombreArchivo), "a");

    if (archivo == NULL) {
        printf("Ocurrió un error, el archivo no fue encontrado!\n\n");
        return;
    }

    double promedioFinal =  (totalProcesosFinalizados + 1) / tiempoTotalEjecucion ;

    fprintf(archivo,"══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════");
    fprintf(archivo, "\n                                                                Rendimiento general:\n");
    fprintf(archivo,"══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    fprintf(archivo, "                                                          Desperdicio Externo Total: %f\n", desperdicioExterno);
    fprintf(archivo, "                                                          Total de procesos finalizados: %d\n", totalProcesosFinalizados);
    fprintf(archivo, "                                                          Total de procesos en Ejecucion: %d\n", procesosEjecucion);
    fprintf(archivo, "                                                          Tiempo de ejecucion de la politica: %f segundos\n", tiempoTotalEjecucion);
    fprintf(archivo, "                                                          Promedio de proceso finalizados por unidad de tiempo: %.6lf segundos\n", promedioFinal);
    fprintf(archivo,"══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n\n");

    fclose(archivo);
}
void leerArchivoVista(GtkTextBuffer *buffer,char *prueba) {
    FILE *archivo;
    char linea[256];

    // Abrir el archivo en modo lectura
    archivo = fopen(obtenerRutaRelativa(prueba), "r");
    if (archivo == NULL) {
        g_print("Error al abrir el archivo.\n");
        return;
    }

    // Leer el archivo línea por línea
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        // Agregar la línea al buffer de texto de GTK
        gchar *tabulacion = g_strjoin("\t", linea, NULL);
        gtk_text_buffer_insert_at_cursor(buffer, tabulacion, -1);
        g_free(tabulacion);
    }

    // Cerrar el archivo
    fclose(archivo);
}

#endif //PROYECTO_2_SO_ARCHIVO_H

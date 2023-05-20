//
// Created by liwbh on 13/05/23.
//

#ifndef PROYECTO_2_SO_ARCHIVO_H
#define PROYECTO_2_SO_ARCHIVO_H
#include <stdio.h>
#include "../Listas/Nodo.h"
#include <gtk/gtk.h>
#include <glib.h>

int crearArchivo(char* ruta){
    //"playbin uri=file:////home/liwbh/CLionProjects/Proyecto-01-SO/Sonidos/victoria.wav"
    // usando el modo w crea el archivo, si ya hay uno con ese nombre lo sobre escribe "../Archivos/log.txt"
    FILE *archivo = fopen(ruta, "w");

    if(archivo == NULL){
        //mensaje de error
        printf("Ocurrio un error, el archivo no fue creado con exito!\n\n");

        //cerrar el archivo
        fclose(archivo);

        //detener el metodo
        return 0;
    }

    //mensaje de exito
    printf("\nEl archivo a sido creado con exito!\n\n");

    //cerrar el archivo
    fclose(archivo);

    return 1;
}
int escribirArchivo(NodoProceso *nodo, int desperdicioInterno, int desperdicioExterno, const char *nombreArchivo) {
    FILE *archivo;

    //inicializar archivo, modo agregar
    archivo = fopen(nombreArchivo, "a");

    //validar si el archivo existe
    if(archivo == NULL) {
        //mensaje de error
        printf("Ocurrio un error, el archivo no fue encontrado!\n\n");

        //cerrar el archivo
        fclose(archivo);

        //detener metodo
        return 0;
    }


    static int encabezadoEscrito = 0; // Variable de control del encabezado

    if (encabezadoEscrito == 0) {
        char encabezado[] = "Analisis de Rendimiento de Registros de Particiones Fijas";
        int longitudEncabezado = strlen(encabezado);
        int anchoArchivo = 80; // Ancho total del archivo (ajusta este valor según tus necesidades)
        int espacios = (anchoArchivo - longitudEncabezado) / 2;

        // Escribir los espacios en blanco antes del encabezado
        for (int i = 0; i < espacios; i++) {
            fprintf(archivo, " ");
        }

        fprintf(archivo, "%s\n", encabezado);
        fprintf(archivo, "-------------------------------------------------------------------------------------------------------------------\n");
        fprintf(archivo, "ID:       Nombre:          Peso:      Entrada/Salida:  Tiempo de E/S:  Desperdicio Interno:  Desperdicio Externo:\n");
        fprintf(archivo, "-------------------------------------------------------------------------------------------------------------------\n");
        encabezadoEscrito = 1; // Actualizar el valor de la variable para indicar que el encabezado ya ha sido escrito
    }

    fprintf(archivo, "%-10d %-18s %-10d %-16s %-16d %-22d %-20d\n",
            nodo->id, nodo->nombre, nodo->peso, nodo->nombreE_S, nodo->tiempoE_S, desperdicioInterno, desperdicioExterno);
    fprintf(archivo, "-------------------------------------------------------------------------------------------------------------------\n\n");

    fclose(archivo);
    return 1;
}


void leerArchivoVista(GtkTextBuffer *buffer, const char *prueba) {
    FILE *archivo;
    char linea[256];

    // Abrir el archivo en modo lectura
    archivo = fopen(prueba, "r");
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

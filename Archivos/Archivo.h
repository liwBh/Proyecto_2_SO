//
// Created by liwbh on 13/05/23.
//

#ifndef PROYECTO_2_SO_ARCHIVO_H
#define PROYECTO_2_SO_ARCHIVO_H
#include <stdio.h>
#include "../Listas/Nodo.h"
#include <gtk/gtk.h>


void escribirArchivo(NodoProceso *nodo) {
    FILE *archivo;
    archivo = fopen("procesos.txt", "a");

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    fprintf(archivo, "ID: %d\n", nodo->id);
    fprintf(archivo, "Nombre: %s\n", nodo->nombre);
    fprintf(archivo, "Peso: %d\n", nodo->peso);
    fprintf(archivo, "Iteraciones: %d\n", nodo->nIteraciones);
    fprintf(archivo, "Entrada/Salida: %s\n", nodo->nombreE_S);
    fprintf(archivo, "Tiempo de E/S: %d\n", nodo->tiempoE_S);
    fprintf(archivo, "Lista PFVT: %d\n", nodo->listaPFVT);

    fclose(archivo);
}

void leerArchivo() {
    FILE *archivo;
    NodoProceso nodo;
    archivo = fopen("procesos.txt", "r");

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    while (fscanf(archivo, "ID: %d\n", &nodo.id) != EOF) {
        fscanf(archivo, "Nombre: %[^\n]\n", nodo.nombre);
        fscanf(archivo, "Peso: %d\n", &nodo.peso);
        fscanf(archivo, "Iteraciones: %d\n", &nodo.nIteraciones);
        fscanf(archivo, "Entrada/Salida: %[^\n]\n", nodo.nombreE_S);
        fscanf(archivo, "Tiempo de E/S: %d\n", &nodo.tiempoE_S);
        fscanf(archivo, "Lista PFVT: %d\n", &nodo.listaPFVT);


        // Aquí puedes hacer lo que quieras con el nodo leído
    }

    fclose(archivo);
}


// Función para leer el archivo de texto
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
        gtk_text_buffer_insert_at_cursor(buffer, linea, -1);
    }

    // Cerrar el archivo
    fclose(archivo);
}



#endif //PROYECTO_2_SO_ARCHIVO_H

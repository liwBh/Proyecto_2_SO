//
// Created by liwbh on 13/05/23.
//

#ifndef PROYECTO_2_SO_ARCHIVO_H
#define PROYECTO_2_SO_ARCHIVO_H
#include <stdio.h>
#include "../Listas/Nodo.h"
#include <gtk/gtk.h>
#include <glib.h>


void escribirArchivo(NodoProceso *nodo, int desperdicioInterno, int desperdicioExterno, const char *nombreArchivo) {
    FILE *archivo;
    archivo = fopen(nombreArchivo, "a");

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
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

void leerArchivoVista1(GtkTextBuffer *buffer, const char *prueba) {
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
        // Aplicar formato de ancho fijo a los campos
        int id, peso, tiempoE_S, desperdicioInterno, desperdicioExterno;
        char nombre[50], nombreE_S[50];
        sscanf(linea, "%d %s %d %s %d %d %d", &id, nombre, &peso, nombreE_S, &tiempoE_S, &desperdicioInterno, &desperdicioExterno);

        // Crear la línea de texto con formato de ancho fijo
        char formattedLine[256];
        g_snprintf(formattedLine, sizeof(formattedLine), "ID: %2d\tNombre: %-10s\tPeso: %2d\tEntrada/Salida: %-10s\tTiempo de E/S: %2d\tDesperdicio Interno: %3d\tDesperdicio Externo: %3d\n",
                   id, nombre, peso, nombreE_S, tiempoE_S, desperdicioInterno, desperdicioExterno);

        // Agregar la línea al buffer de texto de GTK
        gtk_text_buffer_insert_at_cursor(buffer, formattedLine, -1);
    }

    // Cerrar el archivo
    fclose(archivo);
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

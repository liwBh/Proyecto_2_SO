//
// Created by liwbh on 13/05/23.
//

#ifndef PROYECTO_2_SO_ARCHIVO_H
#define PROYECTO_2_SO_ARCHIVO_H
#include <stdio.h>
#include "../Listas/Nodo.h"

/*
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
    fprintf(archivo, "Ejecución: %s\n", nodo->ejecucion ? "Sí" : "No");
    fprintf(archivo, "Entrada/Salida: %s\n", nodo->nombreE_S);
    fprintf(archivo, "Tiempo de E/S: %d\n", nodo->tiempoE_S);
    fprintf(archivo, "Contexto: %s\n", nodo->contexto ? "Sí" : "No");
    fprintf(archivo, "Lista PFVT: %d\n", nodo->listaPFVT);
    fprintf(archivo, "Inicio: %d\n", nodo->inicio);
    fprintf(archivo, "Finalización: %d\n", nodo->finalizacion);

    fclose(archivo);
}
*/


/*
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

        char ejecucion[5];
        fscanf(archivo, "Ejecución: %s\n", ejecucion);
        nodo.ejecucion = (strcmp(ejecucion, "Sí") == 0);

        fscanf(archivo, "Entrada/Salida: %[^\n]\n", nodo.nombreE_S);
        fscanf(archivo, "Tiempo de E/S: %d\n", &nodo.tiempoE_S);

        char contexto[5];
        fscanf(archivo, "Contexto: %s\n", contexto);
        nodo.contexto = (strcmp(contexto, "Sí") == 0);

        fscanf(archivo, "Lista PFVT: %d\n", &nodo.listaPFVT);
        fscanf(archivo, "Inicio: %d\n", &nodo.inicio);
        fscanf(archivo, "Finalización: %d\n", &nodo.finalizacion);

        // Aquí puedes hacer lo que quieras con el nodo leído
    }

    fclose(archivo);
}*/

#endif //PROYECTO_2_SO_ARCHIVO_H

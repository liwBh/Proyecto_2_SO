

#ifndef QUIZ_SO_MATRIZ_H
#define QUIZ_SO_MATRIZ_H
#include <stdio.h>
#include <stdbool.h>

//Estructura de los elementros de la matriz
struct Bloque {
    int disponible; //Muestra si la posicion de la matriz esta disponible
    int idProceso; //Almacena el id del proceso al que esta ligado
} Bloque;

//Recorre la matriz y llena inicializando i, j en 0
void llenarMatriz(struct Bloque matriz[8][8]){

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            matriz[i][j].disponible = 0;
            matriz[i][j].idProceso = 0;
        }
    }
}

//Recorre la matriz y retorna si esta disponible la posicion
void mostrarMatriz(struct Bloque matriz[8][8]){
    printf("\n\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("[ %d ]", matriz[i][j].disponible );
        }
        printf("\n");
    }
}

//Metodo que valida si la matriz asigno un 75% de su espacio
bool validarLlenado( struct Bloque matriz[8][8] ){
    int contador = 0;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(matriz[i][j].disponible == 1){
                contador ++;
                if(contador >= 32){
                    return true;
                }
            }
        }
    }
    return false;
}

//Libera los procesos de la matriz por medio de un ID del proceso
void liberarProceso(int idProceso, struct Bloque matriz[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (matriz[i][j].idProceso == idProceso) {
                matriz[i][j].disponible = 0;
                matriz[i][j].idProceso = 0;
            }
        }
    }
}


#endif //QUIZ_SO_MATRIZ_H
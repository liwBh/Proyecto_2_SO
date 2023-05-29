

#include <stdio.h>
#include <stdlib.h>

int main() {

    remove("exe");
    system("clear");
    //system("gcc main.c -o exe");
    system("gcc main.c -o exe `pkg-config --cflags --libs gtk+-3.0 gstreamer-1.0`");
    //system("./exe > historial.txt");//crea archivo si no existe y guarda todo lo impreso en consola y no muestra en terminal
    //system("./exe | tee historial.txt");//crea archivo si no existe y guarda todo lo impreso en consola, y muestra en terminal
    system("./exe");
    return 0;
}
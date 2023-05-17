
#include <stdio.h>
#include <stdlib.h>

int main() {

    remove("exe");
    system("clear");

    system("gcc main.c -o exe");
    system("./exe");
    return 0;
}


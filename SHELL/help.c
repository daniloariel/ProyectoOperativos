#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/*
    Proyecto Nro 1, Sistemas Operativos
    Comisión: Escudero Johanna, LU: 105868
              Pesce Danilo, LU: 104936

    Ejercicio 1.1.2.f Mini Shell. Mostrar ayuda con los comandos posibles 
*/

/*
    Funcion help
*/

int main(int argc, char *argv[]){

    if(argv[0]!=NULL){
        printf("Error, el comando help no tiene argumentos\n");
    }else{

        printf("Ayuda de la Mini Shell\n");
        printf("Lista de comandos disponibles:\n");
        printf("1) mkdir (1)\n");
        printf("2) rmdir (1)\n");
        printf("3) touch (1)\n");
        printf("4) ls\n");
        printf("5) cat (1)\n");
        printf("6) help\n");
        printf("7) pwd\n");
        printf("8) exit\n");
    }
}
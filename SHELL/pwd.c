#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define max 100


/*
    Proyecto Nro 1, Sistemas Operativos
    Comisión: Escudero Johanna, LU: 105868
              Pesce Danilo, LU: 104936

    Ejercicio 1.1.2.(extra) Mini Shell. Muestra la ruta actual del directorio donde estoy situado
*/

/*
    Funcion pwd
*/

int main(int argc, char *argv[]){

    char * ruta;

    // si no tengo argumentos
    if(argv[0]==NULL){
        ruta = getcwd(NULL,max);
        printf("%s\n",ruta);
    }
    else
        printf("El comando 'pwd' no recibe argumentos\n");
    
    return 0;
}

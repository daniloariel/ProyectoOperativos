#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>



/*
    Proyecto Nro 1, Sistemas Operativos
    Comisión: Escudero Johanna, LU: 105868
              Pesce Danilo, LU: 104936

    Ejercicio 1.1.2.d Mini Shell. Listar el contenido de un directorio
*/

/*
	Funcion listar directorio ls(0)
*/

int main(int argc, char *argv[]){

	if(argv[0]!=NULL)
    printf("Este comando no lleva parámetros");

    else{

        char *pointer=NULL;
        DIR *dp=NULL; // direccion.

        struct dirent *sd=NULL; // elemento del directorio.

        pointer = getenv("PWD");
        dp=opendir((const char*)pointer);

        while((sd=readdir(dp))!=NULL)
            printf("%s  \t ",sd->d_name);

        printf("\n");
    }

    return 0;
}

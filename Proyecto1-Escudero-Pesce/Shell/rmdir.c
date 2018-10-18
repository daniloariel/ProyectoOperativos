#include <stdio.h>
#include <unistd.h>

/*
    Proyecto Nro 1, Sistemas Operativos
    Comisión: Escudero Johanna, LU: 105868
              Pesce Danilo, LU: 104936

    Ejercicio 1.1.2.b Mini Shell. Eliminar un directorio (vacio)
*/


/*
	Funcion rmdir(1)
*/

int main(int argc, char *argv[]){

	int error;

	// Si hay un solo argumento se elimina el directorio
    if(argv[0]!=NULL && argv[1]==NULL)
        error = rmdir(argv[0]);
    else
        printf("Cantidad incorrecta de argumentos\n");


    if(error!=0)
        printf("No se pudo borrar el directorio\n");
    else
        printf("Se ha borrado correctamente el directorio\n");

	return 0;
}

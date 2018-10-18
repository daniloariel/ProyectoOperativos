#include <stdio.h>
#include <unistd.h>


/*
    Proyecto Nro 1, Sistemas Operativos
    Comisión: Escudero Johanna, LU: 105868
              Pesce Danilo, LU: 104936

    Ejercicio 1.1.2.c Mini Shell. Crear un archivo
*/

/*
    Funcion crear archivo 
*/

int main(int argc, char *argv[]){

    if(argv[0]!=NULL && argv[1]== NULL){
            FILE * archivo;
            archivo = fopen(argv[0],"w");
            if(archivo==NULL)
                printf("No se pudo crear el archivo");
            else
                printf("El archivo se ha creado satisfactoriamente");
            fclose(archivo);
    }
    else
        printf("Cantidad incorrecta de argumentos\n");

    return 0;
}

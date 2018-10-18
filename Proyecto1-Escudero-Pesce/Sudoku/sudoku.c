#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>


/*
    Proyecto Nro 1, Sistemas Operativos
    Comisión: Escudero Johanna, LU: 105868
              Pesce Danilo, LU: 104936

    Ejercicio 1.1.1.c.1 Ejercicio del Sudoku resuelto con Procesos
*/


#define size 9

int verificarFilas(int **matriz, int c){

    int pos,i,k;
    c = c%9;
    int arreglo[size];

    for(k = 0 ; k<size ; k++)
        arreglo[k] = 0;

    for(i = 0; i<size; i++){
        pos = matriz[i][c];
        if(arreglo[pos-1]!= 0)
            return 1;
        else
            arreglo[pos-1] = pos;
    }
    return 0;
}

int verificarColumnas(int **matriz, int f){

    int j,pos,k;
    f=f%9;
    int arreglo[size];

    for(k = 0; k<size; k++)
        arreglo[k] = 0;

    for(j = 0; j<size; j++){
        pos= matriz[f][j];
        if(arreglo[pos-1]!=0)
            return 1;
        else
            arreglo[pos-1]= pos;
    }
    return 0;
}

int verificarCuadrantes(int **matriz, int c){
    //acá verifico que no se repitan los numeros en un cuadrante.
    //hay que hacer c%9

    int arreglo[size];
    int i,j,k,pos;
    c= c%9;

    for(k = 0; k<size; k++)
        arreglo[k] = 0;

    for(i = 0; i < 3; i++)
        for(j = 0 ; j < 3; j++){
            pos = matriz[((c/3)*3)+i][((c%3)*3)+j];
            if(arreglo[pos-1]!=0)
                return 1;
            else
                arreglo[pos-1]= pos;
        }
    return 0;
}

int main(){

    FILE * archivo = fopen("sudoku.txt","r");

    if(archivo == NULL){

        fputs ("File error",stderr);
        exit (1);
    }

    int filas = 9;
    int columnas = 9;

    //Creo la matriz

    int **matriz = NULL;
    matriz = (int **)malloc(filas*sizeof(int*));
    //veo si hay errores
    if(matriz == NULL)
        perror("");

    for (int i = 0; i<filas; i++){
        matriz[i] = (int*) malloc(columnas *sizeof(int));
        //veo si hay error
        if(matriz[i]==NULL)
            perror("");
    }

    //Inicializo la matriz con los componentes del archivo si es un blanco pongo 0!!!!!!!!!!!!!!!!!!!!!!!!!!!!.

    int i,j;
    i=0;
    while(i<filas){
        j=0; int num;
        while(j<columnas){
            char a = fgetc(archivo);

            if(a=='1' || a=='2' ||a=='3' ||a=='4' ||a=='5' ||a=='6' ||a=='7' ||a=='8' ||a=='9'){
                num = (int)a;
                num = num - 48;
                matriz[i][j]= num;
                j++;
            }
                else if(a==' '){
                        num = (int)a;
                        num = num - 48;
                        matriz[i][j]= num;
                        j++;
                    }

        }
        i++;
    }

    int pid;
    for(i = 0; i<26;i++){
        pid = fork();

        //error
        if(pid<0){
            printf("Error: Ha ocurrido un error intentando crear un hijo.\n");
            exit(EXIT_FAILURE);
        }

        if(pid == 0){
            int result;//result es el resultado de las funciones verificadoras.

            if(i >= 0 && i < 9){
                //verificar cuadrantes
                result = verificarColumnas(matriz,i);

            }
            else if(i>= 9 && i < 18){
                    //verificar filas
                    result = verificarFilas(matriz,i);

            }
                else if(i>=18 && i < 27){
                        //verificar columnas
                        result = verificarCuadrantes(matriz,i);
                }

            return result;
        }


    }
    int resultado[27];
        //aca me tengo que fijar que dice cada hijo
        for(i = 0 ; i<27; i++){
            wait(&resultado[i]);
        }
        i=0;
        int termine = 0;
        while(i<27 && termine == 0){
            if(WEXITSTATUS(resultado[i])){
                termine = 1;
            }
            i++;
        }

        if(termine == 1)
            printf("La resolución ingresada para el sudoku no es válida\n");
        else
            printf("La solucion es correcta\n");

    //libero la memoria utilizada por la matriz.
    for (int i = 0; i<filas; i++)
        free(matriz[i]);
    
    free(matriz);
    //cierro el archivo.
    fclose(archivo);

    return 0;
}

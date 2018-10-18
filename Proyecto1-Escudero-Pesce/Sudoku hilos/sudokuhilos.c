/*
    Comision: Escudero Johanna, Pesce Danilo.
*/

#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>


/*
    Proyecto Nro 1, Sistemas Operativos
    Comisión: Escudero Johanna, LU: 105868
              Pesce Danilo, LU: 104936

    Ejercicio 1.1.1.c.b Ejercicio del Sudoku resuelto por Hilos
*/



// Los siguientes métodos realizan el trabajo de los hilos -----------------------------------------

#define size 9

typedef struct argumentos{
    int **m;
    int res;
    int hilo;
} *arg_t;

void *verificarCuadrante(void *a){
    arg_t estr = (arg_t) a;

    int arreglo[size];
    int i,j,k,pos;
    int c= (estr->hilo)%9;

    for(k = 0; k<size; k++)
        arreglo[k] = 0;

    for(i = 0; i < 3; i++){
        for(j = 0 ; j < 3; j++){
            pos = estr->m[((c/3)*3)+i][((c%3)*3)+j]; //kekilombo
            if(arreglo[pos-1]!=0){
                estr->res=1;
                pthread_exit(NULL);
            }
            else
                arreglo[pos-1]= pos;
        }
    }

        estr->res=0;
        pthread_exit(NULL);


}

void *verificarFila(void *a){
arg_t estr = (arg_t) a;

    int pos,i,k;
    int c = (estr->hilo)%9;
    int arreglo[size];

    for(k = 0 ; k<size ; k++)
        arreglo[k] = 0;

    for(i = 0; i<size; i++){

        pos = estr->m[i][c];

        if(arreglo[pos-1]!= 0){
            estr->res = 1;
            pthread_exit(NULL);
        }
        else
            arreglo[pos-1] = pos;
    }
    estr->res=0;
    pthread_exit(NULL);


}

void *verificarColumna(void *a){
    arg_t estr = (arg_t) a;

    int j,pos,k;
    int f = (estr->hilo)%9;
    int arreglo[size];

    for(k = 0; k<size; k++)
        arreglo[k] = 0;

    for(j = 0; j<size; j++){
        pos = estr->m[f][j];
        if(arreglo[pos-1]!=0){
            estr->res=1;
            pthread_exit(NULL);
        }
        else
            arreglo[pos-1]= pos;
    }
    estr->res=0;
    pthread_exit(NULL);


}

// -------------------------------------------------------------------------------------------------


int main(){

    FILE * archivo = fopen("sudoku.txt","r");

    if(archivo == NULL){

        fputs ("File error\n",stderr);
        exit (1);
    }

    int filas = 9;
    int columnas =9;

    //Creo la matriz
    int **matriz = NULL;


   // int **matriz = NULL;
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
            char a = fgetc(archivo); //Leo del archivo.

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

    //Declaración de hilos
    pthread_t hilo[27];

    //Creación de los hilos
    int error;

    arg_t estructuras[27];

    for(i = 0; i< 9 ; i++){
        estructuras[i] = malloc(sizeof(struct argumentos));
        estructuras[i]->m = matriz;
        estructuras[i]->hilo = i;
        error=pthread_create(&hilo[i],NULL,verificarFila,estructuras[i]);
    }
    if(error!=0)
        printf("No se pudo crear hiloFila\n");

    for(i = 9; i<18; i++){
        estructuras[i] = malloc(sizeof(struct argumentos));
        estructuras[i]->m = matriz;
        estructuras[i]->hilo = i;
        error=pthread_create(&hilo[i],NULL,verificarColumna,estructuras[i]);
    }
    if(error!=0)
        printf("No se pudo crear hiloColumna\n");

    for(i=18; i<27; i++){
        estructuras[i] = malloc(sizeof(struct argumentos));
        estructuras[i]->m = matriz;
        estructuras[i]->hilo = i;
        error=pthread_create(&hilo[i],NULL,verificarCuadrante,estructuras[i]);
    }
    if(error!=0)
        printf("No se pudo crear hiloCuadrante\n");


    //Espera que todos los hilos finalicen.

    int valido=0;
    for(i=0; i<9; i++){
        pthread_join(hilo[i],NULL);
        if(valido == 0 && estructuras[i]->res==1)
            valido = 1;
        free(estructuras[i]);
    }

    // Chequea los resultados
    if(valido==1)

        printf("La solución ingresada para el sudoku no es válida\n");

    else

        printf("La solución es correcta\n");


    //libero la memoria utilizada por la matriz.
    for (int i = 0; i<filas; i++)
        free(matriz[i]);
    
    free(matriz);

    //cierro el archivo.
    fclose(archivo);

    return 0;

}

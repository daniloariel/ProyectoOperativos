#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio_ext.h>



/*
    Proyecto Nro 1, Sistemas Operativos
    Comisión: Escudero Johanna, LU: 105868
              Pesce Danilo, LU: 104936

    Ejercicio 1.1.2 Mini Shell.
*/


//Cada funcion es una archivo que se lo paso al excecv para que cargue la imagen

/*Llamadas al sistema implementadas:
    1- mkdir
    2- rmdir
    3- touch
    4- ls
    5- cat
    6- help
    7- pwd
    8- exit
*/

//Constante para tamaño máximo de los arreglos
#define max 100

//variables globales de la shell

char entrada[max];
char *comando;
char *argumentos[max];

//obtiene los argumentos de las funciones.
void getArgumentos(){

    int i;

    comando=NULL;

	//Se elimina cualquier contenido previo de argumentos[i]
    for(i=0; i<(max);i++)
        argumentos[i]= NULL;

    comando = strtok(entrada," "); //obtengo la entrada

    char *aux = strtok(NULL," ");

    i=0;
    while(i<max-2 && aux!=NULL){
        argumentos[i]=aux;
        i++;					//obtengo los argumentos
        aux = strtok(NULL," ");
    }

}



int main (int argc, char *argv[]){

    int noTermine =1;

    printf("Bienvenido a la Mini Shell de Sistemas Operativos\n");
    printf("Escriba 'help' para obtener ayuda o un comando\n");

    while (noTermine){

        __fpurge(stdin);
        printf("\nIngrese un comando: ");
        scanf("%[^\n]s",entrada);
        strcat(entrada,"\0");

        getArgumentos();

        if(strcmp(comando,"exit")==0)
        	return 0;

        //Creo un proceso hijo que se encarga de la tarea solicitada por el usuario
        int pid;
        pid = fork();
        if(pid == 0){
   			int error;
   			//Cargo el archivo como programa a ejecutar en el hijo.
        	error = execv(comando,argumentos);
        	if (error < 0){ //Si ocurre un error
        		printf("Ocurrio un error en el execv(2), puede que los comandos ingresados no existan\n");
        		return 0;
        	}
        }else
        	if(pid<0)
        		printf("Ocurrio un error en el fork(0)\n");

        wait(NULL);
	}	

       return 0;
}

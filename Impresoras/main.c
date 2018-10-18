#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


/*
    Proyecto Nro 1, Sistemas Operativos
    Comisión: Escudero Johanna, LU: 105868
              Pesce Danilo, LU: 104936

    Ejercicio 1.2.1.a Impresoras Compartidas 
*/

//Declaración de semaforos

sem_t impresora_disponible, impresora1, impresora2;

int requerir_impresora(){

    //hay impresoras disponibles????
    sem_wait(&impresora_disponible);

    int esta = sem_trywait(&impresora1); // Trato de consumir una señal
    if(esta==0)                         //si la impresora1 esta disponible
        return 1;
    else{               //si la impresora 2 esta disponible
        sem_wait(&impresora2);
        return 2;
    }
}

void liberar_impresora(int num){

	//Libero la impresora correspondiente
    if(num == 1)
        sem_post(&impresora1);
    else
        sem_post(&impresora2);

    printf("Se libero la impresora nro %i \n",num);

    sem_post(&impresora_disponible);

}

void *usuario(void *a){

    int nroUsuario = *(int*)a;
    int nroImpresora;

    while(1){

    	printf("El usuario %i requiere una impresora \n", nroUsuario);

        nroImpresora = requerir_impresora();

        printf("El usuario %i está usando la impresora %i \n",nroUsuario,nroImpresora);
        fflush(stdout);
        sleep(1);

        liberar_impresora(nroImpresora);

    }

    pthread_exit(0);


}
int main(){

    int cantUsuarios, i;
    cantUsuarios = 10; 

    pthread_t usuarios[cantUsuarios];

    sem_init(&impresora1,0,1);
    sem_init(&impresora2,0,1);
    sem_init(&impresora_disponible,0,2);

    int error;
    for(i=0; i<cantUsuarios;i++){
        error = pthread_create(&usuarios[i],NULL,usuario,&i);
        if(error != 0){
            printf("Error al crear un hilo de usuario\n");
            return -1;
        }
    }


    for(i=0; i<cantUsuarios;i++)
        pthread_join(usuarios[i],NULL);

    return 0;
    
}

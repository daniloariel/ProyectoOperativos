#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


//Cada usuario tiene un thread.

sem_t impresora_disponible, impresora1, impresora2;

int requerir_impresora(){

    //hay impresoras disponibles????
    sem_wait(&impresora_disponible);

    if(sem_trywait(&impresora1))
        return 1;
    else{
        sem_wait(&impresora2);
        return 2;
    }
}

void liberar_impresora(int num){

    if(num == 1)
        sem_post(&impresora1);
    else
        sem_post(&impresora2);

    sem_post(&impresora_disponible);

}

void *usuario(void *a){

    int nroUsuario = *(int*)a;
    int nroImpresora;

    while(1){

    nroImpresora = requerir_impresora();

    printf("El usuario ");
    printf("%i",nroUsuario);
    printf(" está usando la impresora ");
    printf("%i",nroImpresora);
    sleep(5);

    liberar_impresora(nroImpresora);

    }

    pthread_exit(NULL);

}

int main(){

    printf("HOLA");
    int cantUsuarios, i;
    cantUsuarios = 10; //tiene que ingresarlo el usuario
    pthread_t usuarios[cantUsuarios];

   sem_init(&impresora1,0,1);
   sem_init(&impresora2,0,1);
   sem_init(&impresora_disponible,0,2);

    for(i=0; i<cantUsuarios;i++)
        pthread_create(&usuarios[i],NULL,usuario,&i);


    for(i=0; i<cantUsuarios;i++)
        pthread_join(usuarios[i],NULL);

    return 0;
}

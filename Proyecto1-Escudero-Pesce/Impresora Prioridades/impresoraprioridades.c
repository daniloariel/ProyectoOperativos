#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>


/*
    Proyecto Nro 1, Sistemas Operativos
    Comisión: Escudero Johanna, LU: 105868
              Pesce Danilo, LU: 104936

    Ejercicio 1.2.1.b Impresoras compartidas (prioridades).
*/

//Defino la cantidad de usuarios
#define maxUsuarios 4

//Defino cantidad de prioridades
#define maxPrioridades 3


sem_t prioridad[maxPrioridades];
sem_t permisoPrioridad[maxPrioridades];

sem_t imp1Disponible,imp2Disponible,usar1,usar2,termine1,termine2,desocupa1,desocupa2;


void *usuario(void *a){

    int soy = *(int *)a;
	int priority,estaLa1,estaLa2;
	srand(pthread_self());
	priority=(rand()%maxPrioridades);

	while(1)
	{
        printf("Soy el usuario %i con prioridad %i y solicito una impresora\n",soy,priority);
        fflush(stdout);

        //El usuario solicita una impresora
        sem_post(&prioridad[priority]);

        //El usuario espera permiso para utilizar una impresora
        sem_wait(&permisoPrioridad[priority]);

        //Luego del permiso, verifica si la impresora 1 es la que esta disponible
        estaLa1 = sem_trywait(&imp1Disponible);

        if(estaLa1==0)
        {
            printf("Soy el usuario %i con prioridad %i y voy a usar la impresora 1\n",soy,priority);
            fflush(stdout);

            //El usuario envia señal a la impresora 1 de que va a ser utilizada
            sem_post(&usar1);

            //El usuario espera a que la impresora termine de operar
            sem_wait(&termine1);

            //El usuario desocupa la impresora 1
            sem_post(&desocupa1);
        }
        else
        {
            //Como la impresora 1 no esta disponible, utiliza la impresora 2 que debe de estar disponible
            estaLa2 = sem_trywait(&imp2Disponible); //podria ser un wait

            if(estaLa2==0)
            {
                printf("Soy el usuario %i con prioridad %i y voy a usar la impresora 2\n",soy,priority);
                fflush(stdout);

                //El usuario envia señal a la impresora 2 de que va a ser utilizada
                sem_post(&usar2);

                //El usuario espera a que la impresora termine de operar
                sem_wait(&termine2);

                //El usuario desocupa la impresora 2
                sem_post(&desocupa2);
            }
        }

        sleep(rand() % 15);

	}

    pthread_exit(0);
}

void *rutinaImpresora(void *a){

    int quienSoy = *(int *)a;
	int i;
	int hayUsuario;
	srand(pthread_self());

	while(1)
	{
		i=0;

		while(i<3)
		{

            //Chequeo si hay algun usuario con prioridad i
			hayUsuario = sem_trywait(&prioridad[i]);

			//Hay un usuario con prioridad i esperando
			if(hayUsuario == 0)
			{
                //Autorizo a un usuario de prioridad i a usar la impresora
                sem_post(&permisoPrioridad[i]);

                //Soy la impresora 1 sino soy la impresora 2
                if(quienSoy==1)
                {
                    //Espera la señal de que va a ser usada
                    sem_wait(&usar1);

                    printf("Impresora 1 ocupada con usuario de prioridad: %i\n",i);
                    fflush(stdout);

                    //La impresora es usada
                    sleep((rand() % 5)+4);

                    //La impresora informa que termino de trabajar
                    sem_post(&termine1);

                    //Espera que el usuario la desocupe
                    sem_wait(&desocupa1);

                    //Una vez desocupada debe reiniciar las prioridades para arrancar por los que tienen mayor prioridad
                    i=0;

                    printf("Impresora 1 esta disponible\n");
                    fflush(stdout);

                    //La impresora esta disponible para ser usada
                    sem_post(&imp1Disponible);

                }
                else
                {
                    //Espera la señal de que va a ser usada
                    sem_wait(&usar2);

                    printf("Impresora 2 ocupada con usuario de prioridad: %i\n",i);
                    fflush(stdout);

                    //La impresora es usada
                    sleep((rand() % 5)+4);

                    //La impresora informa que termino de trabajar
                    sem_post(&termine2);

                    //Espera que el usuario la desocupe
                    sem_wait(&desocupa2);

                    //Una vez desocupada debe reiniciar las prioridades para arrancar por los que tienen mayor prioridad
                    i=0;

                    printf("Impresora 2 esta disponible\n");
                    fflush(stdout);

                    //La impresora esta disponible para ser usada
                    sem_post(&imp2Disponible);

                }
			}
			else
			{
                //No hay usuarios de prioridad i esperando, se chequeda prioridad siguiente
                i++;
            }
		}

	}

	pthread_exit(0);

}


int main(){

	int i;


	//Inicializacion de semaforos
	for(i=0;i<maxPrioridades;i++)
	{
		sem_init(&prioridad[i],0,0);
		sem_init(&permisoPrioridad[i],0,0);
    }

	sem_init(&imp1Disponible,0,1);
	sem_init(&imp2Disponible,0,1);
	sem_init(&usar1,0,0);
	sem_init(&usar2,0,0);
	sem_init(&termine1,0,0);
	sem_init(&termine2,0,0);
	sem_init(&desocupa1,0,0);
	sem_init(&desocupa2,0,0);

	//Creo e inicializo los hilos de los usuarios
	pthread_t usuarios[maxUsuarios];
	int numeroDeUsuario[maxUsuarios];

    int error;

	for(i=0;i<maxUsuarios;i++)
	{
        numeroDeUsuario[i]=i;
		error = pthread_create(&usuarios[i],NULL,usuario,&numeroDeUsuario[i]);
        if (error != 0){
            printf("Error al crear un hilo\n");
            return -1;
        }
    }

    //Creo e inicializo los hilos de las impresoras
	pthread_t impresora1,impresora2;

    int j = 1;
    int k = 2;

    sleep(2);
	error = pthread_create(&impresora1,NULL,rutinaImpresora,&j);
    if(error !=0){
        printf("Error al crear el hilo de la impresora 1\n");
        return -1;
    }
	sleep(1);
	error = pthread_create(&impresora2,NULL,rutinaImpresora,&k);
    if(error != 0){
        printf("Error al crear el hilo de la impresora 2\n");
        return -1;
    }
    //Espero a que los hilos finalicen

	for(i=0;i<maxUsuarios;i++)
		pthread_join(usuarios[i],NULL);

	pthread_join(impresora1,NULL);
	pthread_join(impresora2,NULL);

	return 0;
}

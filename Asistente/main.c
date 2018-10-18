#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/*
	Proyecto Nro 1, Sistemas Operativos
	Comisión: Escudero Johanna, LU: 105868
			  Pesce Danilo, LU: 104936

	Ejercicio 1.2.2 Asistente.
*/

//Crea las variables gloables para los semaforos
sem_t llegaEstudiante,sillasDisponibles,estudianteAtendido,despertarAsistente,asistenteDurmiendo,oficinaAsistente;

pthread_mutex_t control = PTHREAD_MUTEX_INITIALIZER;

void *asistente(void * c){

    int s,v;
    srand(pthread_self());

    while(1){

		pthread_mutex_lock(&control);
		//Verfica si hay estudiantes esperando
        v = sem_trywait(&llegaEstudiante);

        if(v<0){

            //Si no hay estudiantes, el asistente duerme y espera que un estudiante lo despierte cuando llegue
            printf("No hay estudiantes, asistente duerme\n");
            fflush(stdout);
            sem_post(&asistenteDurmiendo);
            pthread_mutex_unlock(&control);
            sem_wait(&despertarAsistente);
            sem_wait(&llegaEstudiante);
        }
        else
            pthread_mutex_unlock(&control);
        

        //Libera la silla que el estudiante ocupaba
        sem_post(&sillasDisponibles);

        //Avisa al estudiante para que entre en la oficina del asistente
        sem_post(&oficinaAsistente);

		printf("Consulta con el estudiante\n");
		fflush(stdout);

		//Simula el tiempo que tarda el asistente en explicarle al estudiante
        s=(rand()%5)+1;
        sleep(s);

        //Despacha al estudiante 
        sem_post(&estudianteAtendido);

        //Retrasa la ejecucion para apreciar el funcionamiento
        sleep(3);
    }

    pthread_exit(0);
}

void * estudiante(void * c){

    printf("Llego un estudiante\n");
    fflush(stdout);

    //Espera por una silla disponible
    int puedo,s;
    srand(pthread_self());

    while(1){
    	pthread_mutex_lock(&control); //zona critica
	    puedo = sem_trywait(&sillasDisponibles);

	    if(puedo<0){

	    	s=(rand()%5)+1;
	    	pthread_mutex_unlock(&control);
	    	printf("Me fui y vengo a probar suerte mas tarde\n");
	        sleep(s);
	    
	    }else{	
	    		pthread_mutex_unlock(&control);
				//Si hay silla disponible
				printf("Lugar disponible\n");
				fflush(stdout);

				//Comunica la llegada del estudiante
			    sem_post(&llegaEstudiante);


				pthread_mutex_lock(&control);
				//Verifica si debe despertar al asistente
				int n = sem_trywait(&asistenteDurmiendo);
				if(n>=0){

			        //Si el asistente dormia, ya se lo desperto
			        printf("Despertando Asistente\n");
			        fflush(stdout);
			        sem_post(&despertarAsistente);
			    }

			    pthread_mutex_unlock(&control);

			    //Espera para poder entrar en la oficina del asistente y tener consulta
			    sem_wait(&oficinaAsistente);

			    //Espera que el asistente termine de explicar
			    sem_wait(&estudianteAtendido);

			    printf("Consulta finalizada\n");
			    fflush(stdout);

			    pthread_exit(0);
		}
	}

}


int main (int argc, char * args[]){

    //Inicializa las variables de control
	int r,cont,aux,error;

	//Inicializa los semaforos
    sem_init(&llegaEstudiante,0,0);
    sem_init(&sillasDisponibles,0,3);
    sem_init(&estudianteAtendido,0,0);
    sem_init(&despertarAsistente,0,0);
    sem_init(&asistenteDurmiendo,0,0);
    sem_init(&oficinaAsistente,0,0);

    pthread_mutex_unlock(&control);

	//Crea el hilo del asistente
    pthread_t hiloAsistente;
    error = pthread_create(&hiloAsistente, NULL, asistente, NULL);
    if(error != 0){
        printf("Error al crear el hilo del asistente\n");
        return -1;
    }

    //Retrasa la ejecucion para apreciar el funcionamiento
    sleep(1);

	//Crea una cantidad de hilos de los estudiantes aleatoria
    while(1){

        r = (rand()%15);
        cont = 0;
        pthread_t estudiantes[r];

        while(cont!=r){

            error = pthread_create(&estudiantes[cont],NULL,estudiante,NULL);
            if(error != 0 ){
                printf("Error al crear un hilo de estudiante\n");
                return -1;
            }

			//Genera un lapso de tiempo entre 1 y 3 segundos para crear los hilos de los estudiantes
            aux = (rand() % 2) +1 ;
            sleep(aux);

            cont++;
        }

        cont=0;

		//Espera por la finalizacion de los hilos creados
        while(cont!=r){

            pthread_join(estudiantes[cont],NULL);
            cont++;
        }
    }

	pthread_join(hiloAsistente,NULL);

    return 0;
}
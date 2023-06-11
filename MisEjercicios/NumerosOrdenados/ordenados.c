#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 3

pthread_mutex_t mutex;
pthread_cond_t mi_turno;

int num_actual= 0;
int turno=1;
int esPar=1;
//turno 1 impar, turno 2 par, turno 3 escribe
void *sumaPares()
{
    while(1){

        pthread_mutex_lock(&mutex);

        while(turno!=0){
            pthread_cond_wait(&mi_turno,&mutex);
        }
        ++num_actual;
        turno=3;
        esPar=1;
        pthread_cond_broadcast(&mi_turno);
        if(num_actual>1000) break;
        pthread_mutex_unlock(&mutex);
    }
	
}

void *sumaImpares()
{
     while(1){

        pthread_mutex_lock(&mutex);

        while(turno!=1){
            pthread_cond_wait(&mi_turno,&mutex);
        }
        ++num_actual;
        turno=3;
        esPar=0;
        pthread_cond_broadcast(&mi_turno);
        if(num_actual>1000) break;

        pthread_mutex_unlock(&mutex);
    }
	
}

void *escribeNumero()
{
	while(1){

        pthread_mutex_lock(&mutex);
        while(turno!=3){
            pthread_cond_wait(&mi_turno,&mutex);
        }

        printf("%u\n",num_actual);
	    //sleep((rand() % 3) + 1);

        if(esPar) turno=2;
        else turno=1;
 
        pthread_cond_broadcast(&mi_turno);
        if(num_actual>1000) break;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char *argv[])
{

	pthread_mutex_init(&mutex, NULL);

	pthread_cond_init(&mi_turno, NULL);

	pthread_t threadsArray[THREADS];

	pthread_create(&threadsArray[0],NULL,sumaPares,NULL);
	pthread_create(&threadsArray[1],NULL,sumaImpares,NULL);
	pthread_create(&threadsArray[2],NULL,escribeNumero,NULL);

	
    pthread_join(threadsArray[0], NULL);
    pthread_join(threadsArray[1], NULL);
    pthread_join(threadsArray[2], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&mi_turno);
	
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 10 //caldero
#define SALVAJES 15 

pthread_mutex_t mutex;
pthread_cond_t full,empty; /*condicion*/

int caldero=CAPACITY;
int tickets=0;
int turno=0;
typedef struct
{
	int id;

}SalvajeInfo;

void putServingsInPot()
{
    caldero+=CAPACITY;

    printf("Soy el cocinero, he añadido %u raciones más\n",CAPACITY);
}
void eat(int miId)
{
	printf("Soy el salvaje con id: %u y estoy comiendo\n", miId);

}
void getServingsFromPot()
{
    --caldero;
}
void *salvaje(void *arg)
{
    SalvajeInfo* miInfo=(SalvajeInfo*)arg;
    int miId=miInfo->id;

    while(1){

        pthread_mutex_lock(&mutex);

        while(caldero==0){

            pthread_cond_wait(&full,&mutex);
        }

       getServingsFromPot();
    
        if(caldero==0){

            pthread_cond_signal(&empty);
        }
        pthread_mutex_unlock(&mutex);
        eat(miId);//Para que puedan comer concurrentemente
    }
	
}

void *cocinero()
{
    while(1){
        pthread_mutex_lock(&mutex);

        while(caldero!=0){
            pthread_cond_wait(&empty,&mutex);
        }

        putServingsInPot();

        pthread_cond_broadcast(&full);
        pthread_mutex_unlock(&mutex);
    }
	
}

int main(int argc, char *argv[])
{

	pthread_mutex_init(&mutex, NULL);

	pthread_cond_init(&full, NULL);
	pthread_cond_init(&empty, NULL);

	pthread_t salvajesArray[SALVAJES];
	pthread_t cocinerosArray[1];

	pthread_create(&cocinerosArray[0],NULL,cocinero,NULL);

	for (int i = 0; i < SALVAJES; i++)
	{
        SalvajeInfo * miInfo=malloc(sizeof(SalvajeInfo));
        miInfo->id=i;
		pthread_create(&salvajesArray[i],NULL,salvaje,miInfo);
	}
	
    pthread_join(cocinerosArray[0], NULL);

	for (int i = 0; i < SALVAJES; i++)
	{
		pthread_join(&salvajesArray[i], NULL);
	}


	return 0;
}

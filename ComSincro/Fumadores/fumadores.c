#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 2 //tamano mesa
#define CLIENTS 3 
#define AGENTES 1 
#define TABACO 0
#define PAPEL 1
#define CERILLA 2

pthread_mutex_t mutex;
pthread_cond_t full,empty; /*condicion*/

int num_ingredientes = 0;
int buffer[CAPACITY]={-1,-1};
typedef struct
{
	int id;
	int miIngrediente;

}FumadorInfo;

void *fumador(void *arg)
{
    FumadorInfo* miInfo=(FumadorInfo*)arg;
    int miId=miInfo->id;
    int miIngrediente=miInfo->miIngrediente;

    while(1){

        pthread_mutex_lock(&mutex);
        while (num_ingredientes!=2 || buffer[0]==miIngrediente || buffer[1]==miIngrediente)
        {
            pthread_cond_wait(&full,&mutex);
        }
        buffer[0]=-1;
        buffer[1]=-1;
        num_ingredientes=0;
        fuma(miId,miIngrediente);
        pthread_cond_broadcast(&empty);
        pthread_mutex_unlock(&mutex);
    }
	
}

void *agente()
{
    while(1){
        pthread_mutex_lock(&mutex);

        while (num_ingredientes!=0 || buffer[0]!=-1 || buffer[1]!=-1)
        {
            pthread_cond_wait(&empty,&mutex);
        }
        
        int random=rand()%CLIENTS;
        int random1;

        do{
            random1=rand()%CLIENTS;

        }while(random1==random);
        
        buffer[0]=random;
        buffer[1]=random1;
    
        coloca(random);
        coloca(random1);

        num_ingredientes=2;
        pthread_cond_broadcast(&full);
        pthread_mutex_unlock(&mutex);
    }
	
}
void coloca(int ing)
{

    switch(ing){
        case 0:{
	        printf("Soy el agente he colocado tabaco \n");
            break;
        }
        case 1:{
	        printf("Soy el agente he colocado papel \n");

            break;
        }
        case 2:{
	        printf("Soy el agente he colocado cerillas \n");

            break;
        }
    }

}
void fuma(int miId,int miIngrediente)
{
	printf("Fumador con ingrediente ", miId);

    switch(miIngrediente){
        case 0:{
	        printf("tabaco ");
            break;
        }
        case 1:{
	        printf("papel ");

            break;
        }
        case 2:{
	        printf("cerillas ");

            break;
        }
    }
	printf("esta fumando\n");

	sleep((rand() % 3) + 1);
}

int main(int argc, char *argv[])
{

	pthread_mutex_init(&mutex, NULL);

	pthread_cond_init(&full, NULL);
	pthread_cond_init(&empty, NULL);

	pthread_t fumadoresArray[CLIENTS];
	pthread_t agentesArray[AGENTES];

    int ingredientes[]={TABACO,PAPEL,CERILLA};

	pthread_create(&agentesArray[0],NULL,agente,NULL);

	for (int i = 0; i < CLIENTS; i++)
	{
        FumadorInfo * miInfo=malloc(sizeof(FumadorInfo));
        miInfo->id=i;
        miInfo->miIngrediente=ingredientes[i];
        
		pthread_create(&fumadoresArray[i],NULL,fumador,miInfo);
	}
	
    pthread_join(agentesArray[0], NULL);

	for (int i = 0; i < CLIENTS; i++)
	{
		pthread_join(fumadoresArray[i], NULL);
	}


	return 0;
}

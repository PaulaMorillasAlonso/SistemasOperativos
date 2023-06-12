#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CLIENTES 10 
#define SURTIDORES 2 

pthread_mutex_t mutex; /*mutex para buffer (discoteca) compartido*/
pthread_cond_t libre; /*condicion*/
int ticket=0;
int turno=0;
typedef struct
{
	int id;
	int ocupado;

}SurtidorInfo;

SurtidorInfo surtidoresInfo [SURTIDORES];
void getUnusedPump(){

    int surtidorId;
    pthread_mutex_lock(&mutex);
    int miTicket=ticket;
    ticket++;
    while(miTicket!=turno || (surtidoresInfo[0].ocupado && surtidoresInfo[1].ocupado)){
        pthread_cond_wait(&libre,&mutex);
    }

    turno++;

    if(!surtidoresInfo[0].ocupado){
        surtidoresInfo[0].ocupado=1;
        surtidorId=surtidoresInfo[0].id;
    }
    else if(!surtidoresInfo[1].ocupado){
        surtidoresInfo[1].ocupado=1;
        surtidorId=surtidoresInfo[1].id;
    }

    pthread_cond_broadcast(&libre);
    pthread_mutex_unlock(&mutex);

    PumpFuel(surtidorId,miTicket);
	
}
void releasePump(int pumpId){

    pthread_mutex_lock(&mutex);
    printf("El cliente ha soltado el surtidor %u\n",pumpId);

    surtidoresInfo[pumpId].ocupado=0;

    pthread_cond_broadcast(&libre);
    pthread_mutex_unlock(&mutex);

}
void *cliente()
{
    getUnusedPump();
    /* Uso del surtidor */
    //PumpFuel(pumpId,);
    /* Deja de usar el surtidor */
    releasePump(pumpId);
}

void PumpFuel(int idSurtidor,int ticketCliente)
{

    switch(idSurtidor){
        case 0:{
	        printf("El cliente con ticket %u, está usando el surtidor 0\n",ticketCliente);
            break;
        }
        case 1:{
	        printf("El cliente con ticket %u, está usando el surtidor 1\n",ticketCliente);
            break;
        }
    }
    sleep((rand() % 3) + 1);

}

int main(int argc, char *argv[])
{


	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&libre, NULL);

    for (int i = 0; i < SURTIDORES; i++)
    {
        surtidoresInfo[i].id=i;
        surtidoresInfo[i].ocupado=0;
    }
    

	pthread_t clientesArray[CLIENTES];

	for (int i = 0; i < CLIENTES; i++)
	{
		pthread_create(&clientesArray[i],NULL,cliente,NULL);
	}

	for (int i = 0; i < CLIENTES; i++)
	{
		pthread_join(clientesArray[i], NULL);
	}

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&libre);

	return 0;
}

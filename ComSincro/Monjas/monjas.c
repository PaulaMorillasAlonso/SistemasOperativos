#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define NMONJAS 4
int turno=0;
int meToca=0;
int comiendo=0;
int esperaMonja=0;
pthread_mutex_t mutex;
pthread_cond_t espera, esperaSalir, startPartida;
int entrar_comedor_y_sentarse(){
    int turnosalida;
    pthread_mutex_lock(&mutex);
    comiendo=0;
    turnosalida=turno;
    turno=(turno+1)%NMONJAS;
    esperaMonja++;
     printf("esperaMonja %d\n",esperaMonja);
    while(esperaMonja!=NMONJAS){
        printf("esperooo\n");
        pthread_cond_wait(&espera,&mutex);
    }
    printf("Soy la monja %d y estoy comiendo %d\n",turnosalida,esperaMonja);
    comiendo++;
    pthread_cond_broadcast(&espera);
    pthread_mutex_unlock(&mutex);
    return turnosalida;

}
void salir_comedor(int turno_salida){
    pthread_mutex_lock(&mutex);
    
    while(turno_salida!=meToca||comiendo!=NMONJAS){
         pthread_cond_wait(&espera,&mutex);
    }
    meToca=(meToca+1)%NMONJAS;
    esperaMonja--;
    printf("Soy la monja %d y voy a hacer mis quehaceres\n",turno_salida);
    pthread_cond_broadcast(&espera);
    pthread_mutex_unlock(&mutex);
}
void *monja(void *arg)
{
   
    int turno_salida;
    while (1)
    {
        //<<labor_en_cuestion>>
        turno_salida = entrar_comedor_y_sentarse();
        //<<comer>>
        salir_comedor(turno_salida);
        sleep(rand()%5+1);
    }
}
int main(int argc, char **argv)
{
    pthread_t hilos[NMONJAS];
    pthread_cond_init(&espera, NULL);
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < NMONJAS; i++)
    {
        pthread_create(&hilos[i], NULL,
                       monja, NULL);
    }
    for (int i = 0; i < NMONJAS; i++)
    {  
        pthread_join(hilos[i], NULL);
    }
    
}

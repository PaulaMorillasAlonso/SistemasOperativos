#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NATLETAS 4

#define NEQUIPOS 4

typedef struct{
    int equipo;
    int idlocal;
    int idglobal;

}atletaINfo;

int podium=0;
int turno[NEQUIPOS];
int estados[NEQUIPOS]={0};
pthread_mutex_t global;
pthread_mutex_t mutex[NEQUIPOS];
pthread_cond_t esperaSalir[NEQUIPOS];
void start_race(atletaINfo id_corredor){
    pthread_mutex_lock(&mutex[id_corredor.equipo]);
    while(id_corredor.idlocal!=turno[id_corredor.equipo]||estados[id_corredor.equipo]!=0){
        pthread_cond_wait(&esperaSalir[id_corredor.equipo],&mutex[id_corredor.equipo]);
    }
    estados[id_corredor.equipo]=1;
    turno[id_corredor.equipo]++;
    printf("Atleta equipo %d y dorsal %d comienza a correr\n",id_corredor.equipo,id_corredor.idlocal);
    pthread_mutex_unlock(&mutex[id_corredor.equipo]);
}
void run(){
    sleep(rand()%5+1);
}
void notify_arrival(atletaINfo id_corredor){
    pthread_mutex_lock(&mutex[id_corredor.equipo]);
    if(id_corredor.idlocal==3){
        pthread_mutex_lock(&global);
        podium++;
        printf("EQUIPO %d quedo : %dº \n",id_corredor.equipo,podium);
        pthread_mutex_unlock(&global);
    }
    else{
        printf("cambio Equipo %d jugador %d \n",id_corredor.equipo,id_corredor.idlocal);
        estados[id_corredor.equipo]=0;
        pthread_cond_broadcast(&esperaSalir[id_corredor.equipo]);
    }
   
    pthread_mutex_unlock(&mutex[id_corredor.equipo]);
}
void*atleta(void*arg){
    atletaINfo info=*(atletaINfo*)arg;
    start_race(info);
   run();
   notify_arrival(info);

}
int main(int argc, char **argv)
{
    int equipos=NEQUIPOS;
     int atletas=NATLETAS;
     pthread_t hilos[equipos*atletas];
     //pthread_cond_init(&espera, NULL);
     //pthread_mutex_init(&mutex, NULL);
    atletaINfo infoAtletas[equipos*atletas];
    int k=0;
    for (int i = 0; i < NEQUIPOS; i++)
    {
        for(int j=0;j<NATLETAS;j++){
            infoAtletas[k].equipo=i;
            infoAtletas[k].idglobal=k;
            infoAtletas[k].idlocal=j;
            pthread_create(&hilos[k], NULL,
                        atleta, &infoAtletas[k]);
            k++;
        }
    }
    
    for (int i = 0; i < equipos*atletas; i++)
    {  
        pthread_join(hilos[i], NULL);
    }
}



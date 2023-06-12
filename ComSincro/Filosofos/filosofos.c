#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 5
#define LEFT(i) ((i+N-1) % N)
#define RIGHT(i) ((i+1) % N)
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N];

pthread_mutex_t m;
pthread_cond_t vc_palillo;

// prueba a coger el palillo
void test(int i)
{
    if (state[i] == HUNGRY
    && state[LEFT(i)] != EATING
    && state[RIGHT(i)] != EATING) {
        state[i] = EATING;
        pthread_cond_broadcast(&vc_palillo);
    }
}

void cogerPalillosMonitor(int i){
    pthread_mutex_lock(&m);
    state[i] = HUNGRY;
    test(i);
    pthread_mutex_unlock(&m);
    pthread_cond_wait(&vc_palillo, &m);
}

void dejarPalillosMonitor(int i){

    pthread_mutex_lock(&m); 
    state[i] = THINKING;
    test(LEFT(i));
    test(RIGHT(i));
    pthread_cond_broadcast(&vc_palillo);
    pthread_cond_broadcast(&vc_palillo);
    pthread_mutex_unlock(&m);
}

void comer(){
    printf("Comiendo...\n");
    sleep(3);
}

void pensar(){
    printf("Pensando...\n");
    sleep(2);
}

void filosofo (int i){
    while (1){
        pensar();
        /*Comunicamos al monitor la necesidad de coger palillos*/
        cogerPalillosMonitor(i);
        comer();
        /*Solicitar al monitor que se dejan los palillos*/
        dejarPalillosMonitor(i);
    }
}

int main(void) {

	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&vc_palillo, NULL);

	pthread_t filosofos[10];

	for (int i = 0; i < 5; i++)
	{
		pthread_create(&filosofos[i], NULL, filosofo, i);
	}

	for (int i = 0; i < 5; i++) {
		pthread_join(filosofos[i], NULL);
	}

	return 0;
}
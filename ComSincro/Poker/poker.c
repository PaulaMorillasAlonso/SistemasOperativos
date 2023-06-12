#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_JUG 10

// variables globales -> Recursos compartidos
int turno = 0;
int ticket = 0;

int mesa = 0; // cuánta gente hay en la mesa

pthread_mutex_t m;
pthread_cond_t vc_full;
pthread_cond_t vc_space;

// no se ejecuta hasta que no estén sentados los 4
void play(void) {
	printf("Partida en marcha con %d jugadores.\n", mesa);
	sleep(1);
};

/* garantizar que cada jugador accede a la mesa en estricto orden
de llegada, mientras no haya empezado la siguiente partida */
void seat_at_table() {
	pthread_mutex_lock(&m);

	int myTicket = ticket;
	ticket++;

	// mientras no sea mi turno O la mesa esté llena no pasa
	while (myTicket != turno || mesa == 4) {

        printf("Esperando el jugador con turno %d.\n", myTicket);
		pthread_cond_wait(&vc_space, &m); // espero a que haya un hueco 
	}

	mesa++;
	turno++;
    //printf("La mesa tiene %d personas.\n", mesa);

    // esperar hasta que la mesa se llene para salir y ejecutar el play()
	while (mesa < 4) { // esperar a que se llene la mesa
        pthread_cond_broadcast(&vc_space);
     //   printf("Esperando el jugador %d a que se llene la mesa.\n", myTicket);
	 	pthread_cond_wait(&vc_full, &m);
	}

	// aviso de que se ha llenado la mesa -> el 4º jugador lo hace
	pthread_cond_broadcast(&vc_full);

	pthread_mutex_unlock(&m);

}

void leave_table() {
	pthread_mutex_lock(&m); // evitar carrera
	mesa--;

	if (mesa == 0)
		pthread_cond_broadcast(&vc_space); // avisamos si de que se puede empezar una nueva partida

	pthread_mutex_unlock(&m);
}

void* player(void* arg)
{
	while (1) {
		seat_at_table();
		play();
		leave_table();
	}
}


int main(void) {

	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&vc_full, NULL);
	pthread_cond_init(&vc_space, NULL);

	pthread_t jugadores[NUM_JUG];

	for (int i = 0; i < NUM_JUG; i++)
	{
		pthread_create(&jugadores[i], NULL, player, NULL);
	}

	for (int i = 0; i < NUM_JUG; i++) {
		pthread_join(jugadores[i], NULL);
	}

	return 0;
}
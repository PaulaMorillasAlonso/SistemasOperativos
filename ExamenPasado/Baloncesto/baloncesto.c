#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NJUGADORES 4
#define MAXREBOTE 70
#define TOPEANOTACION 4

struct jugador {
   int dorsal; // dorsal (único) del jugador
   int eficTiro; // entero entre 1 y 100 que determina su efectividad en el tiro 
   int eficRebote; // entero entre 1 y 100 que determina su efectividad en el rebote 
   int puntos; // puntos conseguidos durante el entrenamiento
};
// POSESION -> un jugador tiene el balon.  ENAIRE -> el balón se ha lanzado a canasta
enum estadoBalon_t {POSESION, ENAIRE}; 

// MUTEX Y VARIABLES DE CONDICIÓN
pthread_mutex_t m;
pthread_cond_t vc;

int finished = 0;

enum estadoBalon_t balon; // Modela el estado actual del balón
int dificultadRebote =0; // var. Global para indicar la dificultad del rebote actual
struct jugador jugadores[NJUGADORES] = {
    {0,30,MAXREBOTE,0}, // jugador con dorsal 0, eficTiro 30, eficRebote MAXREBOTE
    {1,80,20,0} ,
    {2,40,MAXREBOTE,0} ,    
    {3,50,50,0}
};

void rebotear(struct jugador *j )
{  
    pthread_mutex_lock(&m); 

    //esperar a conseguir rebote
    while(finished == 0 && (!balon == ENAIRE || dificultadRebote > j->eficRebote)){
        pthread_cond_wait(&vc, &m);
    }

    if (finished == 0) 
    {// si el entrenamiento no ha acabado (puedo salir del wait porque acabe el entrenamiento)
        balon = POSESION;
        //printf("Cojo el balón. Mi dorsal es %d\n", j->dorsal);
    }
    pthread_mutex_unlock(&m); 
}

void correr() {
    usleep(100);
}

void tirar(struct jugador *j ) {
    pthread_mutex_lock(&m);
    int difTiro;
    balon = ENAIRE;

    difTiro = rand() % 100;
    // si difTiro es menor que nuestra eficacia en tiro: canasta
    //printf("Tiro. Mi dorsal es %d. La dificultad es %d y tengo %d\n", j->dorsal, difTiro, j->eficTiro);
    if (difTiro < j->eficTiro){
        j->puntos += 2;
    }
    // si hemos sobrepasado TOPEPUNTOS, termina el entrenamiento  
    if (j->puntos >= TOPEANOTACION){
        finished = 1;
        printf("finished = 1\n");
    }

    dificultadRebote = rand() % MAXREBOTE; 
    //printf("Suelto balón. Mi dorsal es %d\n", j->dorsal);

    //avisar a todos los  jugadores para que luchen por el rebote
    pthread_cond_broadcast(&vc);

    pthread_mutex_unlock(&m);
}


void* jugadorInit(void* arg) {
    struct jugador *j = arg;

    while (!finished) {
        rebotear(j);
        correr();
        if (!finished) // para que no tiren los demás cuando uno acaba
            tirar(j);
        correr();
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_mutex_init(&m, NULL);
	pthread_cond_init(&vc, NULL);

    finished = 0;
    balon = ENAIRE;

    pthread_t hilosJug[NJUGADORES];

	for (int i = 0; i < NJUGADORES; i++)
	{
		pthread_create(&hilosJug[i], NULL, jugadorInit, &jugadores[i]);
	}

	for (int i = 0; i < NJUGADORES; i++) {
		pthread_join(hilosJug[i], NULL);
	}

	for (int i = 0; i < NJUGADORES; i++) {
		printf("Puntos conseguidos por el jugador %d: %d\n", jugadores[i].dorsal, jugadores[i].puntos);
	}

	return 0;
}
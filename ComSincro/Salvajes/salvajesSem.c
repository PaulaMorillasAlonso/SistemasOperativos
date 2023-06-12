// Problema cl�sico de productor-consumidor

int caldero = 0;

// IMPORTANTE inicializarlos
sem_t q_salvajes;
sem_t salvaje, cocinero;


getServiceFromPot() {
	while (1) {
		sem_wait(&q_salvajes);

		if (caldero == 0) { // si no hay comida, llamo al cocinero y espero a que sirva
			sem_post(&cocinero);
			sem_wait(&salvaje);
		}
		caldero--;
		sem_post(&q_salvajes);
	}
}

void* Salvajes(void* arg) {
	getServiceFromPot();
	eat();
}

void putServiceInPot(int M) {
	while (1) {
		sem_wait(&cocinero); // espera a que le pidan comida
		caldero += M;
		sem_post(&salvaje); // avisa de que ha repuesto comida
	}
}

void* Cocinero(void* arg) {
	putServiceInPot(M);
}


main() {
	
	sem_init(&q_salvajes, 0, 1); // 1er param -> pshared // 2� param -> valor inicial
	sem_init(&salvaje, 0, 0);
	sem_init(&cocinero, 0, 0);
}
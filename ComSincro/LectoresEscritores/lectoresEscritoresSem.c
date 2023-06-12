sem_t swriters, sreaders;
sem_init(&swriters, 0, 1);
sem_init(&sreaders, 0, 1);

void Reader(void) {
    while (1) {
    sem_wait(&sreaders);

    if (++num_readers == 1)
        sem_wait(&swriters);
        sem_post(&sreaders);
    printf("%d\n", data);

    sem_wait(&sreaders);

    if (--num_readers == 0)
        sem_post(&swriters);
        sem_post(&sreaders);
    }
}
void Writer(void) {
    while (1) {
        sem_wait(&swriters);
        data = data + 2;
        sem_post(&swriters);
    }
}
pthread_mutex_t m;
pthread_cond_t rq,wq;

void Readers(void) {
while (1) {
    pthread_mutex_lock(&m);

    while (reader_waiting)
        pthread_cond_wait(&rq, &m);

    while (nwriters) {
        reader_waiting = 1;
        pthread_cond_wait(&wq, &m);
        reader_waiting = 0;
    }
    nreaders++;

    pthread_cond_broadcast(&rq);

    pthread_mutex_unlock(&m);

    printf("%d\n", data); //zona concurrente para los escritores

    pthread_mutex_lock(&m);

    if (--nreaders == 0)
        pthread_cond_signal(&wq);
        pthread_mutex_unlock(&m);
        
    }
}
void Writers(void)
{
    while (1) {
        pthread_mutex_lock(&m);

        while (nreaders || nwriters)
            pthread_cond_wait(&wq, &m);

        nwriters++;
        pthread_mutex_unlock(&m);

        data += 2;

        pthread_mutex_lock(&m);
        nwriters--;
        pthread_cond_signal(&wq);
        pthread_mutex_unlock(&m);
    }
}
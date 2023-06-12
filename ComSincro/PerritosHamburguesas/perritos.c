#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_USUARIOS 2 
#define CLIENTES 4     // numero de usuarios
#define COCINEROS 4    // numero de usuarios
#define N 5
typedef enum
{
    PERRITO,
    HAMBORGUESA

} COMIDAS;
struct cliente
{
    int comida;
    int id;
};
int platos[2] = {0};
int clien = 0;
pthread_mutex_t mutex;
pthread_cond_t clientes[2];
pthread_cond_t cocineros[2];
void servirComida(int comida)
{
    pthread_mutex_lock(&mutex);

    while (platos[comida] != 0)
    {
        printf("ESPERANDO CLIENTES COMAN %d\n", comida);
        pthread_cond_wait(&cocineros[comida], &mutex);
    }
    platos[comida] = N;
    if (comida == 0)
        printf("---PERRITOS HECHOS---\n");
    else
        printf("---HAMBURGUESAS HECHAS---\n");

    pthread_cond_broadcast(&clientes[comida]);
    pthread_mutex_unlock(&mutex);
}
void *Cocinero(void *arg)
{
    int comida = *(int *)arg;
    if (comida == 0)
        printf("LLEGA COCINERO: comida PERRITOS\n");
    else
        printf("LLEGA COCINERO: comida HAMBORGUESA\n");
    while (1)
    {
        servirComida(comida);
    }
}
void conseguirPlato(int comida, int id)
{
    pthread_mutex_lock(&mutex);

  platos[comida]--;
    while (platos[comida] == 0)
    { 
        pthread_cond_broadcast(&(cocineros[comida]));
        pthread_cond_wait(&clientes[comida], &mutex);

    }

   
    pthread_mutex_unlock(&mutex);
}
void comer(int comida, int id)
{
    if (comida == 0)
        printf("Cliente %d : comiendo PERRITO quedan %d\n", id, platos[comida]);
    else
        printf("Cliente %d : comida HAMBORGUESA quedan %d\n", id, platos[comida]);
    sleep(rand() % 5 + 1);
}
void *Clients(void *arg)
{
    struct cliente comida = *(struct cliente *)arg;
    if (comida.comida == 0)
        printf("Cliente %d : LLEGA quiero PERRITO\n", comida.id);
    else
        printf("Cliente %d : LLEGA quiero HAMBORGUESA\n", comida.id);
    while (1)
    {
        conseguirPlato(comida.comida, comida.id);
        comer(comida.comida, comida.id);
    }
}

int main(int argc, char *argv[])
{
    int i;

    pthread_t cocineros1[COCINEROS];
    pthread_t usuarios[CLIENTES];
      pthread_cond_init(&cocineros[0], NULL);
      pthread_cond_init(&cocineros[1], NULL);
       pthread_cond_init(&clientes[0], NULL);
      pthread_cond_init(&clientes[1], NULL);
    //         pthread_cond_init(&cocineros[1], NULL);
    pthread_mutex_init(&mutex, NULL);

    int ids[COCINEROS] = {HAMBORGUESA};
    struct cliente idClientes[CLIENTES];
    platos[0] = 5;
    platos[1] = 5;
    for (i = 0; i < 4; i++)
    {
        if (i % 2 == 0)
            ids[i] = PERRITO;
            else  ids[i] = HAMBORGUESA;

        pthread_create(&cocineros1[i], NULL, Cocinero, &ids[i]);
    }
    for (i = 0; i < CLIENTES; i++)
    {
        idClientes[i].id = i;
        idClientes[i].comida = rand() % 2;

        pthread_create(&usuarios[i], NULL, Clients, &idClientes[i]);
    }
    for (i = 0; i < COCINEROS; i++)
    {

        pthread_join(cocineros1[i], NULL);
    }
    for (i = 0; i < CLIENTES; i++)
    {

        pthread_join(usuarios[i], NULL);
    }
    //    // pthread_join(bus,NULL);
    pthread_cond_destroy(&clientes[0]);
    pthread_cond_destroy(&clientes[1]);
    pthread_cond_destroy(&cocineros[0]);
    pthread_cond_destroy(&cocineros[1]);
    // 	// pthread_cond_destroy(&user);

    pthread_mutex_destroy(&mutex);
    //     return 0;
}
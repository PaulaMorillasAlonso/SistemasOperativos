#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <err.h>

#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

typedef struct {
	int id;
	int isvip; // 1 == true , 0 == false
} ClientInfo;

int aforo = 0;
int turno = 0;
int turnoVIP = 0;
int ticket = 0;
int ticketVIP = 0;

int VIPwaiting = 0;

pthread_mutex_t m;
pthread_cond_t vc_enter;

void enter_normal_client(int id)
{
	pthread_mutex_lock(&m);

	int myTicket = ticket;
	ticket++;

	// esperar a que la discoteca no esté llena
	while (aforo == CAPACITY || myTicket != turno || VIPwaiting > 0){
		pthread_cond_wait(&vc_enter, &m);
	}

	aforo++;
	turno++;

	printf("Client %2d (not vip) with ticket %d entering disco\n", id, myTicket);

	pthread_cond_broadcast(&vc_enter);

	pthread_mutex_unlock(&m);
}

void enter_vip_client(int id)
{
	pthread_mutex_lock(&m);

	int myTicket = ticketVIP;
	ticketVIP++;

	// esperar a que la discoteca no esté llena
	while (aforo == CAPACITY || myTicket != turnoVIP){
		VIPwaiting++;
		pthread_cond_wait(&vc_enter, &m);
		VIPwaiting--;	
	}

	aforo++;
	turnoVIP++;

	printf("Client %2d (  vip  ) with ticket %d entering disco\n", id, myTicket);

	pthread_cond_broadcast(&vc_enter);

	pthread_mutex_unlock(&m);
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void exit_client(int id, int isvip)
{
	pthread_mutex_lock(&m);
	printf("Client %2d (%s) exiting disco\n", id, VIPSTR(isvip));
	aforo--;
	pthread_cond_broadcast(&vc_enter);
	pthread_mutex_unlock(&m);
}

void *client(void *arg)
{
	int isvip =  ((ClientInfo*)arg)->isvip;
	int id =  ((ClientInfo*)arg)->id;
	
	if (isvip == 1)
        enter_vip_client(id);
    else
        enter_normal_client(id);

    dance(id, isvip);
    exit_client(id, isvip);

	free(arg);
}

int main(int argc, char *argv[])
{
	if (argc < 2){
		fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
		exit(1);
	}
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&vc_enter, NULL);

	FILE *fp;  
	int buff;

	// "r" = mode -> for reading
	if ((fp = fopen(argv[1], "r")) == NULL){
		err(2,"The input file %s could not be opened\n", argv[1]);
	}

	fscanf(fp, "%d", &buff);
	int N = buff; // nº de personas que entran

	pthread_t* clientes = malloc(N*sizeof(pthread_t));


	for (int i = 0; i < N; ++i){
		ClientInfo* arg = malloc(sizeof(ClientInfo));
		arg->id = i;
		fscanf(fp, "%d", &buff);
		arg->isvip = buff;
		
		pthread_create(&clientes[i], NULL, client, arg);
	}

	for (int i = 0; i < N; i++) {
		pthread_join(clientes[i], NULL);
	}

	free(clientes);
	return 0;
}

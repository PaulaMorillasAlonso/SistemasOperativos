#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define N 4
struct tharg {
int i;
char priority;
};

void *thread_usuario(void *arg)
{
	struct tharg *tharg = (struct tharg *) arg;
	long id=(long)pthread_self();
	printf("Thread %d with pthread id %ld has this priority: %c\n", tharg->i,
	id, tharg->priority);
	pthread_exit(0);

}

int main(int argc, char* argv[])
{
	int i;
	pthread_attr_t attr;
	struct tharg args[N];
	pthread_t thid[N];
	pthread_attr_init(&attr);
	for(i = 0; i < N; i ++) {
		args[i].i = i;
		if(i % 2 == 0){ //es par
			args[i].priority ='P';
		}
		else{
			args[i].priority ='N';
		}
		pthread_create(&thid[i], &attr, thread_usuario, &args[i]);
	}

	for(i = 0; i < N; i ++){
		pthread_join(thid[i], NULL);
	}

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <signal.h>
#define MAX 5
int stop=0;
void handleSignal(int signal){
	if (signal == SIGINT) {
        stop = 1;
		printf("setting stop to 1\n"); 
    }
}
int main(int argc, char *argv[])
{
	FILE *file = NULL;
	signal(SIGINT, handleSignal);
	/*Abre archivo de origen*/
	if ((file = fopen("datos.txt", "w+")) == NULL){
		err(2, "The file datos.txt could not be opened");
	}
	char *buffer = malloc(sizeof(char));

	while(stop!=1)
	{
		int err;
		//err = fgets(buffer,MAX,stdin);
		err = scanf("%ms", &buffer);
		if (stop != 1)
			fprintf(file,"%u\n",atoi(buffer));
		free(buffer);
	}
	printf("El programa va a finalizar\n");
	/*fseek(file,0,SEEK_END);
	int fileSize=ftell(file);
	ftruncate(fileSize-2);
	*/
	// Cierra los archivos
	fclose(file);
	return 0;
}

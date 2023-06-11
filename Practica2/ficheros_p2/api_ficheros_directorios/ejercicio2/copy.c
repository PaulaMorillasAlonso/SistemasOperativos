#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define SIZE 512

/*Permite hacer la copia de un fichero regular usando las llamadas al sistema
 del estándar POSIX: open, read, write y close*/
/*Realiza la copia en bloques de 512B.Siempre se deben escribir en el fichero 
destino tantos bytes como se hayan leído del fichero origen.*/

void copy(int fdo, int fdd)
{
	char * buffer[SIZE]; //Buffer intermedio para cominucacion entre archivos
	int leido; //Numero de bytes que se han leido en la llamada a read
	do{
		leido=read(fdo,buffer,SIZE); //Lee SIZE bytes del archivo y los escribe en el buffer
		write(fdd,buffer,leido); //Escribe el numero de bytes leidos desde el buffer al otro archivo
	}while(leido!=0);
}

int main(int argc, char *argv[])
{

	if (argc!=3) {
		fprintf(stderr,"Usage: %s <origin_filename> <destiny_filename>\n",argv[0]);
		exit(1);
	}

	int originFd=open(argv[1],O_RDONLY);
	if(originFd==-1){
		fprintf(stderr,"Origin file couldn't be opened\n");
		exit(1);
	}
	int destFd=open(argv[2],O_WRONLY|O_CREAT);
	if(destFd==-1){
		fprintf(stderr,"Desiny file couldn't be opened nor created\n");
		exit(1);
	}

	copy(originFd,destFd);
	
	close(originFd);
	close(destFd);

	return 0;
}

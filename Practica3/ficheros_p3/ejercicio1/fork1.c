#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
/*programa que crea dos hijos: uno no cambia de ejecutable y otro si */
void printInfo(int isParent,int pid, int ppid){
	if(isParent==0) printf("Hijo ID: %i, su padre tiene ID:%i \n",pid,ppid);
	else printf("Padre ID: %i, su padre tiene ID:%i \n",pid,ppid);
}
int forkAndWrite(){

	int isParent=fork();
	int pid=getpid();
	int ppid=getppid();
	int out=0;
	if(isParent > 0) waitpid(isParent,&out,0);
	printInfo(isParent,pid,ppid);
	return isParent;
}

int main(int argc, char **argv)
{
	//Se puede probar con ./fork1 echo "Mensaje"
	forkAndWrite();
	int isParent=forkAndWrite();
	if(isParent==0) { //si es hijo, ejecuta teniendo en cuenta el argumento de la llamada a la ejecucion (echo mens)
		execvp(argv[1], argv);
	}

	return 0;
}

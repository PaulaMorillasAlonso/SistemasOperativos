#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* programa que crea N hijos siguiendo un grado de dependencias. Usa las
 * versiones de execl y execlp */

/*
void printInfo(int pid, int maxParentPid)
{
	printf("Soy P%i\n", pid - maxParentPid);
}

int forkAndWrite(int maxParentPid)
{
	int isParent = fork();
	int pid = getpid();
	int ppid = getppid();
	int out = 0;
	if (isParent == 0){
		printInfo(pid, maxParentPid);
	}
	return isParent;
}

int forkAndWriteAfterFinished(int maxParentPid, int waitForPid[], int nWaitFor){

	int out;
	for (int i = 0; i < nWaitFor; i++)
	{
		waitpid(waitForPid[i] + maxParentPid, &out, 0);
	}
	
	int pid = forkAndWrite(maxParentPid);

	return pid;
}

int forkAfterFinished(int maxParentPid, int waitForPid[], int nWaitFor){

	int out;
	for (int i = 0; i < nWaitFor; i++)
	{
		waitpid(waitForPid[i] + maxParentPid, &out, 0);
	}
	
	int pid = fork();

	return pid;
}
*/

int main(int argc, char **argv)
{
	pid_t pid1 = fork();

	if (pid1 == 0){
		execlp("echo", "echo", "Soy P1", NULL);
	}

	// ---

	while(waitpid(pid1, NULL, 0) == 0);

	pid_t pid2 = fork();

	if (pid2 == 0){
		execlp("echo", "echo", "Soy P2", NULL);
	}

	pid_t pid5 = fork();

	if (pid5 == 0){
		execlp("echo", "echo", "Soy P5", NULL);
	}

	pid_t pid7 = fork();

	if (pid7 == 0){
		execlp("echo", "echo", "Soy P7", NULL);
	}

	// ---

	while(waitpid(pid2, NULL, 0) == 0);

	pid_t pid3 = fork();

	if (pid3 == 0){
		execlp("echo", "echo", "Soy P3", NULL);
	}

	pid_t pid4 = fork();

	if (pid4 == 0){
		execlp("echo", "echo", "Soy P4", NULL);
	}

	// ---

	while(waitpid(pid4, NULL, 0) == 0 || waitpid(pid5, NULL, 0) == 0);

	pid_t pid6 = fork();

	if (pid6 == 0){
		execlp("echo", "echo", "Soy P6", NULL);
	}

	// ---

	while(waitpid(pid3, NULL, 0) == 0 || waitpid(pid6, NULL, 0) == 0 || waitpid(pid7, NULL, 0) == 0);

	pid_t pid8 = fork();

	if (pid8 == 0){
		execlp("echo", "echo", "Soy P8", NULL);
	}

	// ---

	return 0;
}

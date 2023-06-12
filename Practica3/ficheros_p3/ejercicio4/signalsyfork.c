#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

/*programa que temporiza la ejecución de un proceso hijo */
int pid;
void killHandler()
{
	kill(pid, SIGKILL); // Envia señal sigkill al hijo
}
int main(int argc, char **argv)
{
	pid = fork();
	if (pid == 0)
	{ // es hijo
		execvp(argv[1], argv+1);
		exit(EXIT_FAILURE);
	}
	else // es padre
	{
		/*establecerá que el manejador de la señal SIGALRM sea
		una función que envíe una señal SIGKILL al proceso hijo*/

		struct sigaction a1;
		a1.sa_handler = killHandler;
		alarm(5); // espera 5 seg
		sigaction(SIGALRM, &a1, NULL);

	}

	int n;
	int exitState;
	do
	{
		n = wait(&exitState); // espera a que termine
	} while (n == -1 && errno == EINTR);

	if (n == -1)
	{
		// error
		perror("wrong errno exit");
		exit(EXIT_FAILURE);
	}

	if (WIFEXITED(exitState))
	{ // termina de forma normal
		printf("The child process ended normally\n");
	}
	else if (WIFSIGNALED(exitState))
	{ // termina por una señal
		printf("The child process ended due to a signal\n");
	}


	return 0;
}

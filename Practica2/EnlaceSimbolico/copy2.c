#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
struct stat buffer;
/*Copia de un archivo regular o enlace simbólico*/
void copy(int fdo, int fdd)
{
	int size = 512;
	char buffer[size];
	int numRead = read(fdo, buffer, size);
	while (numRead > 0)
	{
		write(fdd, buffer, numRead);
		numRead = read(fdo, buffer, size);
	}
}

int main(int argc, char *argv[])
{

	if (lstat(argv[1], &buffer) == -1)
	{
		perror("Se ha producido un error al leer información sobre el archivo\n");
	}

	if ((buffer.st_mode & S_IFMT) == S_IFREG) // si los bits de tipo, aplicada la mascara, son iguales a los de un fichero regular
	{
		int fdo = open(argv[1], O_RDONLY);
		int fdd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
		copy(fdo, fdd);
		close(fdo);
		close(fdd);
	}
	else if((buffer.st_mode & S_IFMT)==S_IFLNK) //Si por el contrario, es un enlace simbolico
	{
		int sizeInBytes= buffer.st_size+1;
		char * newLinkBuffer[sizeInBytes];
		readlink(argv[1],newLinkBuffer,sizeInBytes);
		newLinkBuffer[sizeInBytes-1]='\0';
		symlink(newLinkBuffer,"linkCopy");
	}
	else perror("El archivo especificado no es de tipo regular o enlacce simbolico\n");

	return 0;
}

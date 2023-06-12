#include <stdio.h>
#include <stdlib.h>
#include <err.h>
/*Leer de un archivo y escribir en otro*/
int main(int argc, char *argv[])
{

	FILE *file = NULL;
	FILE *res = NULL;
	// Ejercicio original: int c,ret;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
		exit(1);
	}

	/*Abre archivo de origen*/
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2, "The input file %s could not be opened", argv[1]);

	// Abre archivo destino y si no existe lo crea
	if ((res = fopen(argv[2], "r+")) == NULL)
	{
		// err(2,"The output file %s could not be opened",argv[2]);
		res = fopen("resultado.txt", "w+");
	}
	if (res == NULL)
	{
		err(2, "The output file %s could not be opened nor created", argv[2]);
	}
	// Ejercicio original
	/*
		Lee un carácter disponible del fichero y devuelve el char o EOF
		while ((c = getc(file)) != EOF) {

		Escribe un caracter en la salida estandar
		ret=putc((unsigned char) c, stdout);

		if (ret==EOF){
			fclose(file);
			err(3,"putc() failed!!");
		}
	}*/

	// Obtenemos el tamaño del fichero origen
	fseek(file, 0, SEEK_END);
	int fileSize = ftell(file);

	// Recolocamos su cursor al inicio
	rewind(file);

	/* fread lee el archivo origen y guarda su contenidoen buffer*/
	char *buffer[fileSize];
	if (fread(buffer, sizeof(char), fileSize, file) != 0)
	{

		// Escribir por terminal (limpia la terminal)
		fflush(stdout);
		fwrite(buffer, sizeof(char), fileSize, stdout); // stdout para escribir por terminal
		// Escribir en el arcivo resultado.txt. Coge el contenido del buffer y lo copia al archivo res
		fwrite(buffer, sizeof(char), fileSize, res);
	}
	// Cierra los archivos
	fclose(file);
	fclose(res);
	return 0;
}

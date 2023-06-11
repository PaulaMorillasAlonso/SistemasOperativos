#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>

/*
El programa debe abrir el fichero indicado en la línea de comandos (consultar optind 
en la página de manual de getopt) y después situar el marcador de posición en la posición 
correcta antes de leer. Para ello haremos uso de la llamada al sistema lseek (consultar la 
página de manual). Si el usuario ha usado el flag -e debemos situar el marcador N bytes antes 
del final del fichero. Si el usuario ha usado el flag -e debemos avanzar el marcador N bytes desde el comienzo del fichero.

Una vez situado el marcador de posición, debemos leer leer byte a byte hasta el final de fichero,
 escribiendo cada byte leído por la salida estándar (descriptor 1).*/
void writeLastBytes(char*filename,int nBytes){

	//Abrimos el archivo
	FILE * file=NULL;
	file=fopen(filename,"r");
	//Movemos el cursor al final para obtener el numero total de bytes
	fseek(file,0L,SEEK_END);
	int fileSize=ftell(file);
	//Lo devolvemos al principio
	fseek(file,0L,SEEK_SET);
	//Lo movemos a la poscion donde comienzan los nBytes que se van a escribir
	fseek(file,fileSize-nBytes,SEEK_SET);
	//Leemos y escribimos byte a byte (enunciado)
	int c,ret;
	while ((c = getc(file)) != EOF) {

		//Escribe un caracter en la salida estandar
		ret=putc((unsigned char) c, stdout);

		if (ret==EOF){
			close(file);
			err(3,"putc() failed!!");
		}
	}

}
void writeFromNBytes(char*filename,int nBytes){
	//Abrimos el archivo
	FILE * file=NULL;
	file=fopen(filename,"r");
	//Movemos el cursor al byte N y escribimos desde ahi
	fseek(file,nBytes,SEEK_SET);
	int c,ret;
	while ((c = getc(file)) != EOF) {

		//Escribe un caracter en la salida estandar
		ret=putc((unsigned char) c, stdout);

		if (ret==EOF){
			close(file);
			err(3,"putc() failed!!");
		}
	}
	//printf("vas a escribir desde el byte %u del archivo %s\n",nBytes,filename);
}
void writeAsUsual(char*filename){

//Abrimos el archivo
	FILE * file=NULL;
	file=fopen(filename,"r");
	//Leemos y escribimos byte a byte (enunciado)
	int c,ret;
	while ((c = getc(file)) != EOF) {

		//Escribe un caracter en la salida estandar
		ret=putc((unsigned char) c, stdout);

		if (ret==EOF){
			close(file);
			err(3,"putc() failed!!");
		}
	}
	//printf("vas a escribir el archivo %s sin cambios\n",filename);

}
int main(int argc, char *argv[])
{
	int opt=0;
	int bytesToSkip=0;
	int toggleE=0; int toggleN=0;
	char * filename;

	while((opt = getopt(argc, argv, "en")) != -1) {
		switch(opt) {
			case 'n':{
				toggleN=1;

				break;
			}
			case 'e':{
				toggleE=1;
				break;
			}
		}
	}
	if(toggleN){
		
		bytesToSkip=atoi(argv[optind]);
		filename=argv[optind+1];

	}else{

		filename=argv[optind];
	}
	//printf("El nombre de tu archivo es %s\n",filename);
	
	if(bytesToSkip!=0){

		if(toggleE){
			writeLastBytes(filename,bytesToSkip);

		}else{
			writeFromNBytes(filename,bytesToSkip);
		}
	}
	else{
		writeAsUsual(filename);
	}
	return 0;
}

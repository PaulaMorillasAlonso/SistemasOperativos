#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

/* Forward declaration */
//void get_size_dir(char *fname, int numBytes);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
/*int get_size(char *fname, size_t *bl)
{
	int blocks=0;
	struct stat sb;

	// Para identificar si un fichero es un directorio deberemos hacer una llamada a lstat
	// y consultar el campo st_mode
	lstat(fname, &sb);

	switch (sb.st_mode & S_IFMT)
	{
	case S_IFDIR:
        blocks+=sb.st_blocks;
        //printf("BLOQUES: %d\n", blocks);
		return get_size_dir(fname, blocks);
		break;

	default:
		return sb.st_blocks;
		break;
	}

}*/


/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
void get_size_dir(char *dname, int numBytes, int flagO,FILE *file)
{
	struct dirent* read;
    DIR* FOLDER;
    FOLDER=opendir(dname);
    char * buffer= malloc(sizeof(char)*numBytes);
    while((read=readdir(FOLDER))!=NULL){

        if (!(strcmp(read->d_name,".")==0 || strcmp(read->d_name,"..")==0) ) {
            struct stat sbs;
            lstat(read->d_name, &sbs);
            switch (sbs.st_mode & __S_IFMT) {
                case __S_IFREG:
                    if(flagO){

                        FILE * currFile;
                        currFile=fopen(read->d_name,"r");
                        int bytesRead=fread(buffer,sizeof(char),numBytes,currFile);
                        fwrite(buffer,sizeof(char),bytesRead,file);
                        char * salto="\n";
                        fwrite(salto,sizeof(char),1,file);
                        fclose(currFile);
                    }
                    else{
                        FILE * currFile;
                        currFile=fopen(read->d_name,"r");
                        int bytesRead=fread(buffer,sizeof(char),numBytes,currFile);
                        fprintf(stdout,"%s\n",buffer);
                        fclose(currFile);
                    }
                break;
            }
        }

    }
    closedir(FOLDER);
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
    FILE * dest=NULL;
	int opt;
    int numBytes=0;
    int flagO=0;
	// Para conocer el número de kilobytes reservados por el sistema para almacenar un fichero podemos hacer uso de la llamada a lstat, 
	// que nos permite saber el número de bloques de 512 bytes reservados por el sistema.
	while((opt = getopt(argc, argv, "on:")) != -1){

		switch (opt)
        {
        case 'n':
            numBytes=atoi(optarg);
            break;
        case 'o':
            flagO=1; //escribe en un fichero nuevo
            break;
        
        default:
            break;
        }
	}
    if(flagO){

        dest=fopen(argv[optind], "w+");

    }

    get_size_dir(".", numBytes,flagO,dest);
	//printf("%dK %s\n", s, argv[i]);
	
	
	// El programa debe mostrar por la salida estándar una línea por fichero de la línea de comandos, 
	// con el tamaño total en kilobytes del fichero y el nombre de dicho fichero. 
	// Para comprobar si nuestro programa funciona correctamente podemos comparar su salida con la del 
	// comando du -ks, pasando a este comando la misma lista de ficheros que al nuestro. Notar que se pueden usar los comodines del shell.

	return 0;
}

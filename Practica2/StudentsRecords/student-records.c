#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/sysmacros.h>

#define MAX_CHARS_NIF  8
//Ejecuta con ./student-records -f database -c 27:67659034X:Chris:Rock 34:78675903J:Antonio:Banderas
typedef struct {
    int student_id; 
    char NIF[MAX_CHARS_NIF+1];  
    char* first_name;
    char* last_name;
} student_t;

typedef struct{
	FILE* file; // FILE* file ??
	int create;
	int addExisting;
	int typeSearch; // 1 -> search by ID 2-> search by NIF
	int searchArg;
	int output_mode;

} options_t;

typedef enum {
    VERBOSE_MODE,
    SEARCH_MODE,
    INSERT_MODE
} output_mode_t;

// El 1er parámetro es el listado de registros en formato ASCII pasados como argumentro al programa en la línea de comando (records), 
// así como el número de registros (nr_records), y devuelve la representación binaria en memoria de los mismos. 
// Esta representación será un array de estructuras cuya memoria ha de reservarse con malloc() dentro de la propia función.
student_t* parse_records(char* records[], int nr_records){

	student_t* students;
	student_t* curr_student;

	students = malloc(sizeof(student_t)*nr_records);
	char* token;

	// zero fill the array of structures -> ponerlo todo a 0
    memset(students,0,sizeof(student_t)*nr_records);

	for (int i = 0; i < nr_records; i++){
		curr_student = &students[i];
		
		sscanf(strsep(&records[i], ":"), "%d", &curr_student->student_id);
		strcpy(curr_student->NIF, strsep(&records[i], ":"));
		curr_student->first_name = strsep(&records[i], ":");
		curr_student->last_name = strsep(&records[i], ":");

		// CHECK ANSWER
		printf("%d ", curr_student->student_id);
	 	printf("%s ", curr_student->NIF);
	 	printf("%s ", curr_student->first_name);
	 	printf("%s\n", curr_student->last_name);
	}
/* 
	// OTRA FORMA:
    for (int i = 0; i < nr_records; i++){
        cur_estudiantes=&estudiantes[i];
        int token_id=0;
        while((token = strsep(&records[i], ":"))!=NULL) {
			switch(token_id) {
                case 0:
                if (sscanf(token,"%d",&cur_estudiantes->student_id)!=1) {
					fprintf(stderr, "Couldn't parse token is %s\n", token);
					return NULL;
				}
                break;
                case 1:
                strcpy(cur_estudiantes->NIF,token);
                break;
                case 2:
                cur_estudiantes->first_name=strdup(token);
                break;
                case 3:
                cur_estudiantes->last_name=strdup(token);
                break;		
			    default:
				break;
			}
			token_id++;
		}
    }
*/

	return students;	
}

// Vuelca al fichero binario ya abierto (students) los registros de estudiantes pasados como parámetro (entries). 
// Para maximizar la reutilización de código, esta función NO escribirá en el fichero la cabecera numérica 
// que indica el número el número de registros
int dump_entries(student_t* entries, int nr_entries, FILE* students){
	int cont = 0;
	char* aux;
	for (int i = 0; i < nr_entries; i++){
		// fwrite is for binary data
		fwrite(&entries[i].student_id, sizeof(int), 1, students);
		fwrite(entries[i].NIF, strlen(entries[i].NIF), 1, students);
		fwrite(entries[i].first_name, strlen(entries[i].first_name)+1, 1, students);
		fwrite(entries[i].last_name, strlen(entries[i].last_name)+1, 1, students);
		cont++;

			// Esto es si quisieramos escribirlo en txt -> normal text
		// fprintf(students,"[Entry #%d]\n",i);
		// 	fprintf(students,"\tstudent_id=%d\n\tNIF=%s\n\t"
		// 	        "first_name=%s\n\tlast_name=%s\n",
		// 	        entries[i].student_id, entries[i].NIF,
		// 	        entries[i].first_name, entries[i].last_name);
	}

	return cont;
}

// lee una cadena de caracteres terminada en '\0' del fichero cuyo descriptor se pasa como parámetro,
// reservando la cantidad de memoria adecuada para la cadena leída.
char* loadstr(FILE* students){

	int len = 0;
    char *name;
	int token;
	do {
		token =fgetc(students);
        len++;
	}while(token != '\0');

    name =  malloc(sizeof(char) * len);

    fseek(students, -len, SEEK_CUR);
	fread(name, sizeof(char), len, students);	

    return name;

}

// Lee toda la información de un fichero binario de registros de estudiantes ya abierto, 
// y devuelve tanto la información de la cabecera (parámetro de retorno nr_entries), 
// como el array de registros de estudiantes (valor de retorno de la función). 
// La memoria del array que se retorna debe reservarse con malloc() dentro de la propia función.
student_t* read_student_file(FILE* students, int* nr_entries){

	int er = fread(nr_entries, sizeof(int), 1, students);
	if (er <= 0)
		printf("Error al leer la cabecera del archivo records.\n");

	
	int n = *nr_entries;

	student_t* s;
	s = malloc(sizeof(student_t)*n);
	//memset(s,0,sizeof(student_t));

	printf("n = %d\n", n);

	for (int i  = 0 ; i < n; i++){
		er = fread(&s[i].student_id, sizeof(int), 1, students);
		if (er <= 0)
			printf("Error al leer el id.\n");

		er = fread(s[i].NIF, sizeof(char), MAX_CHARS_NIF+1, students);
		if (er <= 0)
			printf("Error al leer el NIF.\n");
		
		//char bit = fgetc(students); // Hay un char aquí que nos queremos saltar
		s[i].first_name = loadstr(students);
		s[i].last_name = loadstr(students);
	}

	return s;
};


int main(int argc, char *argv[])
{
	int retCode, opt;
	options_t options;
	char* file = NULL;

	options.output_mode = 0; // A qué lo inicializamos???????
	options.searchArg = 0;
	options.create = 0;
	options.addExisting= 0;
	options.typeSearch = 0;

	/* Parse command-line options */
	while((opt = getopt(argc, argv, "f:hlcaqi:n:")) != -1) {
		switch(opt) {
		case 'h':
			fprintf(stderr,"Usage: %s -f file [ -h | -l | -c | -a | -q [ -i|-n ID] ] ] [ list of records ]\n",argv[0]);
			exit(0);
		case 'f': // abrir, el archivo file
			file = optarg;
		break;
		case 'l': // listar fichero de registro de estudiantes
			options.output_mode = VERBOSE_MODE;
		break;
		case 'c': // crear fichero de registro
			options.create = 1;
			options.output_mode = INSERT_MODE;
			//destiny = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC , 00700);
		break;
		case 'a': // añadir nuevos registros
			options.addExisting = 1;
			options.output_mode = INSERT_MODE;
			
		break;
		case 'q': // buscar registros específicos
			options.output_mode = SEARCH_MODE;
		break; 
		case 'i': // " por identificador
			options.typeSearch = 1;
			options.searchArg = atoi(optarg);
		break;
		case 'n': // " por NIF
			options.typeSearch = 2;
			options.searchArg =atoi(optarg) ;

		break;
		default:
			exit(EXIT_FAILURE);
		}
	}

	if (options.create == 1){ // tenemos que crear el archivo
		options.file = fopen(file, "w+");

	}else if (options.addExisting == 1){
		if ((options.file=fopen(file,"r+"))==NULL) {
			fprintf(stderr, "The input file %s could not be opened: ", file);
			perror(NULL);
			exit(EXIT_FAILURE);
		}
	}
	else{ // abrimos el fichero existente
		if ((options.file = fopen(file, "r")) == NULL)
			err(2,"The input file %s could not be opened",file);
	}

	int aux;
    switch(options.output_mode){
        case SEARCH_MODE:{
			if(options.typeSearch == 0){ //No se ha especificado búsqueda
					fprintf(stderr, "You must specify the type of search with -i from identifier or -n  from NIF");
			}
			else{
				int i = 0;
				student_t* students;		
				int* nr_entries = malloc(sizeof(int));		
				students = read_student_file(options.file, nr_entries);

				switch(options.typeSearch){		
					case 1:								
						do{
							i++;
						} while(i < *nr_entries  &&  students[i].student_id != options.searchArg);
					break;
					case 2:
						do{
							i++;
						} while(i < *nr_entries  &&  atoi(students[i].NIF) != options.searchArg);
					break;
				}

				if(i < *nr_entries){  //Encontrado
					printf("[Entry #%d]\n",i);
					printf("\tstudent_id=%d\n\tNIF=%s\n\t"
						"first_name=%s\n\tlast_name=%s\n",
						students[i].student_id, students[i].NIF,
						students[i].first_name, students[i].last_name);
				}
				else{
					printf("No entry was found\n");
				}

				free(students);
				free(nr_entries);
			}

		}

        break;
        case VERBOSE_MODE:{

			int* nr_entries = malloc(sizeof(int));
			int aux;
			student_t *students;
			students = read_student_file(options.file, nr_entries);

			printf("nr_entries = %d\n", *nr_entries);

			for (int i = 0; i < *nr_entries; ++i){

				printf("[Entry #%d]\n",i);
				printf("\tstudent_id=%d\n\tNIF=%s\n\t"
						"first_name=%s\n\tlast_name=%s\n",
						students[i].student_id, students[i].NIF,
						students[i].first_name, students[i].last_name);
			}

			free(nr_entries);
			free(students);
		}
        break;
        case INSERT_MODE:{

			if (options.addExisting == 0){
				int num = (argc-optind);
				fwrite(&(num), sizeof(int), 1, options.file);
				
				student_t* students;
				students = parse_records(&argv[optind], argc-optind);
				aux=dump_entries(students, argc-optind, options.file);

				printf("%d records written succesfully\n",aux);

				free(students);

			}else{

				student_t* students;
				int* nr_entries = malloc(sizeof(int));
				students = read_student_file(options.file, nr_entries);

				int nP = argc-optind;
				student_t* newSt = parse_records(&argv[optind], nP);

				int aux_nr = 0;

				for(int i = 0; i < nP; ++i){ // cada elemento insertado
					int duplicate = 0;
					for (int j = 0; j < *nr_entries; ++j){ // cada elem. existente
						if (students[j].student_id == newSt[i].student_id){
							duplicate = 1;
							printf("Found duplicate student_id %d\n", newSt[i].student_id);
						}
					}
					if (duplicate == 0){
						printf("i= %d\n", i);

						//fseek(options.file, 0, SEEK_END);
						dump_entries(&newSt[i],1,options.file);
						aux_nr++;

					}
				}

				printf("nr_entries = %d\n", *nr_entries);
				printf("aux_nr = %d\n", aux_nr);
				*nr_entries += aux_nr;
				printf("nr_entries = %d\n", *nr_entries);

				int n = *nr_entries;
				fseek(options.file, 0, SEEK_SET);
				fwrite(&(*nr_entries), sizeof(int), 1, options.file);

				free(newSt);
				free(students);
				
			}
		}
        break;
    }
    // if(fclose(options.file)==EOF){
    //     //fprintf(stderr, "The input file %s could not be closed: ", options.inputfile);
    //             perror(NULL);
    //             exit(EXIT_FAILURE);
    // }

	fclose(options.file);
	

	return 0;
}

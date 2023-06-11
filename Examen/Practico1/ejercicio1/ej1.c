#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h> /* for getopt() */
#define MAXPROC 16

struct task {
    int valid;
    // booleano, indica si la entrada es válida (1) o no (0)
    int next[MAXPROC]; // array de booleanos para sucesores: next[i]=1 si sale una arista hacia el nodo i
    int id;
// identificador/nombre de la tarea, que corresponde a su posición en el array de nodos
};

void print_graph(struct task tasks[], int n)
{
    int i,j;
    for (i = 0; i < n; i++) {
        if (!tasks[i].valid)
        continue;
        printf("%d: ", tasks[i].id);

        for (j = 0; j < n; j++)
            if (tasks[i].next[j])
                printf("%d ", j);

        printf("\n");
    }
}
void process_graph(FILE *archivo)
{
    struct task tasks[MAXPROC];

    char *buffer=malloc(sizeof(char)*(MAXPROC+1));
    char *token;
    char * dup;
    int count=1; //Para contar el numero de lineas que hay, es decir el numero de nodos
    int c;

    //Queremos contar cuantos nodos tiene el grafo
    while ((c = getc(archivo)) != EOF) {
		
		if((unsigned char)c=='\n'){
            count++;
        }
	}
    //IMPORTANTE: RECOLOCAMOS EL PUNTERO AL INICIO
    fseek(archivo,0,SEEK_SET);
    //Inicializamos todos los sucesores a 0
    for(int i=0; i<count;++i){
        for(int j=0; j<MAXPROC; ++j){
            tasks[i].next[j]=0;
        }
    }
 

    for(int i=0; i<count;++i){

        fscanf(archivo, "%[^\n] ", buffer); //Cogemos una linea del archivo
        //Hacemos un duplicado para no mdifica la linea original
        dup=strdup(buffer);
        //Separamos la linea dejando a la izquierda el id y a la derecha informacion sobre predecesores
        token = strsep(&dup, ",");
        //printf("valor token: %s y valor restante: %s\n",token,dup);
        //Guardamos el valor del id convirtiendolo a int
        int izq=atoi(token);
        //Asignamos para ese nodo su id y el valor valid
        tasks[izq].id=izq;
        tasks[izq].valid=1;
        
        //A la derecha nos queda la informacion sobre predecesores
        int der=atoi(dup);
    
        if(der!=0) //tiene algun predecesor
        {
            //Extraemos el predecesor
            char * aux;
            aux=strdup(dup);
            token = strsep(&aux, ",");
            //printf("He quitado el numero de predecesores, era: %s, queda: %s\n",token,aux );
            //Ahora solo quedan los valores de los predecesores
            for(int j=0; j<der;++j){

                token = strsep(&aux, ",");
                int predecesor=atoi(token);
                tasks[predecesor].next[izq]=1;
                //printf("valor predecesor: %u, mi valor: %u \n",predecesor,izq);
            }
        }
    }
    print_graph(tasks,count);
    free(buffer);
    fclose(archivo);
}
int main(int argc, char *argv[])
{   
    FILE * archivo=NULL;
    archivo=fopen(argv[1],"r");

    if(archivo==NULL){
        printf("Error no se ha podido abrir el archivo\n");
    }
    else{
        process_graph(archivo);
    }

   

}
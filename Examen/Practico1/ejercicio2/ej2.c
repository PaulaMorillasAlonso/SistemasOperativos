#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h> /* for getopt() */
#include <pthread.h>
#define MAXPROC 16
pthread_mutex_t mutex;

pthread_cond_t conds[MAXPROC];

struct task {
    int valid;
    // booleano, indica si la entrada es válida (1) o no (0)
    int next[MAXPROC]; // array de booleanos para sucesores: next[i]=1 si sale una arista hacia el nodo i
    int id;
// identificador/nombre de la tarea, que corresponde a su posición en el array de nodos
};
struct task tasks[MAXPROC];

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
void process_graph(FILE *archivo,struct task tasks[])
{
    //struct task tasks[MAXPROC];

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
    for(int i=0; i<MAXPROC;++i){
        for(int j=0; j<MAXPROC; ++j){
            tasks[i].next[j]=0;
            
        }
    }
    for(int j=0; j<MAXPROC; ++j){
           tasks[j].valid=0;
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
void wait_for_predecesors(int id){
    pthread_mutex_lock(&mutex);

    //Vas a calcular cuantos predecesores tienes
    int predecesors=0;
    //Si son validos y en su lista de sucesores apareces tu, suma uno al numero de predecesores
    for (int i = 0; i < MAXPROC; i++)
    {
        if(tasks[i].valid){
            if(tasks[i].next[id]){
                predecesors++;
            }
        }
    }
    //Mientras tengas predecesores espera a que te avisen de que han acabado
    while (predecesors>0)
    {
        pthread_cond_wait(&conds[id],&mutex);
        //Si han acabado implica que tienes un predecesor menos
        --predecesors;
    }
    
    pthread_mutex_unlock(&mutex);
}
void notify_successors(int id)
{
    pthread_mutex_lock(&mutex);

    //Para todos los nodos
    for (int i = 0; i < MAXPROC; i++)
    {
        if(tasks[i].valid){ //si son validos

            //Y son mis sucesores, aviso de que pueden despertar (ya he acabado mis tareas) 
            if(tasks[id].next[i]){

                pthread_cond_broadcast(&conds[i]);
            }
        }
    }
    


    pthread_mutex_unlock(&mutex);
}
void *task_body(void * arg) {

    int id = *(int *) arg;
    wait_for_predecesors(id); // bloqueante, el hilo espera a que sus predecesores le notifiquen su finalización
    printf("Task %u running\n", id); // cuerpo real de la tarea
    sleep(1);
    notify_successors(id); // sincronización, aviso a los sucesores de que esta tarea ha terminado*/
    return NULL;

}
int main(int argc, char *argv[])
{   
    FILE * archivo=NULL;
    archivo=fopen(argv[1],"r");
    //struct task tasks[MAXPROC];
   
    if(archivo==NULL){
        printf("Error no se ha podido abrir el archivo\n");
    }
    else{
       process_graph(archivo,tasks);
    }

   //Inicializa mutex y v.cond
    pthread_mutex_init(&mutex, NULL);
	
    //Array de hilos
	pthread_t threadsArray[MAXPROC];
    for (size_t i = 0; i < MAXPROC ; i++)
    {
        if(tasks[i].valid)
        {
	        pthread_create(&threadsArray[i],NULL,task_body,&tasks[i].id);
            pthread_cond_init(&conds[i], NULL);
        }
    }
    for (size_t i = 0; i < MAXPROC; i++)
    {
        if(tasks[i].valid)
        {
            pthread_join(threadsArray[i], NULL);
            pthread_cond_destroy(&conds[i]);
        }
    }

    pthread_mutex_destroy(&mutex);
	
}
/* 
Los operarios (worker) repiten indefinidamente las acciones de coger un paquete (get parcel)
y cargarlo en el cami�n (load parcel). 

Varios operarios pueden estar cogiendo un paquete al mismo tiempo, pero cargar�n el cami�n de uno en uno. 

El hilo del operario s�lo debe ejecutarla  funci�n load parcel cuando 
		- haya alg�n cami�n en el muelle 
		- y quede sitio en dicho cami�n para el paquete.

Los camiones (truck) tiene capacidad para cargar NP paquetes, y repiten indefinidamente las
acciones de entrar al muelle (enter dock) para recoger la carga y salir al reparto (delivery).

Varios hilos de cami�n pueden estar ejecutando la funci�n delivery al mismo tiempo. 

Sin embargo, en el muelle s�lo cabe un cami�n y por tanto el hilo que simula al cami�n debe esperar
a que el muelle quede libre para ejecutar la funci�n enter dock en exclusiva. 

Asimismo, el hilo debe esperar a que el cami�n est� completamente cargado para salir del muelle y empezar el reparto.

Completa las funciones truck y worker para que los hilos se sincronicen correctamente, 
a�adiendo las variables que consideres necesarias.

Se puede usar tanto mutex como sem�foros
*/
// HECHO CON MUTEX
/*#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>*/
#define NP 10 //Capacidad m�xima de cami�n

int hayCamion = 0; // nos interesa saber si el muelle est� ocupado. 1 = hay 0 = no hay
int paquetes = 0; // contador de carga


pthread_mutex m;
// Necesito 3 variables de condicion: (una por cada espera, lo podemos ver en los comentarios del enunciado)
// una para notificar al cami�n cuando se llene, (vc_cargado)
// una para notificar a los conductores de que pueden pasar al muelle (vc_entrada),
// y otra para notificar a los conductores de que se pueden ir del muelle (vc_salida)
pthread_mutex_cond vc_entrada, vc_salida, vc_cargado; 

void* truck(void* arg)
{
	while (1) {
		// 1- Proteger los recursos compartidos
		lock(&m);

		// 2- Esperar la condici�n requerida
		// esperar a poder entrar al muelle
		while (hayCamion == 1) { // este bucle vale as� porque no nos importa el orden de llegada en este problema -> en ese caso ponemos variables ticket y turno
			wait(&vc_entrada, &m);
		}
		// 3- Hacer la acci�n
		enter_dock();
		// 4- Cambiar variables locales
		hayCamion = 1;
		// 5- Avisar del cambio de la variable global
		broadcast(&vc_workers); // Usamos broadcast pq hay muchos hilos worker activos

		// esperar a cami�n cargado = salir
		while(paquetes < NP) {
			wait(&vc_salida, &m); // IMPORTANTE : en el wait pasar tanto la variable de condici�n como el mutex
		}

		hayCamion = 0;
		paquetes = 0; // puede ir antes o despu�s del signal, la condici�n es q tiene que ser antes de que se desbloquee el mutex (dentro de la secci�n cr�tica)
		// por eso es importante hacer los signals dentro de la secci�n cr�tica
		
		signal(&vc_entrada); // aviso de mi salida --> signal notifica a vc_entrada (que est� esperando en la l�nea 53)

		unlock(&m);

		// El cami�n se va a repartir
		delivery();
	}
}

/* IMPORTANTE : Diferencia entre Signal y Broadcast
* 
* The short story is this: the "signal" version will wake up only one thread. 
* So, if there were multiple threads blocked in the "wait" function, and a thread did the "signal," 
* then only one of the threads would wake up. Which one? The highest priority one. 
* If there are two or more at the same priority, the ordering of wakeup is indeterminate. 
*
* With the "broadcast" version, all blocked threads will wake up.
*/

void* worker(void* arg)
{
	while (1) {
		p = get_parcel();
		lock(&m);
		
		// esperar a poder cargar
		while (hayCamion == 0 || paquetes >= NP) { // esperar a que haya cami�n y haya hueco en el cami�n
			wait(&vc_paquete, &m);
		}
		load_parcel(p);
		paquetes++;

		signal(&vc_salida); // Aviso al cami�n de que he cargado, �l comprobar� si tiene los paquetes necesarios para salir.
		// Se puede comprobar aqu� si paquetes == NP en lugar de en truck():
		/*
		* if (paquetes == NP)
		*	signal(&vc_salida);
		*/

		unlock(&m);
		// notificar si cargado
	}
}
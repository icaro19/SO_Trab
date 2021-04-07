/*
Filosofos glutoes com semaforos
gcc Sem_Filosofos.c -o semaf_filo.c -Wall -lpthread -lrt

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

int num_filosofos, tempo_pensando, tempo_comendo, ESQ, DIR;
int* estado;
int* id_filosofo;
sem_t mutex;
sem_t *s;

//#define ESQ	(*id_filosofo+num_filosofos-1)%num_filosofos
//#define DIR	(*id_filosofo+1)%num_filosofos
#define PENSANDO 0
#define FAMINTO 1
#define COMENDO 2

void pensar(int indice);
void *filosofos(void * fil);
char* lista(int indice);
void pegar_garfos(int filosofo);
void soltar_garfos(int filosofo);
void teste(int indice);

int main (int argc, char *argv[])
{

	num_filosofos = atoi(argv[1]);
	tempo_pensando = atoi(argv[2]);
	tempo_comendo = atoi(argv[3]);

	estado = malloc (num_filosofos * sizeof (int));
	id_filosofo = malloc (num_filosofos * sizeof (int));
	s = malloc (num_filosofos * sizeof (int));

	pthread_t tid[num_filosofos];
	
	sem_init(&mutex, 0, 1);
	for (int k = 0; k < num_filosofos; ++k)
	{
		id_filosofo[k] = k;
		sem_init(&s[k], 0, 0);
//		printf("id_filosofo = %d\n", id_filosofo[k]);
	}

	for (int j = 0; j < num_filosofos; ++j)
	{
		pthread_create(&tid[j], NULL, filosofos, (void*)&id_filosofo[j]);
	}

	for (int j = 0; j < num_filosofos; ++j)
	{
		pthread_join(tid[j], NULL);
	//	printf("\n---------------------------------\n");
	}

	pthread_exit(NULL);


	free(estado);
	return 0;
}

void *filosofos(void * fil)
{

	while (1)
	{
//		printf("\n-----filosofos - thread %d-----\n", pthread_self());
		int i = * (int *) fil;

//		printf("i = %d\n", i);
		pensar(i);
		pegar_garfos(i);
//		lista(i);
		soltar_garfos(i);
	}
}

void pensar(int indice)
{
//	printf("\n-----pensar-----\n");
	printf("\nFilosofo %d (%s) pensando\n", indice, lista(indice));
	sleep(tempo_pensando);
}



void pegar_garfos(int filosofo)
{
//	printf("\n-----pegar_garfos-----\n");
	sem_wait(&mutex);
	estado[filosofo] = FAMINTO;
	printf("\nFilosofo %d (%s) esta faminto\n", filosofo, lista(filosofo));
	teste(filosofo);
	sem_post(&mutex);
	sem_wait(&s[filosofo]);
}

void soltar_garfos(int filosofo)
{
//	printf("\n-----soltar_garfos-----\n");
	sem_wait(&mutex);
	estado[filosofo] = PENSANDO;
	printf("\nFilosofo %d (%s) soltou os garfos\n", filosofo, lista(filosofo));
	ESQ = (filosofo+num_filosofos-1)%num_filosofos;
	DIR = (filosofo+1)%num_filosofos;
	teste(ESQ);
	teste(DIR);
	sem_post(&mutex);
}

void teste(int indice)
{
	ESQ = (indice+num_filosofos-1)%num_filosofos;
	DIR = (indice+1)%num_filosofos;
//	printf("\n-----teste-----\n");
	if (estado[indice] == FAMINTO && estado[ESQ] != COMENDO && estado[DIR] != COMENDO)
	{
		estado[indice] = COMENDO;
		printf("\n-----Filosofo %d (%s) esta comendo-----\n", indice, lista(indice));
		sleep(tempo_comendo);
		sem_post(&s[indice]);
	}

	/*else
	{
		printf("Filosofo %d nao pode lista\n", indice);
	}*/
}

char* lista(int indice)
{
	switch (indice)
	{
		case 1:
			return "Aristoteles";
		break;

		case 2:
			return "Socrates";
		break;

		case 3:
			return "Platao";
		break;

		case 4:
			return "Tales";
		break;

		case 5:
			return "Parmenides";
		break;

		case 6:
			return "Democrito";
		break;

		case 7:
			return "Pitagoras";
		break;

		case 8:
			return "Empedocles";
		break;

		case 9:
			return "Xenofanes";
		break;

		case 10:
			return "Protagoras";
		break;

		case 11:
			return "Gorgias";
		break;

		case 12:
			return "Epiteto";
		break;

		case 13:
			return "Xenofonte";
		break;

		case 14:
			return "Teofrasto";
		break;

		case 15:
			return "Alcmeao";
		break;

		case 16:
			return "Cleantes";
		break;

		case 17:
			return "Apolonio";
		break;

		case 18:
			return "Diogenes";
		break;

		case 19:
			return "Aristoxenes";
		break;

		case 20:
			return "Clearco";
		break;

		default:
			return "Cicero";
	}
	
}
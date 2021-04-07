/*
Produtores e consumidores - Semaforo
gcc Sem_prod.c -o semprod.c -Wall -lpthread -lrt
entrada: tamanho do buffer, maximos de itens produzidos, maximo de itens consumidos

TODO
-Definir hierarquia tamanho_buf - prod/cons
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

int *buffer;
int tamanho_buf, itens_prod, itens_consum;
sem_t mutex;
sem_t vazio;
sem_t cheio;

void *produtor();
void *consumidor();
int remover_item();
void consumir_item(int hitem);
int produzir_item();
int inserir_item(int hitem);

int main(int argc, char const *argv[])
{
	tamanho_buf = atoi(argv[1]);
	itens_prod = atoi(argv[2]);
	itens_consum = atoi(argv[3]);

	pthread_t tid_prod, tid_cons;
	buffer = malloc (tamanho_buf * sizeof(int));

	for (int i = 0; i < tamanho_buf; ++i)
	{
		buffer[i] = 0;
//		printf("%d\n", buffer[i]);
	}

	sem_init(&mutex, 0, 1);//inicializa semaforos
	sem_init(&vazio, 0, tamanho_buf);
	sem_init(&cheio, 0, 0);


	if(pthread_create(&tid_prod, NULL, produtor, NULL))//gera thread do produtor
	{
		printf("erro ao gerar produtor\n");
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&tid_cons, NULL, consumidor, NULL))//gera thread do consumidor
	{
		printf("erro ao gerar consumidor\n");
		exit(EXIT_FAILURE);
	}

	pthread_join(tid_cons, NULL);
	pthread_join(tid_prod, NULL);

	free(buffer);

	return 0;
}

//--------------------------------PRODUTOR----------------------------

void *produtor()
{
	printf("\n*****producao = %d itens*****\n", itens_prod);
	int item, status;

	while(1)
	{
		
		for (int i = 0; i < itens_prod && i < tamanho_buf; ++i)
		{
			item = produzir_item();
			
			sem_wait(&vazio);//entra na regiao critica do produtor
			sem_wait(&mutex);

			status = inserir_item(item);

			if(!status)
			{
				printf("\nAviso: buffer cheio\n");
//				i = itens_prod;
			}

			else
			{
				printf("\nItem %d de %d foi inserido no buffer\n", i+1, itens_prod);
			}

			sem_post(&mutex);
			sem_post(&cheio);//sai da regiao critica
		}

		

		

/*		printf("-----buffer pos-producao:-----\n");

		for (int i = 0; i < tamanho_buf; ++i)
		{
			printf("%d ", buffer[i]);
		}

		printf("\n");*/
	}
}

int produzir_item()
{
	printf("\nproduzindo 1 item\n");
	sleep(1);

	return 1;
}

int inserir_item(int hitem)
{
	int i;
	for (i = 0; buffer[i] != 0 && i < tamanho_buf; ++i);

	if (i < tamanho_buf)
	{
		printf("\n---inserindo item no buffer---\n");
		buffer[i] = 1;
		sleep(1);
		return 1;

	}

	return 0;
	
}

//-------------------------CONSUMIDOR----------------------

void *consumidor()
{

	printf("\n*****consumo = %d itens*****\n", itens_consum);
	int item = 0;

	
	while(1)
	{

		for (int i = 0; i < itens_consum && i < tamanho_buf; ++i)
		{
			printf("j = %d\n", i);
			sem_wait(&cheio);//entra na regiao critica do consumidor
			sem_wait(&mutex);

			item = remover_item();

			
			if(!item)
			{
				printf("\nAviso: buffer vazio\n");
			}

			else
			{
				consumir_item(item);
			}

			sem_post(&mutex);//sai da regiao critica
			sem_post(&vazio);
		}


		
/*		for (int i = 0; i < tamanho_buf; ++i)
		{
			printf("%d \n", buffer[i]);
		}

		printf("\n");*/
	}


}


int remover_item()
{
	int i;
	for (i = 0; i < tamanho_buf && buffer[i] == 0; ++i);

	if (i < tamanho_buf)
	{
		printf("\n---removendo item do buffer---\n");
		sleep(1);
		buffer[i] = 0;
		return 1;
	}

	return 0;
}

void consumir_item(int hitem)
{
	printf("\n---consumindo item do buffer---\n");
	sleep(1);
}
/*
Produtores e consumidores - Threads
gcc Thread_prod.c -o threadprod.c -Wall -lpthread -lrt

*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

int *buffer;
int tamanho_buf, itens_prod, itens_consum, livre;

void *produtor();
void *consumidor();
int remover_item();
int consumir_item(int hitem);
int produzir_item();
int inserir_item(int hitem);

int main(int argc, char const *argv[])
{
	tamanho_buf = atoi(argv[1]);
	itens_prod = atoi(argv[2]);
	itens_consum = atoi(argv[3]);

	pthread_t tid_prod, tid_cons;
	buffer = malloc (tamanho_buf * sizeof(int));
	livre = tamanho_buf;

	for (int i = 0; i < tamanho_buf; ++i)
	{
		buffer[i] = 0;
//		printf("%d\n", buffer[i]);
	}

	if(pthread_create(&tid_prod, NULL, produtor, NULL))
	{
		printf("erro ao gerar produtor\n");
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&tid_cons, NULL, consumidor, NULL))
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

	while(1)
	{

		for (int i = 0; i < itens_prod && i < tamanho_buf; ++i)
		{
			if(!inserir_item(produzir_item()))
			{
				printf("\nAviso: buffer cheio\n");
				i = itens_prod;
			}

			else
			{
				printf("\nItem %d de %d foi inserido no buffer\n", i+1, itens_prod);
			}
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

	while(1)
	{
		for (int i = 0; i < itens_consum && i < tamanho_buf; ++i)
		{
			if(!consumir_item(remover_item()))
			{
				printf("\nAviso: buffer vazio\n");
				i = itens_consum;
			}

			else
			{
				printf("\nItem %d de %d foi consumido\n", i+1, itens_consum);
			}
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
	printf("\nremovendo 1 item\n");
	sleep(1);

	return 1;
}

int consumir_item(int hitem)
{
	int i;
	for (i = 0; i < tamanho_buf && buffer[i] == 0; ++i);

	if (i < tamanho_buf)
	{
		printf("\n---consumindo item do buffer---\n");
		sleep(1);
		buffer[i] = 0;
		return 1;
	}

	return 0;

}
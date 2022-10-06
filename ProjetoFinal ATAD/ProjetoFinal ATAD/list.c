#include "list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct listImpl {
	ListElem *elements;
	unsigned int size;
	unsigned int capacity;
} ListImpl;

/*
Cria uma nova instância da List.
Recebe:
initialCapacity - capacidade inicial (se aplicável)
Retorno:
referência da instância ou;
NULL no caso de inexistência de memória.
*/
PtList listCreate(unsigned int initialCapacity) {
	PtList newList = (PtList)malloc(sizeof(ListImpl));
	if (newList == NULL) return NULL;

	newList->elements = (ListElem*)calloc(initialCapacity, sizeof(ListElem));
	if (newList->elements == NULL) {
		free(newList);
		return NULL;
	}

	newList->size = 0;
	newList->capacity = initialCapacity;

	return newList;
}

/*
Destroi uma instância, libertando a memória associada.
Argumentos:
ptList - endereço da referência da instância;
Retorno:
LIST_NULL se a referência recebida for NULL, ou;
LIST_OK em caso de sucesso;
*/
int listDestroy(PtList *ptList) {
	PtList list = *ptList;

	if (list == NULL) return LIST_NULL;

	free(list->elements);
	free(list);

	*ptList = NULL;

	return LIST_OK;
}

/*
Adiciona um elemento numa dada instância.
Argumentos:
list - referência da instância;
rank - o rank/índice para a operação (0 >= r <= size);
elem - elemento a emlistar;
Retorno:
LIST_NULL se a referência recebida for NULL, ou;
LIST_FULL caso a lista esteja cheia, ou;
LIST_NO_MEMORY caso nao haja memória para guardar o elemento, ou;
LIST_INVALID_RANK caso o rank seja inválido para a operação, ou;
LIST_OK em caso de sucesso;
*/
int listAdd(PtList list, int rank, ListElem elem) {
	if (list == NULL) return LIST_NULL;
	if (rank < 0 || rank > list->size) return LIST_INVALID_RANK;
	/* melhorar com realocação do array 'elements': */
	if (list->size == list->capacity) {
		ListElem *newElements = (ListElem*)realloc(list->elements,
			(list->capacity + 1) * sizeof(ListElem));

		if (newElements == NULL) return LIST_NO_MEMORY;

		list->elements = newElements;
		list->capacity += 1;
	}

	for (int i = list->size; i > rank; i--) {
		list->elements[i] = list->elements[i - 1];
	}

	list->elements[rank] = elem;
	list->size++;
	return LIST_OK;
}


/*
Remove um elemento de uma dada instância.
Argumentos:
list - referência da instância;
rank - o rank/índice para a operação  (0 >= r <= size - 1);
ptElem - elemento removido (retorno por referência);
Retorno:
LIST_NULL se a referência recebida for NULL, ou;
LIST_EMPTY caso a lista esteja vazia, ou;
LIST_INVALID_RANK caso o rank seja inválido para a operação, ou;
LIST_OK em caso de sucesso;
*/
int listRemove(PtList list, int rank, ListElem *ptElem) {
	if (list == NULL) return LIST_NULL;
	if (list->size == 0) return LIST_EMPTY;
	if (rank < 0 || rank > list->size - 1) return LIST_INVALID_RANK;

	*ptElem = list->elements[rank];

	for (int i = rank; i < list->size - 1 ; i++) {
		list->elements[i] = list->elements[i + 1];
	}

	list->size--;

	return LIST_OK;
}


/*
Substitui o elemento no rank de uma dada instância, retornando o
elemento substituido.
Argumentos:
list - referência da instância;
rank - o rank/índice para a operação  (0 >= r <= size - 1);
elem - elemento a inserir no rank;
ptOldElem - elemento anterior no rank (retorno por referência);
Retorno:
LIST_NULL se a referência recebida for NULL, ou;
LIST_EMPTY caso a lista esteja vazia, ou;
LIST_INVALID_RANK caso o rank seja inválido para a operação, ou;
LIST_OK em caso de sucesso;
*/
int listSet(PtList list, int rank, ListElem elem, ListElem *ptOldElem) {
	if (list == NULL) return LIST_NULL;
	if (list->size == 0) return LIST_EMPTY;
	if (rank < 0 || rank > list->size - 1) return LIST_INVALID_RANK;

	*ptOldElem = list->elements[rank];
	list->elements[rank] = elem;
	
	return LIST_OK;
}

/*
Obtem o elemento no rank de uma dada instância.
Argumentos:
list - referência da instância;
rank - o rank/índice para a operação  (0 >= r <= size - 1);
ptElem - elemento no rank (retorno por referência);
Retorno:
LIST_NULL se a referência recebida for NULL, ou;
LIST_EMPTY caso a lista esteja vazia, ou;
LIST_INVALID_RANK caso o rank seja inválido para a operação, ou;
LIST_OK em caso de sucesso;
*/
int listGet(PtList list, int rank, ListElem *ptElem) {
	if (list == NULL) return LIST_NULL;
	if (list->size == 0) return LIST_EMPTY;
	if (rank < 0 || rank > list->size - 1) return LIST_INVALID_RANK;

	(*ptElem) = list->elements[rank];
	
	return LIST_OK;
}

/*
Quantos elementos estão armazenados numa instância.
Argumentos:
list - referência da instância;
ptSize - número de elementos (retorno por referência);
Retorno:
LIST_NULL se a referência recebida for NULL, ou;
LIST_OK em caso de sucesso;
*/
int listSize(PtList list, int *ptSize) {
	if (list == NULL) return LIST_NULL;

	*ptSize = list->size;

	return LIST_OK;
}

/*
Verifica se a instância está vazia (não contém elementos)
Argumentos:
list - referência da instância;
Retorno:
1 caso esteja vazia ou a referência seja NULL, ou;
0 caso não esteja vazia;
*/
int listIsEmpty(PtList list) {
	if (list == NULL ) return 1;
	if (list->size == 0) return 1;
	return 0;
}

/*
Limpa uma instância (remove todos os elementos)
Argumentos:
list - referência da instância;
Retorno:
LIST_NULL se a referência recebida for NULL, ou;
LIST_OK em caso de sucesso;
*/
int listClear(PtList list) {
	if (list == NULL) return LIST_NULL;

	list->size = 0;

	return LIST_OK;
}

/*
Mostra informação sobre uma instância
Argumentos:
list - referência da instância;
*/
void listPrint(PtList list) {

	if (list == NULL)
		printf("LIST NULL \n");
	else if ( /*listIsEmpty(list)*/ list->size == 0)
		printf("LIST EMPTY \n");
	else {
		for (int rank = 0; rank < list->size; rank++) {
			printf("\nAt Rank %d : ", rank);
			listElemPrint(list->elements[rank]);
		}
		
	}
}

#pragma once

#define LIST_OK				0
#define LIST_NULL			1
#define LIST_NO_MEMORY		2
#define LIST_EMPTY			3
#define LIST_FULL			4
#define LIST_INVALID_RANK	5

/* Contem definição do tipo a guardar na list */
#include "listElem.h"

/* Forward declaration do registo (representação dos dados)
da list.
Tem de estar definida em concreto num ficheiro *.c aquando
da compilação do programa.
*/
struct listImpl;

/* Definicao de referência para uma instância da list */
typedef struct listImpl *PtList;

/*
Cria uma nova instância da List.
Recebe:
initialCapacity - capacidade inicial (se aplicável)
Retorno:
referência da instância ou;
NULL no caso de inexistência de memória.
*/
PtList listCreate(unsigned int initialCapacity);

/*
Destroi uma instância, libertando a memória associada.
Argumentos:
ptList - endereço da referência da instância;
Retorno:
LIST_NULL se a referência recebida for NULL, ou;
LIST_OK em caso de sucesso;
*/
int listDestroy(PtList *ptList);

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
int listAdd(PtList list, int rank, ListElem elem);

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
int listRemove(PtList list, int rank, ListElem *ptElem);

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
int listGet(PtList list, int rank, ListElem *ptElem);

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
int listSet(PtList list, int rank, ListElem elem, ListElem *ptOldElem);

/*
Quantos elementos estão armazenados numa instância.
Argumentos:
list - referência da instância;
ptSize - número de elementos (retorno por referência);
Retorno:
LIST_NULL se a referência recebida for NULL, ou;
LIST_OK em caso de sucesso;
*/
int listSize(PtList list, int *ptSize);

/*
Verifica se a instância está vazia (não contém elementos)
Argumentos:
list - referência da instância;
Retorno:
1 caso esteja vazia ou a referência seja NULL, ou;
0 caso não esteja vazia;
*/
int listIsEmpty(PtList list);

/*
Limpa uma instância (remove todos os elementos)
Argumentos:
list - referência da instância;
Retorno:
LIST_NULL se a referência recebida for NULL, ou;
LIST_OK em caso de sucesso;
*/
int listClear(PtList list);

/*
Mostra informação sobre uma instância
Argumentos:
list - referência da instância;
*/
void listPrint(PtList list);


/*
//SEM COMENTÁRIOS - USO PEDAGÓGICO

#define LIST_OK            0
#define LIST_NULL          1
#define LIST_NO_MEMORY     2
#define LIST_EMPTY         3
#define LIST_FULL          4
#define LIST_INVALID_RANK  5

#include "listElem.h"

struct listImpl;
typedef struct listImpl *PtList;

PtList listCreate(unsigned int initialCapacity);
int listDestroy(PtList *ptList);

int listAdd(PtList list, int rank, ListElem elem);
int listRemove(PtList list, int rank, ListElem *ptElem);
int listGet(PtList list, int rank, ListElem *ptElem);
int listSet(PtList list, int rank, ListElem elem, ListElem *ptOldElem);
int listSize(PtList list, int *ptSize);
int listIsEmpty(PtList list);
int listClear(PtList list);
void listPrint(PtList list);

*/
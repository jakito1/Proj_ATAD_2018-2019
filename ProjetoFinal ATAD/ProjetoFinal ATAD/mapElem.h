#pragma once

#include "item.h"

typedef char String[256];

/*Definição da chave do mapa*/
typedef struct stringCode {
	String code;
} StringCode;
typedef StringCode StringKey;

/* definicao do tipo da chave */
typedef StringKey MapKey;
/* definicao do tipo do valor*/
typedef Item MapValue;

StringKey stringCodeCreate(char *str);
void mapKeyPrint(MapKey key);

void mapValuePrint(MapValue value);

/* funcao de comparacao de chaves */
int mapKeyEquals(MapKey key1, MapKey key2);

#include "mapElem.h"
#include "item.h"

/*Função para criar a key do mapa*/
StringKey stringCodeCreate(char *str) {
	StringCode sc;
	strcpy_s(sc.code, sizeof(sc.code), str);
	return sc;
}

/*Imprime a chave do mapa*/
void mapKeyPrint(MapKey key) {
	printf("%s", key.code);
}

/*Imprime um item do mapa*/
void mapValuePrint(MapValue value) {
	itemPrint(&value);
}

/* funcao de comparacao de chaves */
int mapKeyEquals(MapKey key1, MapKey key2) {
	return (strcmp(key1.code, key2.code) == 0);
}
#include "map.h"
#include <stdlib.h>
#include <stdio.h>

/* map.c - Implementation for ADT Map with array list */

typedef struct keyValue {
	MapKey key;
	MapValue value;
} KeyValue;

typedef struct mapImpl {
	/* Abordagem 1:*/
	//MapKey *keys;
	//MapValue *values;

	/* Abordagem 2:*/
	KeyValue *elements;

	/*-------------- */
	unsigned int size;
	unsigned int capacity;
} MapImpl;

int findKeyIndex(PtMap map, MapKey key) {
	if (map == NULL) return -1;

	for (int i = 0; i < map->size; i++) {
		if (mapKeyEquals(map->elements[i].key, key)) {
			return i;
		}
	}
	return -1;
}

/* Operacoes genéricas */
PtMap mapCreate(unsigned int initialCapacity) {
	PtMap newMap = (PtMap)malloc(sizeof(MapImpl));
	if (newMap == NULL) return NULL;

	newMap->elements = (KeyValue*)calloc(initialCapacity, sizeof(KeyValue));
	if (newMap->elements == NULL) {
		free(newMap);
		return NULL;
	}

	newMap->size = 0;
	newMap->capacity = initialCapacity;

	return newMap;
}

int mapDestroy(PtMap *ptMap) {
	PtMap map = *ptMap;

	if (map == NULL) return MAP_NULL;

	free(map->elements);
	free(map);

	*ptMap = NULL;

	return MAP_OK;
}

int mapSize(PtMap map, int *ptSize) {
	if (map == NULL) return MAP_NULL;
	*ptSize = map->size;
	return MAP_OK;
}

int mapIsEmpty(PtMap map) {
	if (map == NULL) return 1;
	return (map->size == 0);
}

int mapClear(PtMap map) {
	if (map == NULL) return MAP_NULL;
	map->size = 0;
	return MAP_OK;
}

void mapPrint(PtMap map) {
	if (map == NULL) {
		printf("(MAP NULL)\n");
	}
	else if (mapIsEmpty(map)) {
		printf("(MAP EMPTY)\n");
	}
	else {
		for (int i = 0; i < map->size; i++) {
			mapKeyPrint(map->elements[i].key);
			printf(" : ");
			mapValuePrint(map->elements[i].value);
		}
	}
}

/* Operacoes específicas */
int mapPut(PtMap map, MapKey key, MapValue value) {
	if (map == NULL) return MAP_NULL;

	int index = findKeyIndex(map, key);
	if (index != -1) {
		map->elements[index].value = value;
		return MAP_OK;
	}
	else {
		if (map->size == map->capacity) {
			//TODO: modificar para aumentar dinamicamente o array
			return MAP_FULL;
		}

		map->elements[map->size].key = key;
		map->elements[map->size].value = value;
		map->size++;
		return MAP_OK;
	}
}

int mapRemove(PtMap map, MapKey key, MapValue *ptValue) {
	if (map == NULL) return MAP_NULL;
	if (map->size == 0) return MAP_EMPTY;

	int index = findKeyIndex(map, key);
	if (index != -1) {
		*ptValue = map->elements[index].value;
		map->elements[index] = map->elements[map->size - 1];
		map->size--;
		return MAP_OK;
	}
	else {
		return MAP_UNKNOWN_KEY;
	}
}

int mapContains(PtMap map, MapKey key) {
	if (map == NULL) return 0;

	return findKeyIndex(map, key) != -1;

	/*
	for (int i = 0; i < map->size; i++) {

		if ( mapKeyCompare(map->elements[i].key , key) ) {
			return 1;
		}
	}

	return 0;*/
}

int mapGet(PtMap map, MapKey key, MapValue *ptValue) {
	if (map == NULL) return MAP_NULL;
	if (map->size == 0) return MAP_EMPTY;

	int index = findKeyIndex(map, key);
	if (index != -1) {
		*ptValue = map->elements[index].value;
		return MAP_OK;
	}
	else {
		return MAP_UNKNOWN_KEY;
	}
	/*for (int i = 0; i < map->size; i++) {

		if (mapKeyCompare(map->elements[i].key, key)) {
			*ptValue = map->elements[i].value;
			return MAP_OK;
		}
	}

	return MAP_UNKNOWN_KEY;*/
}

MapKey* mapKeys(PtMap map) {
	if (map == NULL) return NULL;

	MapKey *keys = (MapKey*)calloc(map->size, sizeof(MapKey));

	for (int i = 0; i < map->size; i++) {
		keys[i] = map->elements[i].key;
	}

	return keys;
}

MapValue* mapValues(PtMap map) {
	if (map == NULL) return NULL;

	MapValue *values = (MapValue*)calloc(map->size, sizeof(MapValue));

	for (int i = 0; i < map->size; i++) {
		values[i] = map->elements[i].value;
	}

	return values;
}
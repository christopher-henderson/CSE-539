#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "hash.h"

typedef struct e {
	char * value;
	char * key;
	struct Entry * next;
} Entry;

Entry * NewEntry(char* key, char * value) {
	Entry * e = malloc(sizeof(Entry));
	e->key = key;
	e->value = value;
	e->next = NULL;
	return e;
}

typedef struct m {
	Entry ** values;
	int length;
	int cap;
} Map;

Map * NewMap(int cap) {
	Map * map = malloc(sizeof(Map));
	map->length = 0;
	map->cap = cap;
	map->values = calloc(sizeof(Entry), sizeof(Entry) * map->cap);
	return map;
}

void put(Map * map, char * key, char * value) {
	int i = (unsigned int)hash(key) % map->cap;
	if (map->values[i] == NULL) {
		map->values[i] = NewEntry(key, value);
		map->length++;
		return;
	}
	if (strcmp(map->values[i]->key, key) == 0) {
		map->values[i]->value = value;
		return;
	}
	Entry * next = (Entry *) map->values[i]->next;
	while (next != NULL) {
		if (strcmp(next->key, key) == 0) {
			map->values[i]->key = key;
			map->values[i]->value = value;
			return;
		}
		if (next->next == NULL) {
			next->next = (struct Entry *) NewEntry(key, value);
			map->length++;
			return;
		}
		next = (Entry *) next->next;
	}
	map->values[i]->next = (struct Entry *) NewEntry(key, value);
	map->length++;
}

char * get(Map * map, char * key) {
	int i = (unsigned int)hash(key) % map->cap;
	Entry * e = map->values[i];
	if (e == NULL) {
		return NULL;
	}
	if (strcmp(e->key, key) == 0) {
		return e->value;
	}
	Entry * next = (Entry *) e->next;
	while (next != NULL) {
		if (strcmp(next->key, key) == 0) {
			return next->value;
		}
		next = (Entry *) next->next;
	}
	return NULL;
}

void delete(Map * map, char * key) {
	int i = (unsigned int)hash(key) % map->cap;
	Entry * e = map->values[i];
	if (e == NULL) {
		return;
	}
	if (e != NULL && strcmp(e->key, key) == 0) {
		map->values[i] = (Entry *) e->next;
		free(e);
		map->length--;
		return;
	}
	Entry * next = (Entry *) e->next;
	Entry * prev = e;
	while (next != NULL) {
		if (strcmp(next->key, key) == 0) {
			prev->next = next->next;
			free(next);
			map->length--;
			return;
		}
		next = (Entry *) next->next;
	}
}

#endif
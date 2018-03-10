#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "md5.h"

const int DEFAULT_CAP = 100;

MD5_CTX mdContext;  // needed to compute MD5

int hash(char * word) {
	int result;
	int * temp;
	MD5Init(&mdContext);  // compute MD5 of password
	MD5Update(&mdContext, (unsigned char *)word, 4);
	MD5Final(&mdContext);
	temp = (int *) &mdContext.digest[12]; 
	result = *temp; // result is 32 bits of MD5 -- there is a BUG here, oh well.
	return result;
}

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
		// if (0 == map->length % 100000) {
		// 	printf("plain %i\n", map->length);
		// }
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
			// if (0 == map->length % 100000) {
			// 	printf("N bucket %i\n", map->length);
			// }
			return;
		}
		next = (Entry *) next->next;
	}
	map->values[i]->next = (struct Entry *) NewEntry(key, value);
	map->length++;
	// if (0 == map->length % 100000) {
	// 	printf("1 bucket%i\n", map->length);
	// }
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


// int main() {

// 	// Map * m = NewMap(2);
// 	put(m, "bob", "bob");
// 	put(m, "alice", "alice");
// 	printf("%s\n", get(m ,"bob"));
// 	printf("%s\n", get(m ,"alice"));
// 	put(m, "eve", "eve");
// 	printf("%s\n", get(m ,"eve"));
// 	printf("%i\n", m->length);
// 	delete(m, "alice");
// 	printf("%s\n", get(m, "bob"));
// 	printf("%i\n", m->length);
// 	delete(m, "bob");
// 	printf("%s\n", get(m, "bob"));
// 	printf("%s\n", get(m, "alice"));
// 	put(m, "alice", "alice");
// 	printf("%s\n", get(m, "alice"));
// }
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib/transform.h"
#include "lib/hashmap.h"
#include "lib/hash.h"

const int CHAIN_LEGNTH = 100000;
FILE * file;

void put_and_log_count(Map * map, char * value) {
	put(map, value, value);
	if (map->length % 100000 == 0) {
		printf("Computed %i hashes\n", map->length);
	}
}

void compute_hash_chain(Map *visited, const char *buf) {
	char * start = malloc(sizeof(char) * 5);
	strcpy(start, buf);
	char * next;
	int i = 0;

	put_and_log_count(visited, start);
	char * prev = start;
	while (1) {
		if (!(i < CHAIN_LEGNTH - 1) || !(visited->length < MAX_COMBINATIONS)) {
			// Chain completed, or complete table completed.
			fputs(prev, file);
			fputs(",", file);
			fputs(start, file);
			fputs("\n", file);
			fflush(file);;
			return;
		}
		next = reversal(hash(prev));
		if (get(visited, next) == NULL) {
			put_and_log_count(visited, next);
			i++;
			prev = next;
			continue;
		}
		// Cycle detected. Dump out the chain up to
		// the one right before the cycle.
		fputs(prev, file);
		fputs(",", file);
		fputs(start, file);
		fputs("\n", file);
		fflush(file);
		return;
	}
}

// Build all combinations. If the combination has not 
// been seen before, then compute a hash chain for it.
void compute(Map *visited, char * buf, int start, int end) {
	if (start >= end) {
		if (visited->length >= MAX_COMBINATIONS) {
			return;
		}
		if (get(visited, buf) == NULL) {
			compute_hash_chain(visited, buf);
		}
		return;
	}
	for (int i = 0; i < strlen(characters); i++) {
		buf[start] = characters[i];
		compute(visited, buf, start + 1, end);
	}
}

int main() {
	file = fopen("rainbowtable.csv", "w");
	char buf[5];
	buf[4] = 0; // null terminator
	Map * visited = NewMap(14776336 * 3);
	compute(visited, buf, 0, 4);
	fclose(file);
	printf("Computed %i hashes\n", visited->length);
	return 0;
}
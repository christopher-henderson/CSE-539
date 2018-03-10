#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include "md5.h" 
#include "hashmap.c"

const int CHAIN_LEGNTH = 100;
FILE * file;


char start_points[7688][5];
char endpoints_points[7688][5];
int lookup[7688]; // hash of endpoint gives index of startpoint

char characters[62] = {
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'0','1','2','3','4','5','6','7','8','9'};

char itoc(int i) {
	return characters[i % 62];
}

char * reversal(int in) {
	char * r = malloc(sizeof(char) * 5);
	r[0] = itoc((in >> 24) & 0xFF);
	r[1] = itoc((in >> 16) & 0xFF);
	r[2] = itoc((in >> 8) & 0xFF);
	r[3] = itoc(in & 0xFF);
	r[4] = 0; // null terminator
	return r;
}

void put_and_log_count(Map * map, char * value) {
	put(map, value, value);
	if (map->length % 100000 == 0) {
		printf("Computed %i hashes\n", map->length);
	}
}

void build(Map *visited, const char *buf) {
	char * start = malloc(sizeof(char) * 5);
	strcpy(start, buf);
	char * next;
	int i = 0;

	put_and_log_count(visited, start);
	char * prev = start;
	while (1) {
		if (!(i < CHAIN_LEGNTH - 1) || !(visited->length < 14776336)) {
			fputs(prev, file);
			fputs(":", file);
			fputs(start, file);
			fputs("\n", file);
			fflush(file);
			free(next);
			free(start);
			return;
		}
		next = reversal(hash(prev));
		if (get(visited, next) == NULL) {
			put_and_log_count(visited, next);
			i++;
			prev = next;
			// free(next);
			continue;
		}
		fputs(prev, file); // DIFFERENT! Don't make the cycle the end.
		fputs(":", file);
		fputs(start, file);
		fputs("\n", file);
		fflush(file);
		free(next);
		free(start);
		return;
	}
}

void combinations(Map *visited, char * buf, int start, int end) {
	if (start >= end) {
		if (visited->length >= 14776336) {
			return;
		}
		if (get(visited, buf) == NULL) {
			build(visited, buf);
		}
		return;
	}
	for (int i = 0; i < strlen(characters); i++) {
		buf[start] = characters[i];
		combinations(visited, buf, start + 1, end);
	}
}

int main() {
	// printf("%x\n", hash("aaaa"));

	file = fopen("dammit", "w");
	char buf[5];
	buf[4] = 0; // null terminator
	Map * visited = NewMap(14776336 * 3);
	combinations(visited, buf, 0, 4);
	fclose(file);
	printf("%i\n", visited->length);
	// char * pass = "abcd";
	// printf("%s\n", reversal(hash(pass)));
	// char * s = reversal(hash(pass));
	// printf("%i\n", strcmp(s, "c7fB"));
	return 0;
}
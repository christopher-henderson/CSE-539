#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib/hashmap.h" 
#include "lib/transform.h"
#include "lib/hash.h"

Map * load_table() {
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("rainbowtable.csv", "r");
    if (fp == NULL) {
        exit(1);
    }

    Map * map = NewMap(14776336 * 3);
    char * start = NULL;
    char * end = NULL;
    while ((read = getline(&line, &len, fp)) != -1) {
        line[4] = 0;
        line[read - 1] = 0;
        start = malloc(sizeof(char) * 5);
        end = malloc(sizeof(char) * 5);
        strcpy(start, line + 5);
        strcpy(end, line);
        put(map, end, start);
    }

    fclose(fp);
    if (line) {
        free(line);
    }
    return map;
}

void crack(Map * map, int target) {
	char * start;
	char * guess = reversal(target);
	int temp = hash(guess);
	int guesses = 0;
	while(1) {
		guesses++;
		if (temp == target) {
			printf("Is your password %s?\n", guess);
			printf("Got it in %i guesses\n", guesses);
			return;
		}
		if ((start = get(map, guess)) != NULL) {
			guess = start;
		} else {
			guess = reversal(temp);
		}
		temp = hash(guess);
	}
}

int main() {
	printf("%s\n", "Loading table...");
	Map * map = load_table();
	printf("Enter in the target hash: ");
	fflush(0);
	char target_hash[8];
	read(0, target_hash, 8);
	int target;
	sscanf(target_hash, "%x", &target);
	crack(map, target);
	return 0;
}
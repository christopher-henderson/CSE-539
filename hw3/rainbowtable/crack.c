#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashmap.c" 

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

Map * load_table() {
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("dammit", "r");
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
	while(1) {
		if (temp == target) {
			printf("Is your password %s?\n", guess);
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
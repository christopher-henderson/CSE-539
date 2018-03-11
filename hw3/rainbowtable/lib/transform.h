#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#define NUM_CHARACTERS 62
#define MAX_COMBINATIONS 14776336

char characters[NUM_CHARACTERS] = {
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'0','1','2','3','4','5','6','7','8','9'};

char itoc(int i) {
	return characters[i % NUM_CHARACTERS];
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

#endif
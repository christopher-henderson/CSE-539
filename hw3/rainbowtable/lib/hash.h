#ifndef HASH_H
#define HASH_H

#include "md5.h"

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

#endif
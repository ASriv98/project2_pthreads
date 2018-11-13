#ifndef password_crack_h
#define password_crack_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <ctime>
#include <pthread.h>
#include <unistd.h>

char map(int convert);
void* crack(void* args);
struct params {
	char* password;
	int len;
};

#endif
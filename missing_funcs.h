#ifndef MISSING_FUNCS_H
#define MISSING_FUNCS_H

#include <stdlib.h>

void  *memset(void *b, int c, int len);

void * memcpy(void* dst, const void* src, unsigned int cnt);

char* strcpy(char* destination, const char* source);

double sqrt(double n);

double pow (double x, int y);

double fabs (double x);

char *strcat(char *dest, const char *src);

double cos(double x);

double sin(double x);

double tan(double x);

#endif
#ifndef utility_h_
#define utility_h_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef __USE_GNU
#define __USE_GNU
#endif
#define SOL 99
#include <ucontext.h>

ucontext_t* getContext();
void error(char* string);
void test(char* string, int boolean);
void printColor(char* string, int attribute, int color);
void* mallocSafely(size_t size);
void printUContext(ucontext_t* mycontext);

#endif

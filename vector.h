#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef struct vec {
    float* elems; // elements contained within vector
    size_t alloc; // num of allocated bytes
    size_t size; // num of elements
} vec;

vec* init_vec(size_t size);
vec* copy(vec* v);
int set(vec* v, size_t index, float replacement);
void resize(vec* v, size_t mutation_size);
void del(vec* v);
void merge(vec* v, vec* v2);
void append(vec* v, float item);
void to_string(vec* v);
vec* scaled(vec* v, float s);
float* dot(size_t n, vec* v, vec* v2, ...);
float* theta_between(vec* v, vec* v2);
vec* norm(vec* v);

#endif // VECTOR_H_INCLUDED

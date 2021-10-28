#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#define to_string(X) _Generic((X),                      \
                         vec*: to_stringv,              \
                         mat*: to_stringm               \
                         )(X)

typedef struct vec {
    float* elems; // elements contained within vector
    size_t dim;
    size_t alloc; // num of allocated bytes
    size_t size; // num of elements
} vec;

enum errors {
    NONE,
    INDEX_ERROR,
    MEM_ERROR,
    MATH_ERROR
};

vec* init_vec(size_t size);
void* copy(void* o);
void* setv(vec* v, size_t index, float replacement);
void* resizev(vec* v, size_t mutation_size);
void del(vec* v);
void* mergev(vec* v, vec* v2);
void* append(vec* v, float item);
void to_stringv(vec* v);
vec* sum(size_t n, vec* v, vec* v2, ...);
vec* prod(size_t n, vec* v, vec* v2, ...);
vec* scale(vec* v, float s);
float* dot(size_t n, vec* v, vec* v2, ...);
float* theta_between(vec* v, vec* v2);
vec* perp(vec* v);
vec* norm(vec* v);
vec* proj(vec* v, vec* v2);
vec* cross(vec* v, vec* v2);

#endif // VECTOR_H_INCLUDED

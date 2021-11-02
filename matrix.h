#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "vector.h"

typedef struct mat {
    size_t alloc; // num of allocated bytes
    size_t w, h; // dimensions for the matrix
    vec** cols; // each column is a vector
} mat;

mat* init_mat(size_t w, size_t h);
void to_stringm(mat* m);
void* filterm(mat* m, vec* target);
mat* splicem(const mat* m, size_t w1, size_t w2);
void delm(mat* m);
size_t countm(const mat* m, const vec* v);
void* setm(mat* m, size_t x, size_t y, float replacement);
void* resizem(mat* m, size_t w, size_t h);
mat* identity(size_t w, size_t h);
float* detm(mat* m);
int cmpmx(const void* p1, const void* p2);
int cmpmy(const void* p1, const void* p2);

#endif // MATRIX_H_INCLUDED

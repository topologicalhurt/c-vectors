#include "matrix.h"

static VECTOR_SIZE = sizeof(vec);
MATRIX_SIZE = sizeof(mat);

mat* init_mat(size_t w, size_t h) {
    mat* nmat = malloc(MATRIX_SIZE);
    nmat->w = w;
    nmat->h = h;
    nmat->alloc = w * VECTOR_SIZE;
    nmat->cols = malloc(nmat->alloc);
    for(int i = 0; i < w; i++) {
       nmat->cols[i] = init_vec(h);
    };
    return nmat;
}

void to_stringm(mat* m) {
    printf("[\n");
    for(int i = 0; i < m->w - 1; i++) {
        printf("\t");
        to_string(m->cols[i]);
    };
    printf("\t");
    to_string(m->cols[m->w - 1]);
    printf("]\n");
}

void* setm(mat* m, size_t x, size_t y, float replacement) {
    if(x > m->w || y > m->h) {
        return NULL;
    }
    *((*(m->cols + y)) -> elems + x) = replacement;
}

void delm(mat* m) {
    delv(*(m->cols));
    free(m->cols);
    free(m);
}

void* resizem(mat* m, size_t w, size_t h) {
    vec** tmp = realloc(m->cols, m->alloc);
    if(tmp == NULL) {
        return NULL;
    }
    m->cols = tmp;
    m->alloc = w * VECTOR_SIZE;
    if(w > m->w) {
        for(int i = m->w; i < w; i++) {
            m->cols[i] = init_vec(h);
        };
    } else {
        for(int i = w; i < m->w; i++) {
            delv(m->cols[i]);
        }
    }
    for(int i = 0; i < w; i++) {
        resizev(m->cols[i], h);
    };
    m->w = w;
    m->h = h;
}

mat* identity(size_t w, size_t h) {
    mat* nmat = init_mat(w, h);
    if(w != h) {
        return NULL;
    }
    for(int i = 0; i < h; i++) {
        setm(nmat, i, i, 1);
    };
    return nmat;
}

float* det(mat* m) {

}



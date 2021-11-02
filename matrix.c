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
void* setcolm(mat* m, size_t x, vec* replacement) {
    if(x > m->w) {
        return NULL;
    }
    m->cols[x] = replacement;
}

void* filterm(mat* m, vec* target) {
    size_t target_cnt = countm(m, target);
    if(target_cnt == 0) {
        return m;
    }
    mat* nmat = init_mat(m->w, m->h);
    int j = 0;
    for(int i = 0; i < m->w; i++) {
        if(equalsv(m->cols[i], target) != 0) {
            setcolm(nmat, j++, m->cols[i]);
        }
    }
    resizem(nmat, nmat->w - target_cnt, nmat->h);
    *m = *nmat;
}

void* setm(mat* m, size_t x, size_t y, float replacement) {
    if(x > m->w || y > m->h) {
        return NULL;
    }
    setv(m->cols[x], y, replacement);
}

void delm(mat* m) {
    delv(*(m->cols));
    free(m->cols);
    free(m);
}

size_t countm(const mat* m, const vec* v) {
    size_t count = 0;
    for(int i = 0; i < m->w; i++) {
        count = equalsv(m->cols[i], v) == 0 ? ++count : count;
    }
    return count;
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
            setcolm(m, i, init_vec(h));
        };
    } else {
//        for(int i = w; i < m->w; i++) {
//            delv(m->cols[i]);
//        };
    }
    for(int i = 0; i < w; i++) {
        resizev(m->cols[i], h);
    };
    m->w = w;
    m->h = h;
}

mat* splicem(const mat* m, size_t w1, size_t w2) {
    if(w1 > w2) {
        return NULL;
    }
    mat* nmat = init_mat(w2 - w1, m->h);
    for(int i = w1; i < w2; i++) {
        setcolm(nmat, i - w1, m->cols[i]);
    };
    return nmat;
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

float* detm(mat* m) {

}

int cmpmx(const void* p1, const void* p2) {
    vec* v1 = *((vec**)p1);
    vec* v2 = *((vec**)p2);
    return (int) v1->elems[0] - v2->elems[0];
}

int cmpmy(const void* p1, const void* p2) {
    vec* v1 = *((vec**)p1);
    vec* v2 = *((vec**)p2);
    return (int) v1->elems[1] - v2->elems[1];
}



#include "vector.h"

DEFAULT_DYNAMIC_MEM = 0xA;
FLOAT_SIZE = sizeof(float);
VECTOR_SIZE = sizeof(vec);

vec* init_vec(size_t size) {
    vec* nvec = malloc(VECTOR_SIZE); // allocate memory for vector struct
    nvec->dim = size;
    nvec->size = size > DEFAULT_DYNAMIC_MEM ? size : DEFAULT_DYNAMIC_MEM; // set default size/capacity of vector (10)
    nvec->alloc = FLOAT_SIZE * nvec->size; // determine the amount of bytes allocated
    nvec->elems = calloc(nvec->size, FLOAT_SIZE); // create contiguous memory for elements
    return nvec;
}

vec* copyv(vec* v) {
    vec* nvec = init_vec(v->dim);
    for(int i = 0; i < nvec->size; i++) {
        nvec->elems[i] = v->elems[i];
    }
    return nvec;
}

void* setv(vec* v, size_t index, float replacement) {
    if(v->dim < index) {
        return NULL;
    }
    v->elems[index] = replacement;
}

void* resizev(vec* v, size_t nsize) {
    size_t alloc = FLOAT_SIZE * nsize;
    vec* tmp = realloc(v->elems, alloc);
    if(tmp == NULL) {
        return NULL;
    }
    v->elems = tmp;
    v->size = nsize;
    v->alloc = alloc; // store the current vector size after resize
}

void delv(vec* v) {
    free(v->elems);
    free(v);
}

int* equalsv(const vec* v, const vec* v2) {
    int* tmp = memcmp(v->elems, v2->elems, sizeof(float*));
    if(tmp == NULL) {
        return NULL;
    }
    return tmp;
}

void* mergev(vec* v, vec* v2) {
    size_t psize = v->dim;
    size_t nsize = psize + v2->dim <= DEFAULT_DYNAMIC_MEM ? DEFAULT_DYNAMIC_MEM :
        psize + v2->dim;
    v->size = nsize;
    v->dim += v2->dim;
    if(nsize > DEFAULT_DYNAMIC_MEM) {
        resizev(v, nsize);
        void* tmp = memcpy(&(v->elems[psize]),
        v2->elems,
        (nsize - psize) * FLOAT_SIZE);
        if(tmp == NULL) {
            return NULL;
        }
    } else {
        void* tmp = memcpy(&(v->elems[psize]),
        v2->elems,
        v2->dim * FLOAT_SIZE);
        if(tmp == NULL) {
            return NULL;
        }
    }
}

void* appendv(vec* v, float item) {
    v->dim++;
    void* tmp;
    size_t nsize = v->dim <= DEFAULT_DYNAMIC_MEM ? DEFAULT_DYNAMIC_MEM :
        v->dim;
    if(nsize > DEFAULT_DYNAMIC_MEM) {
        resizev(v, v->dim);
    }
    setv(v, v->dim - 1, item);
}

void to_stringv(vec* v) {
    float* elems = v->elems;
    size_t elem_size = v->dim;
    char buffer[0x1A]; // 24 char bytes for float, 1 char byte for comma, 1 char byte for null terminator
    printf("[");
    for(int i = 0; i < elem_size - 1; i++) {
        snprintf(buffer, 0x1A, "%f,", elems[i]); // load buffer
        printf("%s", buffer); // print chars from buffer
    };
    printf("%f]\n", elems[elem_size - 1]);
}

vec* sumv(size_t n, vec* v, vec* v2, ...) {
    if(n < 2) {
        return NULL;
    }
    size_t elem_size = v->dim; // length of first vector is assumed dimension
    vec* nvec = copyv(v);
    for(int i = 0; i < elem_size; i++) {
        nvec->elems[i] += v2->elems[i];
    }
    if(n == 2) {
        return nvec;
    } else {
        va_list args;
        va_start(args, n - 2);
        for(int i = 0; i < n - 2; i++) {
            vec* arg = va_arg(args, vec*);
            for(int j = 0; j < elem_size; j++) {
                nvec->elems[j] += arg->elems[j];
            }
        }
        va_end(args);
        return nvec;
    }
}

vec* prodv(size_t n, vec* v, vec* v2, ...) {
    if(n < 2) {
        return NULL;
    }
    vec* nvec = copyv(v);
    size_t elem_size = nvec->dim; // length of first vector is assumed dimension
    for(int i = 0; i < elem_size; i++) {
        nvec->elems[i] *= v2->elems[i];
    }
    if(n == 2) {
        return nvec;
    } else {
        va_list args;
        va_start(args, n - 2);
        for(int i = 0; i < n - 2; i++) {
            vec* arg = va_arg(args, vec*);
            for(int j = 0; j < elem_size; j++) {
                nvec->elems[j] *= arg->elems[j];
            }
        }
        va_end(args);
        return nvec;
    }
}

vec* scalev(vec* v, float s) {
    vec* nvec = copyv(v);
    for(int i = 0; i < v->dim; i++) {
        nvec->elems[i] *= s;
    };
    return nvec;
}

static float squared_mag(const vec* v) {
    vec* nvec = copyv(v);
    float mag_squared = 0;
    for(int i = 0; i < v->dim; i++) {
        float comp = nvec->elems[i];
        mag_squared += comp * comp;
    };
    return mag_squared;
}

float magv(vec* v) {
    return sqrt(squared_mag(v)); // Lazily, we just use the square root here.
}

float distv(vec* v, vec* v2) {
    return magv(sumv(2, v, scalev(v2, -1)));
}

float* dotv(size_t n, vec* v, vec* v2, ...) {
    if(n < 2) {
        return NULL;
    }
    size_t elem_size = v->dim; // dimension of first vector is assumed dimension
    float dot_product = 0;
    if(n == 2) {
        for(int i = 0; i < elem_size; i++) {
            dot_product += v->elems[i] * v2->elems[i];
        };
        float* pdot = (float*)&dot_product;
        return pdot;
    } else {
        va_list args;
        va_start(args, n - 2);
        float product;
        for(int i = 0; i < elem_size; i++) {
            product = v->elems[i] * v2->elems[i];
            for(int j = 0; j < n - 2; j++) {
                product *=  va_arg(args, vec*)->elems[i];
            };
            dot_product += product;
            va_start(args, n - 2);
        };
        va_end(args);
        float* pdot = (float*)&dot_product;
        return pdot;
    }
}

float* theta_between(vec* v, vec* v2) {
    float denom = sqrt(squared_mag(v)) * sqrt(squared_mag(v2));
    if(denom == 0) {
        return NULL;
    }
    float theta = acosf(*dotv(2, v, v2) / denom);
    float* ptheta = (float*)&theta;
    return ptheta;
}

vec* perpv(vec* v) {
    if(v->dim != 2) {
        return NULL;
    }
    vec* nvec = copyv(v);
    float tmp = nvec->elems[0];
    float replacement = nvec->elems[1];
    replacement *= -1;
    nvec->elems[0] = replacement;
    nvec->elems[1] = tmp; // swap components and negate
    return nvec;
}

static float fast_inverse_square_root(float n) {
    /*
    Full credit to the quake three team.
    Relevant licensing included in project file.

    Calculates the inverse square root of a floating point.
    */

	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = n * 0.5F;
	y  = n;
	i  = * ( long * ) &y;
	i  = 0x5f3759df - ( i >> 1 );
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );
    y  = y * ( threehalfs - ( x2 * y * y ) );

	return y;
}

vec* normv(vec* v) {
    return scalev(v, fast_inverse_square_root(squared_mag(v)));
}

vec* projv(vec* v, vec* v2) {
    /*
    Return the vector corresponding to the projection of vector v onto vector v2
    */
    return scalev(v, *dotv(2, v, v2) / squared_mag(v2));
}

vec* crossv(vec* v, vec* v2) {
    if(v->dim != 3 || v2->dim != 3) {
        return NULL;
    }
    float* comps = v->elems;
    float* comps2 = v2->elems;
    float a1 = comps[0], a2 = comps[1], a3 = comps[2],
            b1 = comps2[0], b2 = comps2[1], b3 = comps2[2];
    vec* nvec = init_vec(3);
    setv(nvec, 0, a2 * b3 - a3 * b2);
    setv(nvec, 1, a3 * b1 - a1 * b3);
    setv(nvec, 2, a1 * b2 - a2 * b1);
    return nvec;
}














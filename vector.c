#include "vector.h"

#define DEFAULT_DYNAMIC_MEM 0xA
#define FLOAT_SIZE sizeof(float)
#define VECTOR_SIZE sizeof(vec)

vec* init_vec(size_t size) {
    vec* nvec = (vec*) malloc(VECTOR_SIZE); // allocate memory for vector struct
    nvec -> size = size; // set default size/capacity of vector (10)
    nvec -> alloc = FLOAT_SIZE * size; // determine the amount of bytes allocated
    nvec -> elems = calloc(size, FLOAT_SIZE); // create contiguous memory for elements
    return nvec;
}

vec* copy(vec* v) {
    vec* nvec = (vec*) malloc(VECTOR_SIZE);
    memcpy(nvec, v, VECTOR_SIZE);
    return nvec;
}

int set(vec* v, size_t index, float replacement) {
    if(v -> size < index) {
        return -1;
    }
    *(v -> elems + index) = replacement;
    return 1;
}

void resize(vec* v, size_t nsize) {
    v -> size = nsize;
    v -> alloc = FLOAT_SIZE * nsize; // store the current vector size after resize
    v -> elems = realloc(v -> elems, v -> alloc); // reallocate the amount of memory for storing elements
}

void del(vec* v) {
    free(v -> elems);
    free(v);
}

void merge(vec* v, vec* v2) {
    size_t prior_size = v -> size;
    resize(v, v -> size + v2 -> size); // resize vector based on spanned length
    memcpy(&(v -> elems[prior_size]),
        v2 -> elems,
        v -> alloc);
}

void append(vec* v, float item) {
   resize(v, v -> size + 1);
   memcpy(&(v -> elems[v -> size - 1]),
        &item,
        v -> alloc); // special 1D case of merge
}

void to_string(vec* v) {
    float* elems = v -> elems;
    size_t elem_size = v -> size;
    char buffer[0x1A]; // 24 char bytes for float, 1 char byte for comma, 1 char byte for null terminator
    printf("[");
    for(int i = 0; i < elem_size - 1; i++) {
        snprintf(buffer, 0x1A, "%f,", elems[i]); // load buffer
        printf("%s", buffer); // print chars from buffer
    };
    printf("%f]\n", elems[elem_size - 1]);
}

vec* sum(size_t n, vec* v, vec* v2, ...) {
    if(n < 2) {
        return NULL;
    }
    size_t elem_size = v -> size; // length of first vector is assumed dimension
    vec* sumvec = copy(v);
    for(int i = 0; i < elem_size; i++) {
        sumvec -> elems[i] += v2 -> elems[i];
    }
    if(n == 2) {
        return sumvec;
    } else {
        va_list args;
        va_start(args, n - 2);
        for(int i = 0; i < n - 2; i++) {
            vec* arg = va_arg(args, vec*);
            for(int j = 0; j < elem_size; j++) {
                sumvec -> elems[j] += arg -> elems[j];
            }
        }
        va_end(args);
        return sumvec;
    }
}

vec* prod(size_t n, vec* v, vec* v2, ...) {
    if(n < 2) {
        return NULL;
    }
    size_t elem_size = v -> size; // length of first vector is assumed dimension
    vec* prodvec = copy(v);
    for(int i = 0; i < elem_size; i++) {
        prodvec -> elems[i] *= v2 -> elems[i];
    }
    if(n == 2) {
        return prodvec;
    } else {
        va_list args;
        va_start(args, n - 2);
        for(int i = 0; i < n - 2; i++) {
            vec* arg = va_arg(args, vec*);
            for(int j = 0; j < elem_size; j++) {
                prodvec -> elems[j] *= arg -> elems[j];
            }
        }
        va_end(args);
        return prodvec;
    }
}

vec* scale(vec* v, float s) {
    vec* nvec = copy(v);
    for(int i = 0; i < v -> size; i++) {
        nvec -> elems[i] *= s;
    };
    return nvec;
}

static float squared_mag(vec* v) {
    float mag_squared = 0;
    for(int i = 0; i < v -> size; i++) {
        float comp = v -> elems[i];
        mag_squared += comp * comp;
    };
    return mag_squared;
}

float* dot(size_t n, vec* v, vec* v2, ...) {
    if(n < 2) {
        return NULL;
    }
    size_t elem_size = v -> size; // length of first vector is assumed dimension
    float dot_product = 0;
    if(n == 2) {
        for(int i = 0; i < elem_size; i++) {
            dot_product += v -> elems[i] * v2 -> elems[i];
        };
        float* pdot = (float*)&dot_product;
        return pdot;
    } else {
        va_list args;
        va_start(args, n - 2);
        float product;
        for(int i = 0; i < elem_size; i++) {
            product = v -> elems[i] * v2 -> elems[i];
            for(int j = 0; j < n - 2; j++) {
                product *=  va_arg(args, vec*) -> elems[i];
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
    float theta = acosf(*dot(2, v, v2) / denom);
    float* ptheta = (float*)&theta;
    return ptheta;
}

vec* perp(vec* v) {
    vec* nvec = copy(v);
    if(v -> size != 2) {
        return NULL;
    }
    float temp = nvec -> elems[0];
    float replacement = nvec -> elems[1];
    replacement *= -1;
    nvec -> elems[0] = replacement;
    nvec -> elems[1] = temp; // swap components and negate
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

vec* norm(vec* v) {
    return scale(v, fast_inverse_square_root(squared_mag(v)));
}

vec* proj(vec* v, vec* v2) {
    /*
    Return the vector corresponding to the projection of vector v onto vector v2
    */
    vec* nvec = copy(v2);
    return scale(nvec, *dot(2, v, v2) / squared_mag(v2));
}

vec* cross(vec* v, vec* v2) {
    if(v -> size != 3 || v2 -> size != 3) {
        return NULL;
    }
    float* comps = v -> elems;
    float* comps2 = v2 -> elems;
    float a1 = comps[0], a2 = comps[1], a3 = comps[2],
            b1 = comps2[0], b2 = comps2[1], b3 = comps2[2];
    vec* nvec = init_vec(3);
    set(nvec, 0, a2 * b3 - a3 * b2);
    set(nvec, 1, a3 * b1 - a1 * b3);
    set(nvec, 2, a1 * b2 - a2 * b1);
    return nvec;
}















#include "pilot.h"

vert* init_vert(size_t w, size_t h) {
    vert* nvert = malloc(sizeof(vert));
    nvert->coords = init_mat(w, h);
    nvert->dist = INFINITY;
    return nvert;
}

size_t get_num_walls(struct Wall_collection* head) {
    size_t l = 0;
    struct Wall_collection* ptr = head;
    while(ptr != NULL) {
        l++;
        ptr = ptr->next;
    };
    return l;
}

vert* min_euclidean_dist(const mat* set, int n) {
    float min = INFINITY;
    vert* nverts = init_vert(2, 2);
    for(int i = 0; i < n; ++i) {
        for(int j = i + 1; j < n; ++j) {
            float dist = distv(set->cols[i], set->cols[j]);
            if(dist < min) {
                min = dist;
                nverts->dist = min;
                setcolm(nverts->coords, 0, set->cols[i]);
                setcolm(nverts->coords, 1, set->cols[j]);
            }
        };
    };
    return nverts;
}

vert* packed_euclidean_dist(const mat* set, int n, float lb) {
    /* Proven to run in O(n) time according to:
    http://people.csail.mit.edu/indyk/6.838-old/handouts/lec17.pdf
    */
    float min = lb;
    vert* nverts = init_vert(2, 2);
    qsort(set->cols, set->w, sizeof(vec*), cmpmy);
    for(int i = 0; i < n; ++i) {
        for(int j = i + 1; j < n && (set->cols[j]->elems[1] -
            set->cols[i]->elems[1] < min); ++j) {
            float dist = distv(set->cols[i],
                  set->cols[j]);
            if(dist < min) {
                min = dist;
                nverts->dist = min;
                setcolm(nverts->coords, 0, set->cols[i]);
                setcolm(nverts->coords, 1, set->cols[j]);
            }
        }
    }
    return nverts;
}

vert* deep_euclidean_min_search(mat* set, int n) {
    if(n <= 3) {
        return min_euclidean_dist(set, n);
    }
    int mid = set->w >> 1;
    vec* mid_point = set->cols[mid];
    vert* left = deep_euclidean_min_search(splicem(set, 0, mid), mid);
    vert* right = deep_euclidean_min_search(splicem(set, mid, set->w), n - mid);
    float lmin = left->dist, rmin = right->dist;
    float lower_bound = lmin < rmin ? lmin : rmin;
    mat* nmat = init_mat(n, 2);
    int j = 0;
    for(int i = 0; i < n; i++) {
        if(abs(set->cols[i]->elems[0] - mid_point->elems[0]) < lower_bound) {
            nmat->cols[j++] = set->cols[i];
        }
    }
    vert* nverts = init_vert(2, 2);
    filterm(nmat, init_vec(2));
    vert* ped = packed_euclidean_dist(nmat, j, lower_bound);
    nverts->coords = lmin < rmin ? left->coords : right->coords;
    nverts->dist = lower_bound;
    return ped->dist < nverts->dist ? ped : nverts;
}


vert* find_closest_vert(struct Wall_collection* head) {
    struct Wall_collection* ptr = head;
    size_t num_walls = get_num_walls(head);
    if(num_walls <= 1) {
        return NULL;
    }
    mat* tmp = init_mat(num_walls, 2);
    mat* nmat = init_mat(num_walls, 2);
    size_t ro = 0;
    while(ptr != NULL) {
        struct Wall w = ptr->wall;
        setm(tmp, ro, 0, (float)w.x);
        setm(tmp, ro, 1, (float)w.y);
        ro++;
        ptr = ptr->next;
    };
    int j = 0;
    while(tmp->w > 0) {
        setcolm(nmat, j, tmp->cols[0]);
        filterm(tmp, tmp->cols[0]); // Filter out all non-unique elements
        j++;
    }

    resizem(nmat,j, nmat->h);
    qsort(nmat->cols, nmat->w, sizeof(vec*), cmpmx);
    vert* minverts = deep_euclidean_min_search(nmat, nmat->w >> 1);
    qsort(minverts->coords->cols, minverts->coords->w, sizeof(vec*), cmpmx);
    qsort(minverts->coords->cols, minverts->coords->w, sizeof(vec*), cmpmy);
    return minverts;
}

void test_draw_min_dist(SDL_Renderer * renderer, vert* verts) {
    float x0, x1, y0, y1;
    x0 = verts->coords->cols[0]->elems[0];
    x1 = verts->coords->cols[1]->elems[0];
    y0 = verts->coords->cols[0]->elems[1];
    y1 = verts->coords->cols[1]->elems[1];
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
    SDL_RenderPresent(renderer);
}

void test_auto_pilot(struct Robot* robot) {
    vec* NVEC = init_vec(4);
    setv(NVEC, 0, (float) robot->x);
    setv(NVEC, 1, (float) robot->y);
    setv(NVEC, 2, (float) robot->angle);
    setv(NVEC, 3, (float) robot->currentSpeed);
//    to_string(NVEC);
}







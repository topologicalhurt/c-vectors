#ifndef PILOT_H_INCLUDED
#define PILOT_H_INCLUDED

#include "../Template code/robot.h"
#include "vector.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct vert {
    mat* coords;
    float dist;
} vert;


void test_auto_pilot(struct Robot* robot);
void test_draw_min_dist(SDL_Renderer * renderer, vert* verts);
vert* find_closest_vert(struct Wall_collection* head);

#endif // PILOT_H_INCLUDED

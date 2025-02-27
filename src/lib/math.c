#include "math.h"

void Math_Vec2f_Sum(Vec2f* l, Vec2f* r, Vec2f* dest) {
    dest->x = l->x + r->x;
    dest->z = l->z + r->z;
}

void Math_Vec2f_Diff(Vec2f* l, Vec2f* r, Vec2f* dest) {
    dest->x = l->x - r->x;
    dest->z = l->z - r->z;
}

void Math_Vec2f_Scale(Vec2f* vec, f32 scale) {
    vec->x *= scale;
    vec->z *= scale;
}

void Math_Vec2f_ScaleAndStore(Vec2f* vec, f32 scale, Vec2f* dest) {
    dest->x = vec->x * scale;
    dest->z = vec->z * scale;
}
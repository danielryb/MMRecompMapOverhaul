#ifndef __MATH_H__
#define __MATH_H__

#include "z64math.h"

void Math_Vec2f_Sum(Vec2f* l, Vec2f* r, Vec2f* dest);
void Math_Vec2f_Diff(Vec2f* l, Vec2f* r, Vec2f* dest);
void Math_Vec2f_Scale(Vec2f* vec, f32 scale);
void Math_Vec2f_ScaleAndStore(Vec2f* vec, f32 scale, Vec2f* dest);

#endif /* __MATH_H__ */
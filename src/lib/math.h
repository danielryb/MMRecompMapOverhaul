#ifndef __MATH_H__
#define __MATH_H__

#include "z64math.h"

#define SGN(x) (((x) > 0) ? 1 : (((x) < 0) ? -1 : 0))

void Math_Vec2f_Sum(Vec2f* l, Vec2f* r, Vec2f* dest);
void Math_Vec2f_Diff(Vec2f* l, Vec2f* r, Vec2f* dest);
void Math_Vec2f_Scale(Vec2f* vec, f32 scale);
void Math_Vec2f_ScaleAndStore(Vec2f* vec, f32 scale, Vec2f* dest);
f32 Math_Vec2f_Len(Vec2f* vec);

#endif /* __MATH_H__ */
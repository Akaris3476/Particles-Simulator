
#ifndef PARTICLESIMULATOR_VMATH_H
#define PARTICLESIMULATOR_VMATH_H
#include <raylib.h>

float vdistance(Vector2 v1, Vector2 v2);

float vdot(Vector2 v1, Vector2 v2) ;

float vmagnitude(Vector2 v) ;

Vector2 vnormalize(Vector2 v);

Vector2 vmult(Vector2 v1, Vector2 v2);
Vector2 vmult(Vector2 v, float multiplier);

Vector2 vdiv(Vector2 v1, Vector2 v2);
Vector2 vdiv(Vector2 v, float divisor);

Vector2 vsub (Vector2 v1, Vector2 v2);

Vector2 vadd (Vector2 v1, Vector2 v2);

#endif //PARTICLESIMULATOR_VMATH_H

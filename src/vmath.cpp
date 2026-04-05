#include "vmath.h"
#include <cmath>

float vdistance(Vector2 v1, Vector2 v2) {
    return
    sqrt(
        pow((v2.x - v1.x), 2)
                    +
        pow((v2.y - v1.y), 2)
        );
}

float vdot(Vector2 v1, Vector2 v2) {
    return v1.x*v2.x + v1.y*v2.y;
}

float vmagnitude(Vector2 v) {
    return sqrt(v.x*v.x + v.y*v.y);
}

Vector2 vnormalize(Vector2 v) {
    float magnitude = vmagnitude(v);

    if (magnitude == 0)
        return {0.f , 0.f};

    return Vector2(v.x / magnitude, v.y / magnitude);
}

Vector2 vmult(Vector2 v1, Vector2 v2) {
    return Vector2(v1.x * v2.x, v1.y * v2.y);
}

Vector2 vmult(Vector2 v, float multiplier) {
    return Vector2(v.x * multiplier, v.y * multiplier);
}

Vector2 vdiv(Vector2 v1, Vector2 v2) {
    return Vector2(v1.x / v2.x, v1.y / v2.y);
}
Vector2 vdiv(Vector2 v, float divisor) {
    return Vector2(v.x / divisor, v.y / divisor);
}

Vector2 vsub (Vector2 v1, Vector2 v2) {
    return Vector2(v1.x - v2.x, v1.y - v2.y);
}
Vector2 vadd (Vector2 v1, Vector2 v2) {
    return Vector2(v1.x + v2.x, v1.y + v2.y);
}
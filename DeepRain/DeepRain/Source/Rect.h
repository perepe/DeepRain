#pragma once

#include "V2.h"

struct Rect
{
    V2 min;
    V2 max;

    Rect() = default;
    Rect(V2 _min, V2 _max)
        : min(_min)
        , max(_max){};

    inline float width() const { return max.x - min.x; };
    inline float height() const { return max.y - min.y; };

    inline bool isInsideX(float x) { return x > min.x && x < max.x; }
    inline bool isInsideY(float y) { return y > min.y && y < max.y; }
    inline bool isInside(V2 p) { return isInsideX(p.x) && isInsideY(p.y); }
};

#pragma once

#include "V2.h"

struct Rect
{
    V2 _min;
    V2 _max;

    Rect(V2 min, V2 max)
        : _min(min)
        , _max(max){};

    inline float width() const { return _max.x - _min.x; };
    inline float height() const { return _max.y - _min.y; };

    inline bool isInsideX(float x) { return x > _min.x && x < _max.x; }
    inline bool isInsideY(float y) { return y > _min.y && y < _max.y; }
    inline bool isInside(V2 p) { return isInsideX(p.x) && isInsideY(p.y); }
};

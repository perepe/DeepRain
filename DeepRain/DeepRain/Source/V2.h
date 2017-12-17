#pragma once

#include <math.h>

struct c_V2
{
    int x, y;
};

struct V2
{
    float x, y;

    V2(){};
    V2(float _x, float _y)
        : x(_x)
        , y(_y){};
    V2(const V2& v)
        : x(v.x)
        , y(v.y){};
    V2(const c_V2& v)
        : x(v.x)
        , y(v.y){};

    inline void operator+=(const V2& v)
    {
        x += v.x;
        y += v.y;
    };
    inline void operator-=(const V2& v)
    {
        x -= v.x;
        y -= v.y;
    };
    inline void operator*=(const float m)
    {
        x *= m;
        y *= m;
    };
    inline void operator/=(const float d)
    {
        x /= d;
        y /= d;
    };

    inline bool isInside(float p) { return p >= x && p <= y; }
};

inline bool operator==(const V2& v1, const V2& v2) { return v1.x == v2.x && v1.y == v2.y; };
inline bool operator!=(const V2& v1, const V2& v2) { return v1.x != v2.x || v1.y != v2.y; };
inline V2 operator+(const V2& v1, const V2& v2) { return V2(v1.x + v2.x, v1.y + v2.y); };
inline V2 operator-(const V2& v1, const V2& v2) { return V2(v1.x - v2.x, v1.y - v2.y); };
inline V2 operator*(const V2& v, const float m) { return V2(v.x * m, v.y * m); };
inline V2 operator*(const float m, const V2& v) { return V2(v.x * m, v.y * m); };
inline V2 operator/(const V2& v, const float d) { return V2(v.x / d, v.y / d); };
inline float dot(const V2& v1, const V2& v2) { return v1.x * v2.x + v1.y * v2.y; };
inline float sqLen(const V2& v) { return v.x * v.x + v.y * v.y; };

inline V2 unit(const V2& v)
{
    float len = (float)sqrt(sqLen(v));
    return V2(v.x / len, v.y / len);
};

inline V2 unit(const V2& v, float& len)
{
    len = (float)sqrt(sqLen(v));
    return V2(v.x / len, v.y / len);
};

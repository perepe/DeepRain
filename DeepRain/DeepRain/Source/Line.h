#pragma once

#include "V2.h"

struct Line
{
    V2 _start;
    V2 _end;

    Line(){};
    Line(V2 start, V2 end)
        : _start(start)
        , _end(end){};
};

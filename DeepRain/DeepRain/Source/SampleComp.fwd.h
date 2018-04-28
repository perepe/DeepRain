#pragma once

#include "Component.h"

struct SampleComp
{
    static constexpr unsigned int maxSize = 1000;

    static constexpr int kNumPoints = 16;

    Handle streamH;
    unsigned char derivative = 0;
    float samples[kNumPoints];
};

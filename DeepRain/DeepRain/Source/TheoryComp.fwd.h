#pragma once

#include "Component.h"
#include "SampleComp.fwd.h"

struct TheoryComp
{
    static constexpr unsigned int maxSize = 1000;

    Handle _firstNodeH;
    Handle _targetStreamH;
    unsigned char _derivative = 0;
    unsigned char _level = 0;
    unsigned int _numTests = 0;
    float _avgTotalError = 0.0f;

    void update(unsigned int idxs[SampleComp::kNumPoints], int level);
};

#pragma once

#include "Component.h"
#include "SampleComp.fwd.h"

struct TheoryNodeComp
{
    static constexpr unsigned int maxSize = 1000;

    Handle _nextNodeH;
    Handle _theoryH;
    Handle _causeStreamH;
    unsigned char _derivative = 0;
    float _factor = 1.0f;
    float _avgError = 0.0f;

    float update(unsigned int idxs[SampleComp::kNumPoints], unsigned char level);
};

#pragma once

#include "Component.h"
#include "StreamComp.h"

class TheoryComp
{
    Handle _firstNodeH;
    Handle _targetStreamH;
    unsigned char _derivative = 0;
    unsigned int _numTests = 0;
    float _avgErrors[StreamComp::kNumLevels];

  public:
    static constexpr unsigned int maxSize = 1000;

    static constexpr int kNumSamples = 16;

    void update(float* stream);

    static Handle createTheory(float values[kNumSamples], int idxs[kNumSamples], int level);
};

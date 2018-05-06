#include "TheoryComp.h"

#include "Drawer.h"
#include "StreamComp.h"
#include "TheoryNodeComp.fwd.h"
#include <stdlib.h>

void TheoryComp::update(unsigned int idxs[9], int level)
{
    float error = 0.0f;

    TheoryNodeComp& firstNode = Drawer::getComponent<TheoryNodeComp>(_firstNodeH);

    const float totalError = firstNode.update(idxs, level);

    _avgTotalError *= _numTests;
    _avgTotalError += totalError;
    _avgTotalError /= ++_numTests;
}

namespace Theory
{
    void create(unsigned int idxs[9], unsigned char level) {}
}

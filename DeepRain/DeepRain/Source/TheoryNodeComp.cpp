#include "TheoryNodeComp.h"

#include "Drawer.h"
#include "StreamComp.h"
#include <stdlib.h>

float TheoryNodeComp::update(unsigned int idxs[SampleComp::kNumPoints], unsigned char level)
{
    float error = 0.0f;
    return error;
}

namespace TheoryNode
{
    float create(Handle& handle, unsigned int idxs[SampleComp::kNumPoints], unsigned char level)
    {
        Drawer::addComponent<TheoryNodeComp>(handle);
        TheoryNodeComp& theoryNode = Drawer::getComponent<TheoryNodeComp>(handle);

        float error = 0.0f;
        return error;
    }
}

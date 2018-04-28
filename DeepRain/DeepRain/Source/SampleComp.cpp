#include "SampleComp.h"

#include "StreamComp.h"

namespace Sample
{
    void updateSamples(Drawer drawer)
    {
        unsigned char level = 0; // rand() % StreamComp::kNumLevels;
        unsigned int idxs[SampleComp::kNumPoints];
        unsigned int levelPoints = StreamComp::getLevelPoints(level);
        unsigned int first = 0;

        for (int i = 0; i < SampleComp::kNumPoints; ++i)
        {
            idxs[i] = first + rand() % levelPoints;
        }

        for (int i = 0; i < Component<SampleComp>::aliveSize; ++i)
        {
            Handle streamH = drawer.getRandomHandle<StreamComp>();
            unsigned char derivative = rand() % StreamComp::kNumDerivatives;
        }
    }
}

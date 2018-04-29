#include "SampleComp.h"

#include "Drawer.h"
#include "StreamComp.h"

namespace Sample
{
    void updateSamples()
    {
        unsigned char level = rand() % StreamComp::kNumLevels;

        unsigned int idxs[SampleComp::kNumPoints];
        unsigned int levelStart = StreamComp::getLevelStartPoint(level);
        unsigned int levelPoints = StreamComp::getLevelPoints(level);
        unsigned int next = StreamComp::getLevelNextPointIdx(level, levelStart);
        unsigned int levelFilledPoints = StreamComp::getLevelFilledPoints(level);

        for (int i = 0; i < SampleComp::kNumPoints; ++i)
        {
            idxs[i] = levelStart + (next + (rand() % levelFilledPoints) % levelPoints);
        }

        for (int i = 0; i < Component<SampleComp>::aliveSize; ++i)
        {
            SampleComp& sampleComp = Component<SampleComp>::objects[i];

            sampleComp.streamH = Drawer::getRandomHandle<StreamComp>();
            sampleComp.derivative = rand() % StreamComp::kNumDerivatives;

            const StreamComp& stream = Drawer::getComponent<StreamComp>(sampleComp.streamH);

            for (int i = 0; i < SampleComp::kNumPoints; ++i)
            {
                sampleComp.samples[i] = stream._data[idxs[i]];
            }
        }
    }
}

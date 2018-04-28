#include "StreamComp.h"

#include "Allegro.h"
#include "Line.h"
#include <stdlib.h>

void StreamComp::add(float value)
{
    if (_timespan < kMaxTimespan)
    {
        ++_timespan;
    }

    unsigned int levelStartPoint = 0;

    for (unsigned char level = 0; level < kNumLevels; ++level)
    {
        const unsigned int levelPoints = getLevelPoints(level);
        const unsigned int pointTimespan = getPointTimespan(level);
        const unsigned int levelNextPointIdx = getLevelNextPointIdx(level, levelStartPoint);

        const float valueContributionToPoint = value / (float)pointTimespan;
        const bool isFirstContributionToPoint = _lastInv % pointTimespan == 0;
        _data[levelNextPointIdx] = isFirstContributionToPoint ? valueContributionToPoint : _data[levelNextPointIdx] + valueContributionToPoint;
        levelStartPoint += levelPoints;
    }

    _lastInv = _lastInv + 1 >= kMaxTimespan ? 0 : _lastInv + 1;
}

void StreamComp::draw(Allegro* allegro) const
{
    allegro->print(_renderArea.min + V2(50.0f, 20.0f), "%s:", _name);

    if (_timespan == 0)
    {
        return;
    }

    int start = 0;
    float kOffsetY = 50.0f;

    for (int level = 0; level < 1; ++level)
    {
        const unsigned int levelPoints = getLevelPoints(level);
        const unsigned int levelNextPointLevelIdx = getLevelNextPointLevelIdx(level);
        const unsigned int filledPoints = getLevelFilledPoints(level);

        const float kMinValue = 300.0f;
        const float kMaxValue = 600.0f;
        const float scaleX = _renderArea.width() / (levelPoints - 1);
        const float offsetY = level * kOffsetY;
        const float kScaleY = 0.5f * _renderArea.height() / (kMaxValue - kMinValue);

        Line line;
        line.end = V2(_renderArea.min.x + (filledPoints - 1) * scaleX, _renderArea.max.y + offsetY - (_data[start + levelNextPointLevelIdx] - kMinValue) * kScaleY);
        Allegro::drawFilledCircle(line.end);

        if (filledPoints == 1)
        {
            line.start = line.end;
            Allegro::drawLine(line);
            continue;
        }

        for (int j = 1; j < filledPoints; ++j)
        {
            line.start = line.end;
            line.end = V2(_renderArea.min.x + (filledPoints - 1 - j) * scaleX, _renderArea.max.y + offsetY - (_data[start + ((levelNextPointLevelIdx + j) % levelPoints)] - kMinValue) * kScaleY);
            Allegro::drawLine(line);
        }

        start += levelPoints;
    }
}
namespace Stream
{
}

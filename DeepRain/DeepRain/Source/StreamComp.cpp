#include "StreamComp.h"

#include "Allegro.h"
#include "Line.h"
#include <stdlib.h>

unsigned int StreamComp::_timespan = 0;
unsigned int StreamComp::_lastInv = 0;

void StreamComp::add()
{
    if (_timespan < kMaxTimespan)
    {
        ++_timespan;
    }

    _lastInv = _lastInv + 1 >= kMaxTimespan ? 0 : _lastInv + 1;
}

void StreamComp::add(float value)
{
    unsigned int levelStartPoint = 0;

    for (unsigned char level = 0; level < kNumLevels; ++level)
    {
        const unsigned int levelPoints = getLevelPoints(level);
        const unsigned int levelNextPointIdx = getLevelNextPointIdx(level, levelStartPoint);
        const unsigned int pointTimespan = getPointTimespan(level);
        const float valueContributionToPoint = value / (float)pointTimespan;
        const unsigned int contributionIdx = _lastInv % pointTimespan;
        const bool isFirstContributionToPoint = contributionIdx == 0;
        const bool isLastContributionToPoint = contributionIdx == pointTimespan - 1;

        _data[levelNextPointIdx] = isFirstContributionToPoint ? valueContributionToPoint : _data[levelNextPointIdx] + valueContributionToPoint;

        if (isLastContributionToPoint)
        {
            updateDerivative(levelNextPointIdx, level, levelStartPoint, levelPoints);
        }

        levelStartPoint += levelPoints;
    }
}

void StreamComp::updateDerivative(unsigned int idx, unsigned char level, unsigned int levelStart, unsigned int levelPoints)
{
    const unsigned int filledPoints = getLevelFilledPoints(level);

    if (filledPoints <= 1)
    {
        return;
    }

    const unsigned char numDerivatives = kNumLevels - level - 1;
    const unsigned int levelIdx = idx - levelStart;
    unsigned int levelDerivativeNextPointIdx = levelIdx >> 1;

    _derivatives[levelStart + levelDerivativeNextPointIdx] = _data[idx] - _data[levelStart + ((levelIdx + 1) % levelPoints)];

    unsigned int derivativeStart = 0;
    unsigned int derivativePoints = levelPoints >> 1;

    for (unsigned int derivativeIdx = 1; derivativeIdx < numDerivatives; ++derivativeIdx)
    {
        float derivativeValue = _derivatives[levelStart + derivativeStart + levelDerivativeNextPointIdx] - _derivatives[levelStart + derivativeStart + ((levelDerivativeNextPointIdx + 1) % derivativePoints)];

        derivativeStart += derivativePoints;
        derivativePoints >>= 1;

        levelDerivativeNextPointIdx = levelIdx >> (derivativeIdx + 1);

        _derivatives[levelStart + derivativeStart + levelDerivativeNextPointIdx] = derivativeValue;
    }
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

    drawDerivative(allegro);
}

void StreamComp::drawDerivative(Allegro* allegro) const
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
        const unsigned int levelPoints = getLevelPoints(level) >> 1;
        const unsigned int levelNextPointLevelIdx = getLevelNextPointLevelIdx(level) >> 1;
        const unsigned int filledPoints = getLevelFilledPoints(level) >> 1;

        const float kMinValue = -5.0f;
        const float kMaxValue = 5.0f;
        const float scaleX = _renderArea.width() / (levelPoints - 1);
        const float offsetY = level * kOffsetY + 200;
        const float kScaleY = 2.f * _renderArea.height() / (kMaxValue - kMinValue);

        Line line;
        line.end = V2(_renderArea.min.x + (filledPoints - 1) * scaleX, _renderArea.max.y + offsetY - (_derivatives[start + levelNextPointLevelIdx] - kMinValue) * kScaleY);
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
            line.end = V2(_renderArea.min.x + (filledPoints - 1 - j) * scaleX, _renderArea.max.y + offsetY - (_derivatives[start + ((levelNextPointLevelIdx + j) % levelPoints)] - kMinValue) * kScaleY);
            Allegro::drawLine(line);
        }

        start += levelPoints;
    }
}

namespace Stream
{
}

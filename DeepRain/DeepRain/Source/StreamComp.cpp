#include "StreamComp.h"

#include "Allegro.h"
#include "Line.h"
#include <stdio.h>
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
        const unsigned int pointTimespan = getLevelPointTimespan(level);
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

	const unsigned char numDerivatives = 2;// kNumLevels - level - 1;
	const unsigned int levelIdx = idx - levelStart;
	unsigned int levelDerivativeNextPointIdx = levelIdx >> 1;

	unsigned int curIdx = idx;
	unsigned int prevIdx = levelStart + ((levelIdx + 1) % levelPoints);
	float cur = _data[idx];
	float prev = _data[prevIdx];
	float val = cur - prev;

	_derivatives[levelStart + levelDerivativeNextPointIdx] = _data[idx] - _data[levelStart + ((levelIdx + 2) % levelPoints)];

	unsigned int derivativeStart = levelStart;
	unsigned int derivativePoints = levelPoints >> 1;

	for (unsigned int derivativeIdx = 1; derivativeIdx < numDerivatives; ++derivativeIdx)
	{
		unsigned int curIdxa = derivativeStart + levelDerivativeNextPointIdx;
		unsigned int prevIdxa = derivativeStart + ((levelDerivativeNextPointIdx + 2) % derivativePoints);
		float cura = _derivatives[curIdxa];
		float preva = _derivatives[prevIdxa];
		float vala = cura - preva;

		const unsigned int lt = getLevelPointTimespan(level) << (derivativeIdx + 1);
		const unsigned int m = _lastInv % lt;

		//const unsigned int contributionIdx = _lastInv % getDerivativePointTimespan(level, derivativeIdx);
		const unsigned int contributionIdx = (_lastInv % (getLevelPointTimespan(level) << (derivativeIdx + 1)));

		float derivativeValue = _derivatives[derivativeStart + levelDerivativeNextPointIdx] - _derivatives[derivativeStart + ((levelDerivativeNextPointIdx + 2) % derivativePoints)];

		if (strcmp(_name, "Ball X") == 0)
		{
			if (level == 1)
			{
				/*if (_derivatives[derivativeStart + levelDerivativeNextPointIdx] > 0.0f)
				{
				int a = 0;
				a++;
				}*/

				//printf("v1: %.0f v2: %.0f d1: %.0f d1: %.0f d2: %.0f d21: %.0f i: %d\n", cur, prev, val, cura, preva, vala, contributionIdx);
				printf("i: %d v: %.2f\n", derivativeStart + levelDerivativeNextPointIdx, derivativeValue);
			}
		}

		derivativeStart += derivativePoints;
		derivativePoints >>= 1;

		levelDerivativeNextPointIdx >>= 1; // = levelIdx >> (derivativeIdx + 1);

		_derivatives[derivativeStart + levelDerivativeNextPointIdx] = derivativeValue;
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

    for (unsigned int level = 0; level < 2; ++level)
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

        for (unsigned int j = 1; j < filledPoints; ++j)
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
    float kOffsetY = 60.0f;

    for (unsigned int level = 0; level < 2; ++level)
    {
		for (unsigned int derivative = 0; derivative < 2; derivative++)
		{
			const unsigned int derivativePoints = getDerivativePoints(level, derivative);
			const unsigned int derivativeNextPointDerivativeIdx = getDerivativeNextPointDerivativeIdx(level, derivative);
			const unsigned int filledPoints = getDerivativeFilledPoints(level, derivative);

			const float kMinValue = -80.0f;
			const float kMaxValue = 80.0f;
			const float scaleX = _renderArea.width() / (derivativePoints - 1);
			const float offsetY = (level*2 + derivative) * kOffsetY + 60;
			const float kScaleY = 0.2f*_renderArea.height() / (kMaxValue - kMinValue);

			Line line;
			line.end = V2(_renderArea.min.x + (filledPoints - 1) * scaleX, _renderArea.max.y + offsetY - (_derivatives[start + derivativeNextPointDerivativeIdx] - kMinValue) * kScaleY);
			Allegro::drawFilledCircle(line.end);

			if (filledPoints == 1)
			{
				line.start = line.end;
				Allegro::drawLine(line);
				continue;
			}

			for (unsigned int j = 1; j < filledPoints; ++j)
			{
				line.start = line.end;
				line.end = V2(_renderArea.min.x + (filledPoints - 1 - j) * scaleX, _renderArea.max.y + offsetY - (_derivatives[start + ((derivativeNextPointDerivativeIdx + j) % derivativePoints)] - kMinValue) * kScaleY);
				Allegro::drawLine(line);
			}

			start += derivativePoints;
		}
    }
}

namespace Stream
{
}

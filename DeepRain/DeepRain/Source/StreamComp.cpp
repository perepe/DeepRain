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
    _t += value;
    unsigned int levelStartPoint = 0;

    for (unsigned char level = 0; level < kNumLevels; ++level)
    {
        const unsigned int levelPoints = getLevelPoints(level);
        const unsigned int levelNextPointIdx = getLevelNextPointIdx(level, levelStartPoint);
        const unsigned int pointTimespan = getLevelPointTimespan(level);
        const float valueContributionToPoint = 100.0f*sin(_t) / (double)pointTimespan;
        const unsigned int contributionIdx = _lastInv % pointTimespan;
        const bool isFirstContributionToPoint = contributionIdx == 0;
        const bool isLastContributionToPoint = contributionIdx == pointTimespan - 1;

        _data[levelNextPointIdx] = isFirstContributionToPoint ? valueContributionToPoint : _data[levelNextPointIdx] + valueContributionToPoint;

        if (isLastContributionToPoint)
        {
            updateDerivative(levelNextPointIdx, level, levelStartPoint, levelPoints, value);
        }

        levelStartPoint += levelPoints;
    }
}

void fillDerivative(float data[StreamComp::kStreamDataSize], float derivative[StreamComp::kStreamDataSize])
{
	const int kSamplesAvg = 200;
	constexpr float scaleY = 325.949829f / kSamplesAvg;

	for (int i = 0; i < StreamComp::kStreamDataSize; ++i)
	{
		derivative[i] = 0;

		for (int j = 0; j < kSamplesAvg; ++j)
		{
			float factor = 2.0f * (kSamplesAvg - j) / kSamplesAvg - (1 / (float)kSamplesAvg);

			int idx1 = (StreamComp::kStreamDataSize + i) % StreamComp::kStreamDataSize;
			int idx2 = (StreamComp::kStreamDataSize + i - j - 1) % StreamComp::kStreamDataSize;
			float val1 = data[idx1];
			float val2 = data[idx2];
			derivative[i] += (val1 - val2)*scaleY *factor / (j + 1);
		}
	}
}

void StreamComp::updateDerivative(unsigned int idx, unsigned char level, unsigned int levelStart, unsigned int levelPoints, float dt)
{
    const unsigned int filledPoints = getLevelFilledPoints(level);

    if (filledPoints <= 1)
    {
        return;
    }

    const unsigned char numDerivatives = kNumLevels - level - 1;
    const unsigned int levelIdx = idx - levelStart;
    unsigned int levelDerivativeNextPointIdx = levelIdx >> 1;
	unsigned int timespan = getLevelPointTimespan(level);
	timespan <<= 1;
	unsigned int curIdx = idx;
	float cur = _data[idx];
	unsigned int prevIdx = levelStart + ((levelIdx + 1) % levelPoints);
	float prev = _data[prevIdx];
	float val = cur - prev;

	const unsigned int contributionIdx = _lastInv % timespan;
	const bool isLastContributionToPoint = contributionIdx == timespan - 1;

	if (!isLastContributionToPoint)
	{
		return;
	}

	_derivatives[levelStart + levelDerivativeNextPointIdx] = val/(float)(dt*timespan*0.5f);

    unsigned int derivativeStart = levelStart;
	unsigned int derivativePoints = levelPoints >> 1;

    for (unsigned int derivativeIdx = 1; derivativeIdx < numDerivatives; ++derivativeIdx)
    {
		timespan <<= 1;

		const unsigned int contributionIdx = _lastInv % timespan;
		const bool isLastContributionToPoint = contributionIdx == timespan - 1;

		if (!isLastContributionToPoint)
		{
			return;
		}

		unsigned int samplesAvg = derivativePoints >= 2 ? 1 : derivativePoints - 1;
		unsigned int curIdxa = derivativeStart + levelDerivativeNextPointIdx;
		float cura = _derivatives[curIdxa];
		unsigned int prevIdxa = derivativeStart + ((levelDerivativeNextPointIdx + 1) % derivativePoints);
		float preva = _derivatives[prevIdxa];
		float vala = cura - preva;

        derivativeStart += derivativePoints;
        derivativePoints >>= 1;

        levelDerivativeNextPointIdx >>= 1;

        _derivatives[derivativeStart + levelDerivativeNextPointIdx] = vala/(dt*timespan*0.5f);
    }
}

void StreamComp::draw(Allegro* allegro) const
{
	drawRawData(allegro);
	drawRawDerivative(allegro);
}

void StreamComp::drawRawData(Allegro* allegro) const
{
    const float deltaX = _renderArea.width() / (kStreamDataSize - 1);
    const float centerY = _renderArea.min.y + _renderArea.height() * 0.75f;
    const float scaleY = 0.5f;

    Line line;
    line.end.x = _renderArea.min.x;
    line.end.y = centerY + _data[0] * scaleY;

    for (int i = 0; i < kStreamDataSize - 1; ++i)
    {
        line.start = line.end;

        line.end.x += deltaX;
        line.end.y = centerY + _data[i + 1] * scaleY;

        Allegro::drawLine(line);
    }
}

void StreamComp::drawRawDerivative(Allegro* allegro) const
{
    const float deltaX = _renderArea.width() / (kStreamDataSize - 1);
    const float centerY = _renderArea.min.y + _renderArea.height() * 0.25f;
	const float scaleY = 0.5f;

    Line line;
    line.end.x = _renderArea.min.x;
    line.end.y = centerY + _derivatives[0] * scaleY;

    for (int i = 0; i < kStreamDataSize - 1; ++i)
    {
        line.start = line.end;

        line.end.x += deltaX;
        line.end.y = centerY + _derivatives[i + 1] * scaleY;

        Allegro::drawLine(line);
    }
}

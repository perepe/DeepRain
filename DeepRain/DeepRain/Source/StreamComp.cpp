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

	const unsigned int kMaxSamplesAvg = 1;
	unsigned int samplesAvg = filledPoints >= kMaxSamplesAvg + 1 ? kMaxSamplesAvg : filledPoints - 1;
	const float scaleY = 1.0f / samplesAvg;

    const unsigned char numDerivatives = kNumLevels - level - 1;
    const unsigned int levelIdx = idx - levelStart;
    unsigned int levelDerivativeNextPointIdx = levelIdx >> 1;
	unsigned int timespan = getLevelPointTimespan(level);
	timespan <<= 1;
	unsigned int curIdx = idx;
	float cur = _data[idx];
	float val = 0.0f;

	for (unsigned int i = 0; i < samplesAvg; ++i)
	{
		unsigned int prevIdx = levelStart + ((levelIdx + 1 + i) % levelPoints);
		float prev = _data[prevIdx];
		val += scaleY*(cur - prev) / (i + 1);
	}

    

	const unsigned int contributionIdx = _lastInv % timespan;
	const bool isLastContributionToPoint = contributionIdx == timespan - 1;

	if (!isLastContributionToPoint)
	{
		return;
	}

	_derivatives[levelStart + levelDerivativeNextPointIdx] = val/(float)(dt*timespan);

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

		unsigned int samplesAvg = derivativePoints >= kMaxSamplesAvg + 1 ? kMaxSamplesAvg : derivativePoints - 1;
		unsigned int curIdxa = derivativeStart + levelDerivativeNextPointIdx;
		float cura = _derivatives[curIdxa];
		float vala = 0.0f;

		for (unsigned int i = 0; i < samplesAvg; ++i)
		{
			unsigned int prevIdxa = derivativeStart + ((levelDerivativeNextPointIdx + 1 + i) % derivativePoints);
			float preva = _derivatives[prevIdxa];
			vala += scaleY*(cura - preva) / (float)((i + 1));
		}

        derivativeStart += derivativePoints;
        derivativePoints >>= 1;

        levelDerivativeNextPointIdx >>= 1; // = levelIdx >> (derivativeIdx + 1);

        _derivatives[derivativeStart + levelDerivativeNextPointIdx] = vala/(dt*timespan*0.5f);
    }
}

void StreamComp::draw(Allegro* allegro) const
{
	//drawRawData(allegro);
	drawRawDerivative(allegro);
    //drawSin(allegro);
    return;

    allegro->print(_renderArea.min + V2(50.0f, 20.0f), "%s:", _name);

    if (_timespan == 0)
    {
        return;
    }

    int start = 0;
    float kOffsetY = 50.0f;

    for (unsigned int level = 1; level < 2; ++level)
    {
        const unsigned int levelPoints = getLevelPoints(level);
        const unsigned int levelNextPointLevelIdx = getLevelNextPointLevelIdx(level);
        const unsigned int filledPoints = getLevelFilledPoints(level);

        const float kMinValue = -1.0f;
        const float kMaxValue = 1.0f;
        const float scaleX = _renderArea.width() / (levelPoints - 1);
        const float offsetY = level * kOffsetY;
        const float kScaleY = 0.5f * _renderArea.height() / (kMaxValue - kMinValue);

        // float y = _renderArea.max.y + offsetY - (_data[start + levelNextPointLevelIdx] - kMinValue) * kScaleY;
        //++y;
        //--y;

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

    // drawDerivative(allegro);
}

void StreamComp::drawRawData(Allegro* allegro) const
{
    const float deltaX = _renderArea.width() / (kStreamDataSize - 1);
    const float centerY = _renderArea.min.y + _renderArea.height() * 0.5f;
    const float scaleY = 100.0f;

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
    const float centerY = _renderArea.min.y + _renderArea.height() * 0.5f;
	const float scaleY = 1.0f;// 3000000.0f;

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

#define PI       3.14159265358979323846f

void drawArray(Allegro* allegro, Rect renderArea, float data[StreamComp::kStreamDataSize], float num)
{
	const float deltaX = renderArea.width() / (StreamComp::kStreamDataSize - 1);
	float centerY = renderArea.min.y + renderArea.height() * 0.5f + num*0.0f;

	Line line;
	line.end.x = renderArea.min.x;
	line.end.y = centerY + data[0];

	for (int i = 0; i < StreamComp::kStreamDataSize - 1; ++i)
	{
		line.start = line.end;

		line.end.x += deltaX;
		line.end.y = centerY + data[i + 1];

		Allegro::drawLine(line);
	}
}

void StreamComp::drawSin(Allegro* allegro) const
{
	const float deltaX = _renderArea.width() / (kStreamDataSize - 1);
	float centerY = _renderArea.min.y + _renderArea.height() * 0.5f;
	float scaleY = 325.949829f;

	float data1[kStreamDataSize];
	for (int i = 0; i < kStreamDataSize; ++i)
	{
		data1[i] = 100.0f*sin(i*(2 * PI / kStreamDataSize));
	}

	drawArray(allegro, _renderArea, data1, 0);

	float data2[kStreamDataSize];
	fillDerivative(data1, data2);
	drawArray(allegro, _renderArea, data2, 1);

	float data3[kStreamDataSize];
	fillDerivative(data2, data3);
	drawArray(allegro, _renderArea, data3, 2);

	float data4[kStreamDataSize];
	fillDerivative(data3, data4);
	drawArray(allegro, _renderArea, data4, 3);

	float data5[kStreamDataSize];
	fillDerivative(data4, data5);
	drawArray(allegro, _renderArea, data5, 4);

	/*float data6[kStreamDataSize];
	fillDerivative(data5, data6);
	drawArray(allegro, _renderArea, data6, 5);

	float data7[kStreamDataSize];
	fillDerivative(data6, data7);
	drawArray(allegro, _renderArea, data7, 6);

	float data8[kStreamDataSize];
	fillDerivative(data7, data8);
	drawArray(allegro, _renderArea, data8, 7);*/

}

void StreamComp::drawDerivative(Allegro* allegro) const
{
    allegro->print(_renderArea.min + V2(50.0f, 20.0f), "%s:", _name);

    if (_timespan == 0)
    {
        return;
    }

    int start = 0;
    float kOffsetY = 100.0f;

    for (unsigned int level = 1; level < 2; ++level)
    {
        for (unsigned int derivative = 0; derivative < 2; derivative++)
        {
            const unsigned int derivativePoints = getDerivativePoints(level, derivative);
            const unsigned int derivativeNextPointDerivativeIdx = getDerivativeNextPointDerivativeIdx(level, derivative);
            const unsigned int filledPoints = getDerivativeFilledPoints(level, derivative);

            const float kMinValue = -100.0f;
            const float kMaxValue = 100.0f;
            const float scaleX = _renderArea.width() / (derivativePoints - 1);
            const float offsetY = (level * 2 + derivative) * kOffsetY + 5100;
            const float kScaleY = 100.0f * _renderArea.height() / (kMaxValue - kMinValue);

            float y = _renderArea.max.y + offsetY - (_derivatives[start + derivativeNextPointDerivativeIdx] - kMinValue) * kScaleY;
            ++y;
            --y;

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

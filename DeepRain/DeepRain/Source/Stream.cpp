#include "Stream.h"

#include "Allegro.h"
#include "Line.h"

void Stream::add(float value)
{
    if (_size < kMaxValues)
    {
        ++_size;
    }

    if (_last == 3)
    {
        int a = 0;
    }

    int start = 0;

    for (int i = 0; i < kNumLevels; ++i)
    {
        int amount = kFirstLevelSize >> i;
        int size = kFirstLevelSize << i;
        int idx = (_last % size);
        int div = (i << 1);
        int numNums = 1 << div;
        int partialIdx = amount - 1 - (idx >> div);
        _data[start + partialIdx] = _last % numNums == 0 ? value / (float)numNums : _data[start + partialIdx] + value / (float)numNums;
        start += kFirstLevelSize >> i;
    }

    _last = _last + 1 >= kMaxValues ? 0 : _last + 1;
}

void Stream::draw(Allegro* allegro) const
{
    allegro->print(_renderArea.min + V2(50.0f, 20.0f), "%s:", _name);

    if (_size == 0)
    {
        return;
    }

    int start = 0;
    float kOffsetY = 50.0f;

    for (int i = 0; i < kNumLevels; ++i)
    {
        float kMinValue = 300.0f;
        float kMaxValue = 600.0f;
        int amount = kFirstLevelSize >> i;
        int size = kFirstLevelSize << i;
        int idx = ((_last - 1) % size);
        int div = (i << 1);
        int partialIdx = amount - 1 - (idx >> div);
        int filledSize = (_size >> div) < amount ? _size >> div : amount;
        int end = start + amount;
        float scaleX = _renderArea.width() / (amount - 1);
        float offsetY = i * kOffsetY;

        const float kScaleY = _renderArea.height() / (kMaxValue - kMinValue);

        Line line;
        line.end = V2(_renderArea.min.x + (filledSize - 1) * scaleX, _renderArea.max.y + offsetY - (_data[start + partialIdx] - kMinValue) * kScaleY);
        Allegro::drawFilledCircle(line.end);

        if (filledSize == 1)
        {
            line.start = line.end;
            Allegro::drawLine(line);
            continue;
        }

        for (int j = 1; j < filledSize; ++j)
        {
            line.start = line.end;
            line.end = V2(_renderArea.min.x + (filledSize - 1 - j) * scaleX,
                          _renderArea.max.y + offsetY - (_data[start + ((partialIdx + j) % amount)] - kMinValue) * kScaleY);
            Allegro::drawLine(line);
        }

        start += amount;
    }
}

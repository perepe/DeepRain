#include "Stream.h"

#include "Allegro.h"
#include "Line.h"

void Stream::add(float value)
{
    if (_size < kStreamMaxSize)
    {
        _data[_size] = value;
        ++_size;
        return;
    }

    memmove(&_data, &_data[1], sizeof(float) * kStreamMaxSize - 1);
    _data[_size - 1] = value;
}

void Stream::draw(Allegro* allegro) const
{
    allegro->print(_renderArea._min + V2(50.0f, 20.0f), "%s:", _name);

    if (_size == 0)
    {
        return;
    }

    float kMinValue = 99999999.0f;
    float kMaxValue = -99999999.0f;

    for (int i = 0; i < _size; ++i)
    {
        kMinValue = kMinValue < _data[i] ? kMinValue : _data[i];
        kMaxValue = kMaxValue > _data[i] ? kMaxValue : _data[i];
    }

    const float kScaleY = _renderArea.height() / (kMaxValue - kMinValue);

    Line line;
    line._end = V2(_renderArea._min.x + (_size - 1) * _scaleX, _renderArea._max.y - (_data[_size - 1] - kMinValue) * kScaleY);

    if (_size == 1)
    {
        line._start = line._end;
        Allegro::drawLine(line);
        return;
    }

    for (int i = _size - 1; i > 0; --i)
    {
        line._start = line._end;
        line._end = V2(_renderArea._min.x + (i - 1) * _scaleX, _renderArea._max.y - (_data[i - 1] - kMinValue) * kScaleY);
        Allegro::drawLine(line);
    }
}

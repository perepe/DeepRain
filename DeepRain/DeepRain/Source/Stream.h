#pragma once

#include "Rect.h"
#include <assert.h>
#include <string.h>

class Allegro;

class Stream
{
    static constexpr int kNumLevels = 13;
    static constexpr int kStreamDataSize = 1 << kNumLevels - 1;
    static constexpr int kFirstLevelSize = kStreamDataSize >> 1;
    static constexpr int kMaxValues = (kFirstLevelSize * kStreamDataSize) << 1;
    static constexpr int kNameMaxSize = 20;
    static constexpr int kLineWidth = 1;

    float _data[kStreamDataSize];
    int _size = 0;
    int _last = 0;
    char _name[kNameMaxSize] = "";
    const Rect _renderArea;
    const float _scaleX = 0.0f;

  public:
    Stream(const char name[kNameMaxSize], const Rect& renderArea)
        : _renderArea(renderArea)
        , _size(0)
        , _last(0)
        , _scaleX(_renderArea.width() / (kFirstLevelSize - 1))
    {
        strcpy_s(_name, name);
    };

    void add(float value);
    void draw(Allegro* allegro) const;
};

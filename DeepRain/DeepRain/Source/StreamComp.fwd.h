#pragma once

#include "Component.h"
#include "Rect.h"
#include <string.h>

class Allegro;

struct StreamComp
{
    static constexpr unsigned int maxSize = 1000;

    static constexpr unsigned char kNumLevels = 13;
    static constexpr unsigned int kStreamDataSize = 1 << (kNumLevels - 1);
    static constexpr unsigned int kFirstLevelSize = kStreamDataSize >> 1;
    static constexpr unsigned int kMaxTimespan = (kFirstLevelSize * kStreamDataSize) << 1;
    static constexpr unsigned char kNumDerivatives = 8;
    static constexpr unsigned char kNameMaxSize = 20;
    static constexpr unsigned char kLineWidth = 1;

    float _data[kStreamDataSize];
    unsigned int _timespan = 0;
    unsigned int _lastInv = 0;
    char _name[kNameMaxSize] = "";
    Rect _renderArea;
    float _scaleX = 0.0f;

    static constexpr inline unsigned int getLevelPoints(const unsigned char level) { return kFirstLevelSize >> level; };
    static constexpr inline unsigned int getLevelTimespan(const unsigned char level) { return kFirstLevelSize << level; };
    static constexpr inline unsigned int getLevelTimeFactor(const unsigned char level) { return level << 1; };
    static constexpr inline unsigned int getPointTimespan(const unsigned char level) { return 1 << getLevelTimeFactor(level); };
    static inline unsigned int getLevelStartPoint(const unsigned char level)
    {
        unsigned int startPoint = 0;
        for (unsigned char i = 0; i < level; startPoint += getLevelPoints(i++))
            ;
        return startPoint;
    };
    inline unsigned int getLevelLastTimeInv(const unsigned char level) const { return _lastInv % getLevelTimespan(level); };
    inline unsigned int getLevelLastPointInv(const unsigned char level) const { return getLevelLastTimeInv(level) >> getLevelTimeFactor(level); };
    inline unsigned int getLevelNextPointLevelIdx(const unsigned char level) const { return getLevelPoints(level) - 1 - getLevelLastPointInv(level); };
    inline unsigned int getLevelNextPointIdx(const unsigned char level, const unsigned int levelStartPoint) const { return levelStartPoint + getLevelNextPointLevelIdx(level); };
    inline unsigned int getLevelFilledPoints(const unsigned char level) const
    {
        const unsigned int levelPoints = getLevelPoints(level);
        const unsigned int levelTimeFactor = getLevelTimeFactor(level);
        return (_timespan >> levelTimeFactor) < levelPoints ? _timespan >> levelTimeFactor : levelPoints;
    };

    void init(const char name[kNameMaxSize], const Rect& renderArea)
    {
        _renderArea = renderArea;
        _timespan = 0;
        _lastInv = 0;
        _scaleX = _renderArea.width() / (kFirstLevelSize - 1);
        strcpy_s(_name, name);
    };

    void add(float value);
    void draw(Allegro* allegro) const;
};

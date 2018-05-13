#pragma once

#include "Component.h"
#include "Rect.h"
#include <string.h>

class Allegro;

struct StreamComp
{
    static constexpr unsigned int maxSize = 1000;

    static constexpr unsigned char kNumLevels = 8;
    static constexpr unsigned int kStreamDataSize = 1 << (kNumLevels - 1);
	static constexpr unsigned int kFirstLevelSize = kStreamDataSize >> 1;
	static constexpr unsigned int kFirstLevelFirstDerivativeSize = kFirstLevelSize >> 1;
    static constexpr unsigned int kMaxTimespan = (kFirstLevelSize * kStreamDataSize) << 1;
    static constexpr unsigned char kNameMaxSize = 20;
    static constexpr unsigned char kLineWidth = 1;

    float _data[kStreamDataSize];
    float _derivatives[kStreamDataSize];
    static unsigned int _timespan;
    static unsigned int _lastInv;
    char _name[kNameMaxSize] = "";
    Rect _renderArea;
    float _scaleX = 0.0f;

	static constexpr inline unsigned int getLevelPoints(const unsigned char level) { return kFirstLevelSize >> level; };
	static constexpr inline unsigned int getDerivativePoints(const unsigned char level, const unsigned char derivative) { return kFirstLevelFirstDerivativeSize >> level + derivative; };
	static constexpr inline unsigned int getLevelTimespan(const unsigned char level) { return kFirstLevelSize << level; };
	static constexpr inline unsigned int getLevelTimeFactor(const unsigned char level) { return level << 1; };
	static constexpr inline unsigned int getDerivativeTimeFactor(const unsigned char level, const unsigned char derivative) { return (level << 1) + derivative + 1; };
	static constexpr inline unsigned int getLevelPointTimespan(const unsigned char level) { return 1 << getLevelTimeFactor(level); };
	static constexpr inline unsigned int getDerivativePointTimespan(const unsigned char level, const char derivative) { return 1 << getDerivativeTimeFactor(level, derivative); };
    static inline unsigned int computeLevelStartPoint(const unsigned char level)
    {
        unsigned int startPoint = 0;
        for (unsigned char i = 0; i < level; startPoint += getLevelPoints(i++))
            ;
        return startPoint;
    };
	static inline unsigned int computeDerivativeStartPoint(const unsigned char level, const unsigned char derivative)
	{
		unsigned int startPoint = 0;
		for (unsigned char i = 0; i < level; startPoint += getLevelPoints(i++))
			;
		for (unsigned char i = 0; i < derivative; startPoint += getDerivativePoints(level, i++))
			;
		return startPoint;
	};
	static inline unsigned int getLevelLastTimeInv(const unsigned char level) { return _lastInv % getLevelTimespan(level); };
	static inline unsigned int getLevelLastPointInv(const unsigned char level) { return getLevelLastTimeInv(level) >> getLevelTimeFactor(level); };
	static inline unsigned int getDerivativeLastPointInv(const unsigned char level, const unsigned char derivative) { return getLevelLastTimeInv(level) >> getDerivativeTimeFactor(level, derivative); };
	static inline unsigned int getLevelNextPointLevelIdx(const unsigned char level) { return getLevelPoints(level) - 1 - getLevelLastPointInv(level); };
	static inline unsigned int getDerivativeNextPointDerivativeIdx(const unsigned char level, const unsigned char derivative) { return getDerivativePoints(level, derivative) - 1 - getDerivativeLastPointInv(level, derivative); };
	static inline unsigned int getLevelNextPointIdx(const unsigned char level, const unsigned int levelStartPoint) { return levelStartPoint + getLevelNextPointLevelIdx(level); };
	static inline unsigned int getDerivativeNextPointIdx(const unsigned char level, const unsigned char derivative, const unsigned int derivativeStartPoint) { return derivativeStartPoint + getDerivativeNextPointDerivativeIdx(level, derivative); };
    static inline unsigned int getLevelFilledPoints(const unsigned char level)
    {
        const unsigned int levelPoints = getLevelPoints(level);
        const unsigned int levelTimeFactor = getLevelTimeFactor(level);
        return (_timespan >> levelTimeFactor) < levelPoints ? _timespan >> levelTimeFactor : levelPoints;
    };
	static inline unsigned int getDerivativeFilledPoints(const unsigned char level, const char derivative)
	{
		const unsigned int derivativePoints = getDerivativePoints(level, derivative);
		const unsigned int derivativeTimeFactor = getDerivativeTimeFactor(level, derivative);
		return (_timespan >> derivativeTimeFactor) < derivativePoints ? _timespan >> derivativeTimeFactor : derivativePoints;
	};

    void init(const char name[kNameMaxSize], const Rect& renderArea)
    {
        _renderArea = renderArea;
        _scaleX = _renderArea.width() / (kFirstLevelSize - 1);
        strcpy_s(_name, name);
    };

    static void add();

    void add(float value);
    void updateDerivative(unsigned int idx, unsigned char level, unsigned int levelStart, unsigned int levelPoints);
    void draw(Allegro* allegro) const;
    void drawDerivative(Allegro* allegro) const;
};

#pragma once

#include "Component.h"
#include "Drawer.h"
#include "Rect.h"
#include "StreamComp.h"

class Allegro;

class Paddle
{
  public:
    enum class Action
    {
        Serve,
        Up,
        Down,
        Nothing
    };

    static constexpr float kWidth = 10;
    static constexpr float kHalfWidth = kWidth * 0.5f;
    static constexpr float kHeight = 100;
    static constexpr float kHalfHeight = kHeight * 0.5f;

    static constexpr float kSpeed = 200.0f;

  private:
    int _score = 0;
    V2 _pos;
    Handle _streamH;

    void move(bool up, const Rect& field, float dt);

  public:
    Paddle(const V2& pos)
        : _pos(pos)
    {
        bool ok = Drawer::addComponent<StreamComp>(_streamH);
        assert(ok);
        StreamComp& stream = Drawer::getComponent<StreamComp>(_streamH);
        stream.init("Paddle", Rect(V2(0.0f, 200.0f), V2(800.0f, 300.0f)));
    };

    void update(const Rect& field, Action action, float dt);
    void draw(Allegro* allegro, bool drawStream);
    void onPointScored(bool me);

    const V2& getPos() const { return _pos; };
    int getScore() const { return _score; };
};

#pragma once

#include "Component.h"
#include "Drawer.h"
#include "Paddle.h"
#include "Rect.h"
#include "StreamComp.h"

class Pong;

class Ball
{
    V2 _pos;
    V2 _vel;
    Pong* _pong = nullptr;
    Handle _streamXH;
    Handle _streamYH;

    void serve(bool left);
    void attachToPaddle(const V2& leftPaddlePos, const V2& rightPaddlePos, bool left);
    void move(const Rect& field, const V2& leftPaddlePos, const V2& rightPaddlePos, float dt);
    static V2 calculatePositionOverflow(const V2& pos, const Rect& field, const V2& leftPaddlePos, const V2& rightPaddlePos);
    void onOut(bool left);

  public:
    static constexpr float kWidth = 20;
    static constexpr float kHalfWidth = kWidth * 0.5f;

    static constexpr float kSpeedX = 200.0f;
    static constexpr float kSpeedY = 200.0f;

    Ball(const V2& pos, Pong* pong)
        : _pos(pos)
        , _vel(0.0f, 0.0f)
        , _pong(pong)
    {
        bool ok = Drawer::addComponent<StreamComp>(_streamXH);
        ok &= Drawer::addComponent<StreamComp>(_streamYH);
        assert(ok);
        Drawer::getComponent<StreamComp>(_streamXH).init("Ball X", Rect(V2(0.0f, 0.0f), V2(800.0f, 300.0f)));
        Drawer::getComponent<StreamComp>(_streamYH).init("Ball Y", Rect(V2(0.0f, 100.0f), V2(800.0f, 200.0f)));
    };

    void update(const Rect& field, const V2& leftPaddlePos, const V2& rightPaddlePos, Paddle::Action leftAction, Paddle::Action rightAction, float dt);
    void draw(Allegro* allegro);

    const V2& getPos() const { return _pos; };
    const V2& getVelocity() const { return _vel; };
};

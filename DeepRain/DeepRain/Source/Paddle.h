#pragma once

#include "Rect.h"

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

    void move(bool up, const Rect& field, float dt);

  public:
    Paddle(const V2& pos)
        : _pos(pos){};

    void update(const Rect& field, Action action, float dt);
    void draw();
    void onPointScored(bool me);

    const V2& getPos() const { return _pos; };
};

#include "Ball.h"

#include "Pong.h"
#include <allegro5/allegro_primitives.h>

void Ball::update(const Rect& field, const V2& leftPaddlePos, const V2& rightPaddlePos, Paddle::Action leftAction, Paddle::Action rightAction,
                  float dt)
{
    bool waitingForServe = _vel.x == 0.0f;

    if (waitingForServe)
    {
        bool ballInLeftSide = _pos.x < field.width() * 0.5f;
        bool leftServe = ballInLeftSide && leftAction == Paddle::Action::Serve;
        bool rightServe = !ballInLeftSide && rightAction == Paddle::Action::Serve;

        attachToPaddle(leftPaddlePos, rightPaddlePos, ballInLeftSide);

        if (!leftServe && !rightServe)
        {
            return;
        }

        serve(ballInLeftSide);
    }

    move(field, leftPaddlePos, rightPaddlePos, dt);

    bool leftOut = _pos.x < leftPaddlePos.x + Paddle::kHalfWidth + kHalfWidth;
    bool rightOut = _pos.x > rightPaddlePos.x - Paddle::kHalfWidth - kHalfWidth;

    if (leftOut || rightOut)
    {
        onOut(leftOut);
    }
}

void Ball::draw()
{
    al_draw_filled_rectangle(_pos.x - kHalfWidth, _pos.y - kHalfWidth, _pos.x + kHalfWidth, _pos.y + kHalfWidth, {0.8f, 0.2f, 0.2f, 1.0f});
}

void Ball::serve(bool left)
{
    _vel.x = kSpeedX * (left ? 1.0f : -1.0f);
    bool goUp = rand() % 2;
    _vel.y = kSpeedY * (goUp ? 1.0f : -1.0f);
}

void Ball::attachToPaddle(const V2& leftPaddlePos, const V2& rightPaddlePos, bool left)
{
    _pos = left ? leftPaddlePos : rightPaddlePos;
    _pos.x += (left ? 1.0f : -1.0f) * (Paddle::kHalfWidth + kHalfWidth);
}

void Ball::move(const Rect& field, const V2& leftPaddlePos, const V2& rightPaddlePos, float dt)
{
    V2 targetPos = _pos + _vel * dt;
    V2 overflow = calculatePositionOverflow(targetPos, field, leftPaddlePos, rightPaddlePos);

    _vel.x *= overflow.x != 0 ? -1.0f : 1.0f;
    _vel.y *= overflow.y != 0 ? -1.0f : 1.0f;
    _pos = targetPos - overflow;
}

V2 Ball::calculatePositionOverflow(const V2& pos, const Rect& field, const V2& leftPaddlePos, const V2& rightPaddlePos)
{
    float topOverflow = field._min.y + kHalfWidth - pos.y;

    if (topOverflow > 0)
    {
        V2 overflow(0.0f, -topOverflow);
        V2 correctedPos(pos.x, pos.y + topOverflow);
        return overflow + calculatePositionOverflow(correctedPos, field, leftPaddlePos, rightPaddlePos);
    }

    float bottomOverflow = pos.y + kHalfWidth - field._max.y;

    if (bottomOverflow > 0)
    {
        V2 overflow(0.0f, bottomOverflow);
        V2 correctedPos(pos.x, pos.y - bottomOverflow);
        return overflow + calculatePositionOverflow(correctedPos, field, leftPaddlePos, rightPaddlePos);
    }

    float leftOverflow = leftPaddlePos.x + Paddle::kHalfWidth + kHalfWidth - pos.x;

    if (leftOverflow > 0)
    {
        V2 leftPaddleRange = V2(leftPaddlePos.y - Paddle::kHalfHeight - kHalfWidth, leftPaddlePos.y + Paddle::kHalfHeight + kHalfWidth);

        if (leftPaddleRange.isInside(pos.y))
        {
            V2 overflow(-leftOverflow, 0.0f);
            V2 correctedPos(pos.x + leftOverflow, pos.y);
            return overflow + calculatePositionOverflow(correctedPos, field, leftPaddlePos, rightPaddlePos);
        }
    }

    float rightOverflow = pos.x + Paddle::kHalfWidth + kHalfWidth - rightPaddlePos.x;

    if (rightOverflow > 0)
    {
        V2 rightPaddleRange = V2(rightPaddlePos.y - Paddle::kHalfHeight - kHalfWidth, rightPaddlePos.y + Paddle::kHalfHeight + kHalfWidth);

        if (rightPaddleRange.isInside(pos.y))
        {
            V2 overflow(rightOverflow, 0.0f);
            V2 correctedPos(pos.x - rightOverflow, pos.y);
            return overflow + calculatePositionOverflow(correctedPos, field, leftPaddlePos, rightPaddlePos);
        }
    }

    return V2(0.0f, 0.0f);
}

void Ball::onOut(bool left)
{
    _pong->onOut(left);

    _vel.x = 0.0f;
    _vel.y = 0.0f;
}

#pragma once

#include "Paddle.h"
#include "Rect.h"

namespace AI
{
    Paddle::Action selectMove(const Rect& field, const V2& ballPos, const V2& ballVelocity, const V2& paddlePos);
};

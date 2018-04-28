#include "AI.h"

#include <stdlib.h>

namespace AI
{
    Paddle::Action selectMove(const Rect& field, const V2& ballPos, const V2& ballVelocity, const V2& paddlePos)
    {
        V2 paddleToBall = ballPos - paddlePos;
        bool serve = ballVelocity.x == 0 && fabs(paddleToBall.x) < field.width() * 0.5f;

        if (serve)
        {
            return Paddle::Action::Serve;
        }

        if (paddleToBall.y < 0)
        {
            return Paddle::Action::Up;
        }
        else if (paddleToBall.y > 0)
        {
            return Paddle::Action::Down;
        }

        return Paddle::Action::Nothing;
    }

    Paddle::Action randomMove() { return (Paddle::Action)(rand() % 4); }
} // namespace AI

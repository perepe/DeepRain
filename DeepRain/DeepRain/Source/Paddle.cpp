#include "Paddle.h"

#include <allegro5/allegro_primitives.h>

void Paddle::move(bool up, const Rect& field, float dt)
{
    float limit = up ? field._min.y : field._max.y;
    float distToLimit = up ? _pos.y - limit - kHalfHeight : limit - _pos.y - kHalfHeight;
    float direction = up ? -1.0f : 1.0f;

    float moveDist = kSpeed * dt;
    _pos.y += direction * (distToLimit > moveDist ? moveDist : distToLimit);
}

void Paddle::update(const Rect& field, Action action, float dt)
{
    switch (action)
    {
        case Action::Serve:
            break;

        case Action::Up:
            move(true, field, dt);
            break;

        case Action::Down:
            move(false, field, dt);
            break;

        case Action::Nothing:
            break;
    }
}

void Paddle::draw()
{
    al_draw_filled_rectangle(_pos.x - kHalfWidth, _pos.y - kHalfHeight, _pos.x + kHalfWidth, _pos.y + kHalfHeight, {0.2f, 0.2f, 0.8f, 1.0f});
}

void Paddle::onPointScored(bool me)
{
    if (me)
    {
        ++_score;
    }
}

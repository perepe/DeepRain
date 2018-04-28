#include "Paddle.h"

#include "Allegro.h"

void Paddle::move(bool up, const Rect& field, float dt)
{
    float limit = up ? field.min.y : field.max.y;
    float distToLimit = up ? _pos.y - limit - kHalfHeight : limit - _pos.y - kHalfHeight;
    float direction = up ? -1.0f : 1.0f;

    float moveDist = kSpeed * dt;
    _pos.y += direction * (distToLimit > moveDist ? moveDist : distToLimit);
    Drawer::getComponent<StreamComp>(_streamH).add(_pos.y);
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

void Paddle::draw(Allegro* allegro, bool drawStream)
{
    Allegro::drawFilledRect(Rect(V2(_pos.x - kHalfWidth, _pos.y - kHalfHeight), V2(_pos.x + kHalfWidth, _pos.y + kHalfHeight)), Allegro::kBlue);

    if (drawStream)
        Drawer::getComponent<StreamComp>(_streamH).draw(allegro);
}

void Paddle::onPointScored(bool me)
{
    if (me)
    {
        ++_score;
    }
}

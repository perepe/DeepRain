#include "Pong.h"

#include "AI.h"
#include "Allegro.h"

Pong::Pong(Allegro* allegro, const Rect& field)
    : _allegro(allegro)
    , _field(field)
    , _ai(field._min + V2(Paddle::kHalfWidth, field.height() * 0.5f))
    , _dr(field._max - V2(Paddle::kHalfWidth, field.height() * 0.5f))
    , _ball(field._min + V2(Paddle::kWidth + Ball::kHalfWidth, field.height() * 0.5f), this){};

void Pong::update(float dt)
{
    Paddle::Action aiAction = AI::selectMove(_field, _ball.getPos(), _ball.getVelocity(), _ai.getPos());
    Paddle::Action drAction = AI::selectMove(_field, _ball.getPos(), _ball.getVelocity(), _dr.getPos());

    _ai.update(_field, aiAction, dt);
    _dr.update(_field, drAction, dt);
    _ball.update(_field, _ai.getPos(), _dr.getPos(), aiAction, drAction, dt);
}

void Pong::draw()
{
    _ai.draw();
    _dr.draw();
    _ball.draw();

    _allegro->print(V2(_field.width() * 0.5f, 20.0f), "hi");
}

void Pong::onOut(bool left)
{
    _ai.onPointScored(left);
    _dr.onPointScored(!left);
}

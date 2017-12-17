#pragma once

#include "Ball.h"
#include "Paddle.h"
#include "Rect.h"

class Allegro;

class Pong
{
    Rect _field;
    Paddle _ai;
    Paddle _dr;
    Ball _ball;

  public:
    Pong::Pong(const Rect& field);

    void update(float dt);
    void draw(Allegro* allegro);
    void onOut(bool left);
};

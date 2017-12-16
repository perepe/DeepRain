#pragma once

#include "Ball.h"
#include "Paddle.h"
#include "Rect.h"

class Allegro;

class Pong
{
    Allegro* _allegro = nullptr;
    Rect _field;
    Paddle _ai;
    Paddle _dr;
    Ball _ball;

  public:
    Pong::Pong(Allegro* allegro, const Rect& field);

    void update(float dt);
    void draw();
    void onOut(bool left);
};

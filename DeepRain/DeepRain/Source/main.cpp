#include "Allegro.h"
#include "Pong.h"
#include <stdio.h>
#include <time.h>
#include <windows.h>

int main(int argc, char** argv)
{
    V2 screenSize(640.0f, 480.0f);
    Allegro allegro;
    bool exit = !allegro.init(screenSize);

    Rect field(V2(0.0f, 0.0f), screenSize);
    Pong pong(&allegro, field);

    clock_t lastT, t = 0.0, startT = clock();

    while (!exit)
    {
        lastT = t;
        t = clock() - startT;
        float dt = (float)(t - lastT) / CLOCKS_PER_SEC;
        // printf("fps: %.3f, t: %.3f dt: %.3f\n", 1.0f / dt, (float)t, dt);

        exit = allegro.handleEvents();
        pong.update(dt);

        allegro.startFrame();
        pong.draw();
        allegro.endFrame();
    }

    allegro.destroy();

    return 0;
}

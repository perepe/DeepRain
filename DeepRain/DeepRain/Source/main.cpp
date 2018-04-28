#include "Allegro.h"
#include "Pong.h"
#include <stdio.h>
#include <time.h>
#include <windows.h>

int main(int argc, char** argv)
{
    constexpr c_V2 screenSize{800.0f, 600.0f};
    constexpr c_V2 fieldCenter{screenSize.x * 0.5f, screenSize.y * 0.75f};
    constexpr c_V2 fieldSize{400.0f, 300.0f};

    Allegro allegro;
    bool exit = !allegro.init(screenSize);

    constexpr c_V2 fieldHalfSize{fieldSize.x * 0.5f, fieldSize.y * 0.5f};
    constexpr c_V2 fieldMin{fieldCenter.x - fieldHalfSize.x, fieldCenter.y - fieldHalfSize.y};
    constexpr c_V2 fieldMax{fieldCenter.x + fieldHalfSize.x, fieldCenter.y + fieldHalfSize.y};

    Rect field(fieldMin, fieldMax);
    Pong pong(field);

    clock_t lastT, t = 0.0, startT = clock();

    while (!exit)
    {
        lastT = t;
        t = clock() - startT;
        float dt = (float)(t - lastT) / CLOCKS_PER_SEC;
        printf("fps: %.3f, t: %.3f dt: %.3f\n", 1.0f / dt, (float)t, dt);

        exit = allegro.handleEvents();
        pong.update(dt);

        // Sleep(128);

        Allegro::startFrame();
        pong.draw(&allegro);
        Allegro::endFrame();
    }

    allegro.destroy();

    return 0;
}

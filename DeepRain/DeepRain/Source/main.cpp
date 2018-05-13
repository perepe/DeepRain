#include "Allegro.h"
#include "Pong.h"
#include "StreamComp.fwd.h"
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
		clock_t dt = t - lastT;
        //float dt = (float)(t - lastT) / CLOCKS_PER_SEC;
		constexpr clock_t kRate = CLOCKS_PER_SEC / 4;
		clock_t remaining = kRate - dt;
		//float remaining = kFrameRate - dt;
		//assert(remaining >= 0.0f);


		if (remaining > 0.0f)
		{
			Sleep(remaining);
		}
		else
		{
			//printf("extra: %.3f---------------------------------------------------\n", remaining / (float)CLOCKS_PER_SEC);
			//assert(false);
		}

		t = clock() - startT;

        //printf("fps: %.3f, t: %.0f dt: %.3f extra: %.3f\n", 1.0f / ((dt) / (float)CLOCKS_PER_SEC), (float)t, (t - lastT) / (float)CLOCKS_PER_SEC, remaining / (float)CLOCKS_PER_SEC);

        exit = allegro.handleEvents();
        StreamComp::add();
        // pong.update(dt);
        pong.update(kRate / (float)CLOCKS_PER_SEC);

        Allegro::startFrame();
        pong.draw(&allegro);
        Allegro::endFrame();
    }

    allegro.destroy();

    return 0;
}

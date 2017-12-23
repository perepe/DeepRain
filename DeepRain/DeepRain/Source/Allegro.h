#pragma once

#include "Line.h"
#include "Rect.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

class Allegro
{
    ALLEGRO_DISPLAY* _display = nullptr;
    ALLEGRO_EVENT_QUEUE* _eventQueue = nullptr;
    ALLEGRO_FONT* _font = nullptr;

  public:
    static const ALLEGRO_COLOR kBlack;
    static const ALLEGRO_COLOR kWhite;
    static const ALLEGRO_COLOR kRed;
    static const ALLEGRO_COLOR kBlue;

    bool init(const V2& screenSize);
    void destroy();
    bool handleEvents();

    inline static void startFrame() { al_clear_to_color(kBlack); };
    inline static void endFrame() { al_flip_display(); };

    void print(const V2& pos, const char* format, ...) const;

    inline static void drawRect(const Rect& rect, const ALLEGRO_COLOR& color = kWhite)
    {
        al_draw_rectangle(rect.min.x, rect.min.y, rect.max.x, rect.max.y, color, 1);
    };

    inline static void drawFilledRect(const Rect& rect, const ALLEGRO_COLOR& color = kWhite)
    {
        al_draw_filled_rectangle(rect.min.x, rect.min.y, rect.max.x, rect.max.y, color);
    };

    inline static void drawFilledCircle(const V2& center, float radious = 2.0f, const ALLEGRO_COLOR& color = kWhite)
    {
        al_draw_filled_circle(center.x, center.y, radious, color);
    };

    inline static void drawLine(const Line& line, const ALLEGRO_COLOR& color = kWhite, int thickness = 1)
    {
        al_draw_line(line.start.x, line.start.y, line.end.x, line.end.y, color, thickness);
    };
};

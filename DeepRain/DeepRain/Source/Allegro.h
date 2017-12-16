#pragma once

#include "V2.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

class Allegro
{
    ALLEGRO_DISPLAY* _display = nullptr;
    ALLEGRO_EVENT_QUEUE* _eventQueue = nullptr;
    ALLEGRO_FONT* _font = nullptr;

  public:
    bool init(const V2& screenSize);
    void destroy();
    bool handleEvents();

    inline void startFrame() { al_clear_to_color(al_map_rgb(0, 0, 0)); };
    inline void endFrame() { al_flip_display(); };

    void print(const V2& pos, const char* format, ...) const;
};

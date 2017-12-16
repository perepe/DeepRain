#include "Allegro.h"

#include <allegro5/allegro_primitives.h>
#include <stdio.h>

bool Allegro::init(const V2& screenSize)
{
    bool ok = al_init();

    _display = al_create_display(screenSize.x, screenSize.y);
    _eventQueue = al_create_event_queue();

    ok &= _display != nullptr;
    ok &= _eventQueue != nullptr;
    ok &= al_init_font_addon();
    ok &= al_init_primitives_addon();

    al_register_event_source(_eventQueue, al_get_display_event_source(_display));
    _font = al_create_builtin_font();

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    return ok;
}

void Allegro::destroy()
{
    al_destroy_display(_display);
    al_destroy_event_queue(_eventQueue);
}

bool Allegro::handleEvents()
{
    ALLEGRO_EVENT ev;
    bool exit = false;

    while (al_get_next_event(_eventQueue, &ev))
    {
        switch (ev.type)
        {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                exit = true;
                break;

            default:
                break;
        }
    }

    return exit;
}

void Allegro::print(const V2& pos, const char* format, ...) const
{
    ALLEGRO_USTR* buf;
    va_list ap;
    const char* s;
    assert(_font);
    assert(format);

    if (0 == strcmp(format, "%s"))
    {
        va_start(ap, format);
        s = va_arg(ap, const char*);
        al_draw_text(_font, al_map_rgb(255, 255, 255), pos.x, pos.y, ALLEGRO_ALIGN_CENTER, s);
        va_end(ap);
        return;
    }

    va_start(ap, format);
    buf = al_ustr_new("");
    al_ustr_vappendf(buf, format, ap);
    va_end(ap);

    al_draw_text(_font, al_map_rgb(255, 255, 255), pos.x, pos.y, ALLEGRO_ALIGN_CENTER, al_cstr(buf));

    al_ustr_free(buf);
}

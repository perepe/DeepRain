#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

int main(int argc, char **argv)
{

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;

    if (!al_init())
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    display = al_create_display(640, 480);
    if (!display)
    {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue)
    {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        return -1;
    }

    al_init_primitives_addon();

    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_flip_display();

    bool exit = false;
    clock_t lastT, t = 0.0, startT = clock();

    while (!exit)
    {
        ALLEGRO_EVENT ev;

        while (al_get_next_event(event_queue, &ev))
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

        al_clear_to_color(al_map_rgb(0, 0, 0));

        for (int i = 1000; --i;)
        {
            ALLEGRO_COLOR red_color = al_map_rgb(184, 22, 22);

            al_draw_filled_rectangle(3.0, 4.0, 17.0, 16.0, red_color);
        }

        lastT = t;
        t = clock() - startT;

        float mSecs = (float)(t - lastT);

        printf("fps: %.3f, t: %.3f dt: %.3f\n", 1.0f / (mSecs / CLOCKS_PER_SEC), (float)t, mSecs);

        al_flip_display();
    }

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}

#define SFML
#include "graphic_library.cpp"

#include "cnavas.cpp"


double my_width  = 1920;
double my_height = 1080;

int main ()
{
    create_window ({my_width, my_height});
    Background back ({127, 35, 85}, {my_width, my_height});
    // Window_with_scrollbar x ("my_love.bmp", 1000, 700, 2560, 1820, 100, 100 , {700, 400}, {200, 200});

    Image img ("my_love.bmp", {500, 500});
    // x.set_color (RED);

    View_port::Settings set1 = {
            .button_size  = {16, 16},
            .left_button  = {0, my_height - 16},
            .right_button = {my_width - 16, my_height - 16},
            .color        = GRAY,
            .scrl_settings = {
                    .scroller_size = {20, 15},
                    .scroller_color = RED
            }
    };

    View_port::Settings set2 = {
            .button_size  = {16, 16},
            .left_button  = {my_width - 16, 0},
            .right_button = {my_width - 16, my_height - 16},
            .color        = GRAY,
            .scrl_settings = {
                    .scroller_size = {15, 20},
                    .scroller_color = RED
            }
    };

    View_port::Settings set[2] = {set2, set1};
    View_port port (&img, set);

    Window_manager try_ ({&back, &port}, true);
    while (true)
    {
        if (Window_manager::CLOSE == try_.manage_windows ()) {
            break;
        }
        try_.render ();
        img.draw ();
        render_window ();
    }
}
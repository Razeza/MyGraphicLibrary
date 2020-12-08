#define SFML
#include "graphic_library.cpp"

#include "cnavas.cpp"


double my_width  = GetSystemMetrics(SM_CXSCREEN);
double my_height = GetSystemMetrics(SM_CYSCREEN);

int main ()
{
    create_window (my_width, my_height);
    Background back ({108, 122, 137, 255}, my_width, my_height);

    int canvas_width = 1200,
        canvas_height = 700;
    Palitra::Palitra_settings settings = {
            .line_thickness = 5,
            .line_color = GRAY,
            .square_coordinates = {25, 25},
            .square_size = {370 + 360, 370 + 360},
            .line_coordinates = {25, 760},
            .line_size = {370 + 360, 60}
    };

    Paint x ((my_width - canvas_width)/2, (my_height - canvas_height)/2, canvas_width, canvas_height, settings);
    // Paint x (my_width, my_height, "back.bmp", settings);


    View_port::Settings set1 = {
            .button_size  = {16, 16},
            .left_button  = {my_width - 16, 0},
            .right_button = {my_width - 16, my_height - 16},
            .color        = {100, 100, 100},
            .scrl_settings = {
                    .scroller_size = {15, 500},
                    .scroller_color = {120, 120, 120}
            }
    };

    View_port::Settings set2 = {
            .button_size  = {16, 16},
            .left_button  = {0, my_height - 16},
            .right_button = {my_width - 16, my_height - 16},
            .color        = {100, 100, 100},
            .scrl_settings = {
                    .scroller_size = {800, 15},
                    .scroller_color = {120, 120, 120}
            }
    };




    View_port::Settings set[] = {set1, set2};
    View_port view (&(x.get_canvas ()), set);
    Window_manager try_ ({&back, &x, &view}, true);

    while (true)
    {
        if (Window_manager::CLOSE == try_.manage_windows ()) {
            break;
        }
        try_.render ();

        render_window ();
    }

}


#include "app.cpp"

double my_width  = 1920;
double my_height = 1000;


int main ()
{

    Background back ({108, 122, 137, 255}, {my_width, my_height});


    Palitra::Palitra_settings settings = {
            .line_thickness = 5,
            .line_color = GRAY,
            .square_coordinates = {25, 25},
            .square_size = {370 + 360, 370 + 360},
            .line_coordinates = {25, 760},
            .line_size = {370 + 360, 60}
    };

    Painter x ({my_width, my_height}, "graphic/back.bmp", settings);
    x.load_plugins({"ColorFilter"});

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
    Window_manager root_window ({&back, &x, &view}, true);

    App::init({my_width, my_height});
    App::set_root_window(&root_window);
    App::run();
}


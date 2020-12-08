#define SFML

#include <iostream>
#include "graphic_library.cpp"

#include "input_box.cpp"


double my_width  = GetSystemMetrics(SM_CXSCREEN);
double my_height = GetSystemMetrics(SM_CYSCREEN);

int main ()
{
    create_window (my_width, my_height);
    Background back ({108, 122, 137, 255}, my_width, my_height);


    Input_box::Settings settings = {
            .start = {200, 200},
            .size = {500, 150},
            .fill_color = WHITE,
            .line_color = GREEN,
            .thickness = 20,
            .letters_box_size = {300, 50},
            .letter_box_thickness = 3
    };

    std::cout << Input_box::make_input (settings, "hh");
    // Input_box x (settings, "Enter file name:", answer);


//    Window_manager try_ ({&back, &x}, true);
//
//    while (true)
//    {
//        if (Window_manager::CLOSE == try_.manage_windows ()) {
//            break;
//        }
//        try_.render ();
//        render_window ();
//    }

}


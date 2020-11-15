#include <iostream>
#include <windows.h>



double my_width  = GetSystemMetrics(SM_CXSCREEN);
double my_height = GetSystemMetrics(SM_CYSCREEN);

#include "basics.cpp"
#include "tx_define.cpp"
#include "graphic_library.cpp"


int main ()
{


    create_window (my_width, my_height);
    Background back ({127, 35, 85}, my_width, my_height);
    Button<Exit_functor> exit_button (Exit_functor (), {127, 0, 0}, 200, 100, 1200, 700, {0,0,0}, 5);
    Window_with_scrollbar x ("my_love.bmp", 1000, 700, 2560, 1820, 100, 100, 0, 10, 30, 16,"scroll-bar-arrow-up.bmp", "scroll-bar-arrow-down.bmp", 2,
                             "scroll-bar-arrow-left.bmp", "scroll-bar-arrow-right.bmp", {120, 120, 120}, 15, 20, {100, 100, 100});

    Window_manager try_ ({&back, &exit_button, &x}, true);
    while (true)
    {
        try_.manage_windows ();
        try_.render ();
        render_window ();
    }


}
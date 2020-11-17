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
    Window_with_scrollbar x ("my_love.bmp", 1000, 700, 2560, 1820, 100, 100);

    Window_manager try_ ({&back, &x}, true);
    while (true)
    {
        try_.manage_windows ();
        try_.render ();
        render_window ();
    }


}
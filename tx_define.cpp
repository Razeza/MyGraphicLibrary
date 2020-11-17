#ifndef TX_DEFINE
    #define TX_DEFINE



#include "tx_define.hpp"
#include <winuser.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Engine   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


LRESULT CALLBACK My_window_proc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto coordinates = get_mouse_coordinates ();
    int return_value = 0;
    switch (uMsg)
    {
        case WM_LBUTTONUP:
        {
            add_event (new Mouse_button_event (coordinates, Mouse_button_event::LEFT_BUTTON, RELEASED));
            break;
        }
        case WM_RBUTTONUP:
        {
            add_event (new Mouse_button_event (coordinates, Mouse_button_event::RIGHT_BUTTON, RELEASED));
            break;
        }
        case WM_KEYUP:
        {
            switch (wParam)
            {
                case PAGE_UP:
                {
                    add_event (new Keybord_event (PAGE_UP, RELEASED));
                    break;
                }
                case PAGE_DOWN:
                {
                    add_event (new Keybord_event (PAGE_DOWN, RELEASED));
                    break;
                }
            }
            break;
        }
        case WM_CLOSE:
        {
            add_event (new Program_close);
            return_value = 1;
            break;
        }
    }

    return return_value;
}

Engine::Engine ():
    images           (0),
    windows          (0),
    cur_font         (NULL),
    name_of_cur_font ()
{
    txSetWindowsHook (My_window_proc);
    txBegin ();
}



Engine::~Engine () {

    for (auto& i : windows) {
        txDestroyWindow (i);
    }

    for (auto& i : images) {
        txDeleteDC (i);
    }

    txDisableAutoPause ();
}


bool Engine::load_and_set_font (double size_y, double size_x)
{
    cur_font = txSelectFont (name_of_cur_font.c_str (), size_y, size_x);
    return check_font ();
}

bool Engine::check_font ()
{
    if (cur_font == NULL)
    {
        name_of_cur_font.clear ();
        return false;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Event Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




const keys check[] = {PAGE_UP, PAGE_DOWN, ARROW_UP, ARROW_DOWN, ARROW_RIGHT, ARROW_LEFT};


void store_events ()
{
    auto mouse_event = is_clicked ();
    if (mouse_event != Mouse_button_event::NOTHING)
    {
        auto coordinates = get_mouse_coordinates ();
        add_event (new Mouse_button_event (coordinates, mouse_event, PRESSED));
    }


    for (const auto& i : check)
    {
        if (txGetAsyncKeyState (i))
        {
            add_event (new Keybord_event (i));
        }
    }
}

void add_event (Event* new_event)
{
    xxx.queue_of_events.push (new_event);
}

Event* get_event ()
{
    auto return_value = xxx.queue_of_events.front ();
    xxx.queue_of_events.pop ();
    return return_value;
}

bool empty_queue ()
{
    return xxx.queue_of_events.empty();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Window Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void create_window (double size_x, double size_y)
{
    txTextCursor (false);
    xxx.windows.push_back (txCreateWindow (size_x, size_y));
}

void render_window ()
{
    txUpdateWindow ();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Shape Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_line_color (Color color, int line_thickness)
{
    if (color.red != -2)
    {
        txSetColor (RGB (color.red, color.green, color.blue), line_thickness);
    }
    else
    {
        txSetColor (TX_TRANSPARENT, line_thickness);
    }
}

void set_fill_color (Color color)
{
    if (color.red != -2)
    {
        txSetFillColor (RGB (color.red, color.green, color.blue));
    }
    else
    {
        txSetFillColor (TX_TRANSPARENT);
    }

}

void set_line_and_fill_color (Color color, int line_thickness)
{
    set_fill_color (color);
    set_line_color (color, line_thickness);
}


void draw_pixel (double x0, double y0, Color color)
{
    txSetPixel (x0, y0, RGB (color.red, color.green, color.blue));
}

void draw_line (double x0, double y0, double x1, double y1, Color color, int line_thickness)
{
    if (color.red != -1)
    {
        set_line_color (color, line_thickness);
    }

    txLine (x0, y0, x1, y1);
}

void draw_rectangle (double x0, double y0, double x1, double y1, Color color, int line_thickness)
{
    if (color.red != -1)
    {
        set_line_and_fill_color (color, line_thickness);
    }

    txRectangle (x0, y0, x1, y1);
}

void draw_triangle (double x0, double y0, double x1, double y1, double x2, double y2, Color color, int line_thickness)
{
    if (color.red != -1)
    {
        set_line_and_fill_color (color, line_thickness);
    }

    txTriangle (x0, y0, x1, y1, x2, y2);
}

void draw_circle (double x0, double y0, double r, Color color, int line_thickness)
{
    if (color.red != -1)
    {
        set_line_and_fill_color (color, line_thickness);
    }

    txEllipse (x0 - r, y0 - r, x0 + r, y0 + r);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Text Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool load_font (const char* name, int y_size)
{
    xxx.cur_font         = txSelectFont (name, y_size);
    xxx.name_of_cur_font = name;
    return xxx.check_font ();
}

bool set_font (double  size_y, double  size_x)
{
    if (xxx.cur_font == NULL) {
        return false;
    }

    return xxx.load_and_set_font (size_y, size_x);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Image  ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



Image::Image (HDC img, double init_width, double init_height):
        image  (img),
        width  (init_width),
        height (init_height)
{ }

Image::~Image ()
{
    txDeleteDC (image);
}

void Image::draw (double x, double y, double x_shift, double y_shift)
{
    txBitBlt (txDC (), x, y, width, height, image, x_shift, y_shift);
}

bool Image::exist () {
    return (image != NULL);
}

Point Image::get_size ()
{
    return {width, height};
}


Image load_image (const char* name, double width, double height)
{
    return {txLoadImage (name), width, height};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Mouse Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Point get_mouse_coordinates ()
{
    return {txMouseX (), txMouseY ()};
}


// returns 0 - if nothing pressed
//         1 - if left button pressed
//         2 - if right button pressed
Mouse_button_event::Mouse_button is_clicked ()
{
    auto button_state = txMouseButtons ();
    if (button_state & 1)
    {
        return  Mouse_button_event::LEFT_BUTTON;
    }

    if (button_state & 2)
    {
        return Mouse_button_event::RIGHT_BUTTON;
    }

    return Mouse_button_event::NOTHING;
}




#endif
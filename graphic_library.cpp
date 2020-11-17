

#ifndef GRAPHIC_LIBRARY
    #define GRAPHIC_LIBRARY

#include "graphic_library.hpp"
#include "subscription_manager.cpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Background   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Point Background::get_size ()
{
    return back_img.get_size ();
}

Background::Background (const char* name, double init_width, double init_height, double init_x, double init_y):
        back_img    (load_image (name, init_width, init_height)),
        color       (),
        x           (init_x),
        y           (init_y),
        width       (init_width),
        height      (init_height)
{ }

Background::Background (const char* name, double init_width, double init_height, double init_x, double init_y, double init_real_width, double init_real_height):
        back_img    (load_image (name, init_width, init_height)),
        color       (),
        x           (init_x),
        y           (init_y),
        width       (init_width),
        height      (init_height),
        real_width  (init_real_width),
        real_height (init_real_height)
{ }


Background::Background (Color init_color, double init_width, double init_height, double init_x, double init_y, Color init_line_clor, int init_thickness):
        back_img (),
        color    (init_color),
        x        (init_x),
        y        (init_y),
        width    (init_width),
        height   (init_height),
        thickness (init_thickness),
        line_color (init_line_clor)
{
    if (thickness == 0)
    {
        line_color = color;
        thickness = 1;
    }
}


Background::Background (Color init_color, double init_width, double init_height, double init_x, double init_y, double init_real_width, double init_real_height, Color init_line_clor, int init_thickness):
    back_img (),
    color    (init_color),
    x        (init_x),
    y        (init_y),
    width    (init_width),
    height   (init_height),
    thickness (init_thickness),
    line_color (init_line_clor),
    real_width  (init_real_width),
    real_height (init_real_height)
{
    if (thickness == 0)
    {
        line_color = color;
        thickness = 1;
    }
}


void Background::render ()
{
    if (back_img.exist ())
    {
        back_img.draw (x, y, x_shift, y_shift);
        return;
    }

    set_line_color (line_color, thickness);
    set_fill_color (color);
    draw_rectangle (x, y, x + width, y + height);
}

bool Background::process_event (Event* event)
{
    return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Button   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Action>
Button<Action>::Button (Action action_init, const char* name, double init_width, double init_height, double x_init, double y_init, Mouse_button_event::Mouse_button init_button):
        Background      (name, init_width, init_height, x_init, y_init, init_width, init_height),
        action          (action_init),
        button_to_press (init_button)
{ }

template<typename Action>
Button<Action>::Button (Action action_init, Color color, double init_width, double init_height, double x_init, double y_init, Color init_line_color, int thickness, Mouse_button_event::Mouse_button init_button):
        Background      (color, init_width, init_height, x_init, y_init, init_width, init_height, init_line_color, thickness),
        action          (action_init),
        button_to_press (init_button)
{ }

template<typename Action>
bool Button<Action>::contains_point (double mouse_x, double mouse_y)
{
    if ((mouse_x >= x && mouse_x <= x + width) &&
        (mouse_y >= y && mouse_y <= y + height))
    {
        return true;
    }
    return false;
}

template<typename Action>
bool Button<Action>::clicked (double mouse_x, double mouse_y)
{
    if (contains_point (mouse_x, mouse_y))
    {
        action ();
        return true;
    }

    return false;
}

template<typename Action>
void Button<Action>::render ()
{
    Background::render ();
}

template <typename Action>
void Button <Action>::hover ()
{ }

template <typename Action>
bool Button <Action>::process_event (Event* event)
{
    if (event->get_type () == BUTTON_CLICKED)
    {
        auto cur_coordinates = get_mouse_coordinates ();

        if (dynamic_cast<Mouse_button_event*> (event)->action == PRESSED &&
            dynamic_cast<Mouse_button_event*> (event)->button == button_to_press) {
            if (contains_point (cur_coordinates.x, cur_coordinates.y))
            {
                return true;
            }
        }

        if (dynamic_cast<Mouse_button_event*> (event)->action == RELEASED) {
            if (dynamic_cast<Mouse_button_event*> (event)->button == button_to_press)
                return clicked (cur_coordinates.x, cur_coordinates.y);
        }
    }
    return false;
}

template <typename Action>
void Button <Action>::change_action (Action new_action)
{
    action = new_action;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Window   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Window::Window (const char* name,
                double init_width,
                double init_height,
                double init_x,
                double init_y,
                double init_frame_width,
                double init_frame_height,
                double button_width,
                double init_real_width,
                double init_real_height):
        Background   (name, init_width - 2*init_frame_width,
                      init_height - 2*init_frame_height,
                      init_x + init_frame_width, init_y + init_frame_height,
                      init_real_width, init_real_height),
        close_window (Close_functor (this), {127, 0, 0}, button_width, init_frame_height, init_x + init_width - button_width, init_y),
        width        (init_width),
        height       (init_height),
        x            (init_x),
        y            (init_y),
        frame_width  (init_frame_width),
        frame_height (init_frame_height)
{ }

Window::Window (Color color,
                double init_width,
                double init_height,
                double init_x,
                double init_y,
                double init_frame_width,
                double init_frame_height,
                double button_width,
                double init_real_width,
                double init_real_height):
            Background   (color, init_width - 2*init_frame_width,
                          init_height - 2*init_frame_height,
                          init_x + init_frame_width, init_y + init_frame_height,
                          init_real_width, init_real_height),
            close_window (Close_functor (this), {127, 0, 0}, button_width, init_frame_height, init_x + init_width - button_width, init_y),
            width        (init_width),
            height       (init_height),
            x            (init_x),
            y            (init_y),
            frame_width  (init_frame_width),
            frame_height (init_frame_height)
{ }


bool Window::contains_point (double mouse_x, double mouse_y)
{
    if ((mouse_x >= x && mouse_x <= x + width) &&
            (mouse_y >= y && mouse_y <= y + height))
    {
        return true;
    }
    return false;
}

void Window::hover ()
{ }

bool Window::clicked (double mouse_x, double mouse_y)
{
    return close_window.clicked (mouse_x, mouse_y);
}

void Window::render ()
{
    draw_rectangle      (x, y, x + width, y + height, {225, 225, 225}, 0);
    Background::render  ();
    close_window.render ();
}

bool Window::process_event (Event* event)
{
    return close_window.process_event (event);
}

Window::~Window () { }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Window_manager   /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Window_manager::Window_manager (std::vector<Abstract_window*> init_windows, bool init_top_manager):
        windows (init_windows),
        top_manager (init_top_manager)
{ }

void Window_manager::manage_windows ()
{
    txBegin ();
    if (top_manager)
    {
        store_events ();
    }

    while (!empty_queue ())
    {
        Event* cur_event = get_event ();
        if (top_manager && cur_event->get_type () == PROGRAM_CLOSE) {
            exit (0);
        }
        process_event (cur_event);
    }

    render ();
}

bool Window_manager::process_event (Event* event)
{
    if (event->get_type () == CLOSE_WINDOW)
    {
        auto it = std::find (windows.begin (), windows.end (), dynamic_cast<Close_window_event*> (event)->get_window ());
        windows.erase (it);
    }

    for (int i = windows.size () - 1; i >= 0; i--)
    {
        if (windows[i]->process_event (event))
        {
            break;
        }
    }
}

void Window_manager::render ()
{
    for (auto& i : windows)
    {
        i->render ();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Scroller  ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



Scrollbar::Scroller::Scroller (double init_width, double init_height, double init_x, double init_y, Color init_color):
        cur_x       (init_x + 1),
        cur_y       (init_y + 1),
        x           (init_x),
        y           (init_y),
        width       (init_width),
        height      (init_height),
        color       (init_color)
{ }

void Scrollbar::Scroller::render ()
{
    set_line_color (NO_COLOR, 1);
    set_fill_color (color);
    draw_rectangle (cur_x, cur_y, cur_x + width, cur_y + height);
}

bool Scrollbar::Scroller::process_event (Event* event)
{ }

bool Scrollbar::Scroller::contains_point (double mouse_x, double mouse_y)
{
    if ((mouse_x >= cur_x && mouse_x <= cur_x + width) &&
        (mouse_y >= cur_y && mouse_y <= cur_y + height))
    {
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Scrollbar   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Scrollbar::Scrollbar(double init_x, double init_y, double init_width, double init_height,
                     double init_scroller_width, double init_scroller_height, Color init_scroller_color,
                     const char* button_up, const char* button_down, char kind, Point init_real_size, Color color):
            x         (init_x),
            y         (init_y),
            width     (init_width),
            height    (init_height),
            real_size (init_real_size),
            what      (kind),
            page_up   (kind == 0 ? Key_functor (ARROW_UP) : Key_functor (ARROW_LEFT),
                       button_up,   kind == 0 ? init_width : init_height, kind == 0 ? init_width : init_height, init_x, init_y),
            page_down (kind == 0 ? Key_functor (ARROW_DOWN) : Key_functor (ARROW_RIGHT),
                       button_down, kind == 0 ? init_width : init_height, kind == 0 ? init_width : init_height,
                       init_x + (kind == 0 ? 0 : width - 2*init_height), init_y + (kind == 0 ? height - 2*init_width : 0)),
            rect      (Key_functor (), color,    kind == 0 ? init_width : init_width - 3*init_height,  kind == 0 ? init_height - 3*init_width : init_height,
                       kind == 0 ? init_x : init_x + init_height, kind == 0 ? init_y + init_width : init_y),
            scroller  (init_scroller_width, init_scroller_height, kind == 0 ? init_x : init_x + init_height, kind == 0 ? init_y + init_width : init_y, init_scroller_color)
{ }

bool Scrollbar::contains_point (double mouse_x, double mouse_y)
{
    if ((mouse_x >= x && mouse_x <= x + width) &&
            (mouse_y >= y && mouse_y <= y + height))
    {
        return true;
    }
    return false;
}

void Scrollbar::hover ()
{ }

bool Scrollbar::clicked  (double mouse_x, double mouse_y)
{
    bool return_1 = page_up.clicked (mouse_x, mouse_y);
    bool return_2 = page_down.clicked (mouse_x, mouse_y);
    bool return_3 = rect.clicked (mouse_x, mouse_y);
    return return_1 || return_2 || return_3;
}

void Scrollbar::render ()
{
    page_up.render ();
    page_down.render ();
    rect.render ();
    scroller.render ();
}

bool Scrollbar::process_event (Event* event)
{
    if (dynamic_cast<Mouse_button_event*> (event)->action == RELEASED) {
        pressed = false;
        return false;
    }

    if (pressed || scroller.contains_point (dynamic_cast<Mouse_button_event*> (event)->pos.x, dynamic_cast<Mouse_button_event*> (event)->pos.y))
    {
        pressed = true;
        auto new_y = dynamic_cast<Mouse_button_event*> (event)->pos.y;
        auto new_x = dynamic_cast<Mouse_button_event*> (event)->pos.x;

        if (what == 0)
        {

            if (new_y < y + height &&
                new_y > y)
            {

                if (scroller.cur_y - new_y < 0)
                {
                    add_event (new Scroll_event (Scroll_event::DOWN, new_y - scroller.cur_y));
                } else
                {
                    add_event (new Scroll_event (Scroll_event::UP, scroller.cur_y - new_y));
                }
                double cur_shift = (height - 3*width)/(real_size.y - height)*(new_y - scroller.cur_y);
                set_up_down (cur_shift);
            }
        }
        else
        {
            if (new_x < x + width &&
                new_x > x)
            {
                if (scroller.cur_x - new_x < 0)
                {
                    add_event (new Scroll_event (Scroll_event::RIGHT, new_x - scroller.cur_x));
                } else
                {
                    add_event (new Scroll_event (Scroll_event::LEFT, scroller.cur_x - new_x));
                }
                double cur_shift = (width - 3*height)/(real_size.x - width)*(new_x - scroller.cur_x);
                set_right_left (cur_shift);
            }
        }
        return true;
    }

    if (rect.contains_point (dynamic_cast<Mouse_button_event*> (event)->pos.x, dynamic_cast<Mouse_button_event*> (event)->pos.y))
    {
        if (what == 0)
        {
            if (dynamic_cast<Mouse_button_event*> (event)->pos.y < scroller.cur_y)
            {
                rect.change_action (Key_functor (ARROW_UP));
            } else if (dynamic_cast<Mouse_button_event*> (event)->pos.y > scroller.cur_y + scroller.height)
                {
                    rect.change_action (Key_functor (ARROW_DOWN));
                }
        }
        else
        {
            if (dynamic_cast<Mouse_button_event*> (event)->pos.x < scroller.cur_x)
            {
                rect.change_action (Key_functor (ARROW_LEFT));
            } else if (dynamic_cast<Mouse_button_event*> (event)->pos.x > scroller.cur_x + scroller.width)
                {
                    rect.change_action (Key_functor (ARROW_RIGHT));
                }
        }
    }
    return clicked (dynamic_cast<Mouse_button_event*> (event)->pos.x, dynamic_cast<Mouse_button_event*> (event)->pos.y);
}

Scrollbar::~Scrollbar () { }

void Scrollbar::set_up_down (double shift)
{
    if (scroller.cur_y + shift < scroller.y + height - 4*width - 3 && scroller.cur_y + shift > scroller.y)
    {
        scroller.cur_y += shift;
    } else if (scroller.cur_y + shift >= scroller.y + height - 4*width - 3) {
            scroller.cur_y = scroller.y + height - 4*width - 3;
        } else if (scroller.cur_y + shift <= scroller.y) {
                scroller.cur_y = scroller.y;
            }
}

void Scrollbar::up_down (double shift)
{
    double cur_shift = (height - 3*width)/(real_size.y - height)*shift;
    set_up_down (cur_shift);
}

void Scrollbar::set_right_left (double shift)
{
    if (scroller.cur_x + shift < scroller.x + width - 4*height - 3 && scroller.cur_x + shift > scroller.x)
    {
        scroller.cur_x += shift;
    } else if (scroller.cur_x + shift >= scroller.x + width - 4*height - 3) {
            scroller.cur_x = scroller.x + width - 4*height - 3;
        } else if (scroller.cur_x + shift <= scroller.x)  {
                scroller.cur_x = scroller.x;
            }
}

void Scrollbar::right_left (double shift)
{
    double cur_shift = (width - 3*height)/(real_size.x - width)*shift;
    set_right_left (cur_shift);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Window_with_scrollbar   //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Window_with_scrollbar::page_up (double shift)
{
    if (cur_y > 0)
    {
        double plus_y = std::min (shift, cur_y);

        change_coordinates ({0, -plus_y});
        cur_y -= plus_y;
    }
}

void Window_with_scrollbar::page_left (double shift)
{
    if (cur_x > 0)
    {
        double plus_x = std::min (shift, cur_x);

        change_coordinates ({-plus_x, 0});
        cur_x -= plus_x;
    }
}

void Window_with_scrollbar::page_down (double shift)
{
    if (cur_y < max_y)
    {
        double plus_y = std::min (shift, max_y - cur_y);

        change_coordinates ({0, plus_y});
        cur_y += plus_y;
    }
}

void Window_with_scrollbar::page_right (double shift)
{
    if (cur_x < max_x)
    {
        double plus_x = std::min (shift, max_x - cur_x);

        change_coordinates ({plus_x, 0});
        cur_x += plus_x;
    }
}


Window_with_scrollbar::Window_with_scrollbar (const char* name, double init_width, double init_height, double init_real_width, double init_real_height, double init_x, double init_y):
        Window (name, init_width, init_height, init_x, init_y, 0, 10, 30, init_width, init_height),
        bar    {{x + width - frame_width - 16,
                        y + frame_height,
                        16, height - 2*frame_height,
                        15, 20, {100, 100, 100},
                        "scroll-bar-arrow-up.bmp", "scroll-bar-arrow-down.bmp", Y_BAR,
                        {init_real_width, init_real_height},
                        {120, 120, 120}},
                {x + frame_width,
                        y + height - frame_height - 16,
                        width - 2*frame_width, 16,
                        20, 15, {100, 100, 100},
                        "scroll-bar-arrow-left.bmp", "scroll-bar-arrow-right.bmp", X_BAR,
                        {init_real_width, init_real_height},
                        {120, 120, 120}}},
        kind_of_bar (X_Y_BAR),
        max_x  (init_real_width - init_width + 2*0),
        max_y  (init_real_height - init_height + 2*10),
        cur_x  (0),
        cur_y  (0)
{

}

Window_with_scrollbar::Window_with_scrollbar   (const char* name,
                                                double init_width,
                                                double init_height,
                                                double init_real_width,         // real size of image
                                                double init_real_height,        // real size of image
                                                double init_x,
                                                double init_y,
                                                double init_frame_width,
                                                double init_frame_height,
                                                double button_width,
                                                double bar_width,
                                                const char* button_up,
                                                const char* button_down,
                                                char  init_kind_of_bar,
                                                const char* button_left,
                                                const char* button_right,
                                                Color bar_color,
                                                double init_scroller_width,
                                                double init_scroller_height,
                                                Color init_scroller_color):
            Window (name, init_width, init_height, init_x, init_y, init_frame_width, init_frame_height, button_width, init_width, init_height),
            bar    {{x + width - frame_width - bar_width,
                            y + frame_height,
                            bar_width, height - 2*frame_height,
                            init_scroller_width, init_scroller_height, init_scroller_color,
                            button_up, button_down, Y_BAR,
                            {init_real_width, init_real_height},
                            bar_color},
                    {x + frame_width,
                            y + height - frame_height - bar_width,
                            width - 2*frame_width, bar_width,
                            init_scroller_height, init_scroller_width, init_scroller_color,
                            button_left, button_right, X_BAR,
                            {init_real_width, init_real_height},
                            bar_color}},
            kind_of_bar (init_kind_of_bar),
            max_x  (init_real_width - init_width + 2*init_frame_width),
            max_y  (init_real_height - init_height + 2*init_frame_height),
            cur_x  (0),
            cur_y  (0)
    { }

bool Window_with_scrollbar::contains_point (double mouse_x, double mouse_y)
{
    if ((mouse_x >= x && mouse_x <= x + width) &&
            (mouse_y >= y && mouse_y <= y + height))
    {
        return true;
    }
    return false;
};

void Window_with_scrollbar::hover ()
{ }

bool Window_with_scrollbar::clicked (double mouse_x, double mouse_y)
{ }

void Window_with_scrollbar::render ()
{
    Window::render ();

    if (kind_of_bar == X_Y_BAR)
    {
        bar[0].render ();
        bar[1].render ();
        return;
    }
    bar[kind_of_bar].render ();
}



bool Window_with_scrollbar::process_event (Event* event)
{
    if (Window::process_event (event) == true) {
        return true;
    }

    if (event->get_type () == SCROLL_EVENT) {
        double shift = dynamic_cast<Scroll_event*> (event)->shift;
        switch (dynamic_cast<Scroll_event*> (event)->direction)
        {
            case Scroll_event::UP:
            {
                page_up (shift);
                break;
            }
            case Scroll_event::DOWN:
            {
                page_down (shift);
                break;
            }
            case Scroll_event::RIGHT:
            {
                page_right (shift);
                break;
            }
            case Scroll_event::LEFT:
            {
                page_left (shift);
                break;
            }
        }
        return true;
    }

    if (event->get_type () == BUTTON_CLICKED)
    {
        bool return1 = false;
        if (kind_of_bar == X_Y_BAR)
        {
            return1 = bar[0].process_event (event);
            bool return2 = bar[1].process_event (event);
            if (return1 || return2)
            {
                return true;
            }
        } else
        {
            return1 = bar[kind_of_bar].process_event (event);
        }
        return return1;
    }

    if (event->get_type () == KEY_CLICKED)
    {
        auto key_event = dynamic_cast<Keybord_event*> (event);
        switch (key_event->key)
        {
            case ARROW_UP:
            {
                page_up (5);
                bar[0].up_down (-5);
                break;
            }

            case ARROW_DOWN:
            {
                page_down (5);
                bar[0].up_down (5);
                break;
            }

            case ARROW_RIGHT:
            {
                page_right (5);
                bar[1].right_left (5);
                break;
            }

            case ARROW_LEFT:
            {
                page_left (5);
                bar[1].right_left (-5);
                break;
            }

            case PAGE_UP:
            {
                if (!pressed && key_event->action == PRESSED) {
                    pressed = true;
                    page_up (height);
                    bar[0].up_down (-height);
                }

                if (key_event->action == RELEASED) {
                    pressed = false;
                }

                break;
            }

            case PAGE_DOWN:
            {
                if (!pressed && key_event->action == PRESSED) {
                    pressed = true;
                    page_down (height);
                    bar[0].up_down (height);
                }

                if (key_event->action == RELEASED) {
                    pressed = false;
                }
                break;
            }

            default:
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

Window_with_scrollbar::~Window_with_scrollbar () { }





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Exit_functor   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Exit_functor::operator () ()
{
    exit (0);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Draw Functions   ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_connected_line (std::vector<Point> points, std::size_t size, Color color, int line_thickness)
{
    for (std::size_t i = 0; i < size - 1; i++)
    {
        draw_line (points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color, line_thickness);
    }
}


#endif // GRAPHIC_LIBRARY
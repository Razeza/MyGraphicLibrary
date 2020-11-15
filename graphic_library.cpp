#include "graphic_library.hpp"

#ifndef GRAPHIC_LIBRARY
    #define GRAPHIC_LIBRARY



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

bool Background::process_event (const Event &event)
{
    return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Button   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Action>
Button<Action>::Button (Action action_init, const char* name, double init_width, double init_height, double x_init, double y_init):
        Background (name, init_width, init_height, x_init, y_init, init_width, init_height),
        action     (action_init)
{ }

template<typename Action>
Button<Action>::Button (Action action_init, Color color, double init_width, double init_height, double x_init, double y_init, Color init_line_color, int thickness):
        Background (color, init_width, init_height, x_init, y_init, init_width, init_height, init_line_color, thickness),
        action     (action_init)
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
bool Button <Action>::process_event (const Event& event)
{
    if (event.event.event1.event_type == BUTTON_CLICKED)
    {
        auto cur_coordinates = get_mouse_coordinates ();
        clicked (cur_coordinates.x, cur_coordinates.y);
        return true;
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
        close_window (Close_functor ((void*) this), {127, 0, 0}, button_width, init_frame_height, init_x + init_width - button_width, init_y),
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
            close_window (Close_functor ((void*) this), {127, 0, 0}, button_width, init_frame_height, init_x + init_width - button_width, init_y),
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

bool Window::process_event (const Event& event)
{
    if (event.event.event1.event_type == BUTTON_CLICKED)
    {
        return clicked (event.event.event1.coordinates.x, event.event.event1.coordinates.y);
    }
    return false;
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

    Event cur_event = get_event ();
    if (cur_event.event.event1.event_type != NO_EVENT)
    {
        process_event (cur_event);
        render ();
    }
}

bool Window_manager::process_event (const Event& event)
{
    if (event.event.event1.event_type == CLOSE_WINDOW)
    {
        auto it = std::find (windows.begin (), windows.end (), event.event.event4.window);
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

bool Scrollbar::Scroller::process_event (const Event& event)
{ }

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

bool Scrollbar::process_event (const Event& event)
{
    if (rect.contains_point (event.event.event1.coordinates.x, event.event.event1.coordinates.y))
    {
        if (what == 0)
        {
            if (event.event.event1.coordinates.y < scroller.cur_y)
            {
                rect.change_action (Key_functor (ARROW_UP));
            } else if (event.event.event1.coordinates.y > scroller.cur_y + scroller.height)
                {
                    rect.change_action (Key_functor (ARROW_DOWN));
                }
        }
        else
        {
            if (event.event.event1.coordinates.x < scroller.cur_x)
            {
                rect.change_action (Key_functor (ARROW_LEFT));
            } else if (event.event.event1.coordinates.x > scroller.cur_x + scroller.width)
                {
                    rect.change_action (Key_functor (ARROW_RIGHT));
                }
        }
    }
    clicked (event.event.event1.coordinates.x, event.event.event1.coordinates.y);
}

Scrollbar::~Scrollbar () { }

void Scrollbar::up_down (double shift)
{
    double cur_shift = (height - 3*width)/(real_size.y - height)*shift;
    if (scroller.cur_y + cur_shift < scroller.y + height - 4*width - 3 && scroller.cur_y + cur_shift > scroller.y)
    {
        scroller.cur_y += cur_shift;
    }
}

void Scrollbar::right_left (double shift)
{
    double cur_shift = (width - 3*height)/(real_size.x - width)*shift;
    if (scroller.cur_x + cur_shift < scroller.x + width - 4*height - 3 && scroller.cur_x + cur_shift > scroller.x)
    {
        scroller.cur_x += cur_shift;
    }
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
{ };

bool Window_with_scrollbar::clicked (double mouse_x, double mouse_y)
{
    Window::clicked (mouse_x, mouse_y);
};

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



bool Window_with_scrollbar::process_event (const Event& event)
{
    if (event.event.event1.event_type == BUTTON_CLICKED)
    {
        bool return1 = false;
        if (kind_of_bar == X_Y_BAR)
        {
            bool return1 = bar[0].process_event (event);
            bool return2 = bar[1].process_event (event);
            if (return1 || return2)
            {
                return true;
            }
        } else
        {
            return1 = bar[kind_of_bar].process_event (event);
        }
        return return1 || clicked (event.event.event1.coordinates.x, event.event.event1.coordinates.y);
    }

    if (event.event.event1.event_type == KEY_CLICKED)
    {
        switch (event.event.event2.key)
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
                page_up (height);
                bar[0].up_down (-height);
                break;
            }

            case PAGE_DOWN:
            {
                page_down (height);
                bar[0].up_down (height);
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
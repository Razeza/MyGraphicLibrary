

#ifndef GRAPHIC_LIBRARY
    #define GRAPHIC_LIBRARY

#include "graphic_library.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Background   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Point Background::get_size ()
{
    return back_img.get_size ();
}

Background::Background (const char* name, Point size, Point start):
        back_img    (name, size),
        color       ()
{
    back_img.set_pos (start);
}

Background::Background (const char* name, Point size, Point start, Point real_size):
        back_img    (name, size),
        color       (),
        real_size   (real_size)
{
    back_img.set_pos (start);
}


Background::Background (Color init_color, Point size, Point start, Color init_line_clor, int init_thickness):
        back_img   (),
        color      (init_color),
        thickness  (init_thickness),
        line_color (init_line_clor)
{
    back_img.set_pos (start);
    back_img.change_size(size);
    if (thickness == 0)
    {
        line_color = color;
        thickness = 1;
    }
}


Background::Background (Color init_color, Point size, Point start, Point real_size, Color init_line_color, int init_thickness):
    back_img   (),
    color      (init_color),
    thickness  (init_thickness),
    line_color (init_line_color),
    real_size  (real_size)
{
    back_img.set_pos (start);
    back_img.change_size(size);
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
        back_img.draw ();
        return;
    }

    set_line_color (line_color, thickness);
    set_fill_color (color);
    draw_rectangle (back_img.get_start(), back_img.get_start() + back_img.get_full_size());
}

bool Background::process_event (Event* event)
{
    return false;
}

void Background::change_size (Point new_size)
{
    back_img.change_size(new_size);
}

void Background::move (Point new_size)
{
    back_img.set_shift(new_size);
}

void Background::change_coordinates (Point shift)
{
    back_img.shift_coordinates (shift);
}

Image* Background::get_image ()
{
    return &back_img;
}

Point Background::get_start() {
    return back_img.get_start();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Button   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Action>
Button<Action>::Button (Action action_init, const char* name, Point size, Point start, Mouse_button_event::Mouse_button init_button):
        Background      (name, size, start, size),
        action          (action_init),
        button_to_press (init_button)
{ }

template<typename Action>
Button<Action>::Button (Action action_init, Color color, Point size, Point start, Color init_line_color, int thickness, Mouse_button_event::Mouse_button init_button):
        Background      (color, size, start, size, init_line_color, thickness),
        action          (action_init),
        button_to_press (init_button)
{ }

template<typename Action>
bool Button<Action>::contains_point (Point mouse)
{
    auto [mouse_x, mouse_y] = mouse;
    auto [x, y] = get_start();
    auto [width, height] = get_size();

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
    if (contains_point ({mouse_x, mouse_y}))
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
        auto cur_coordinates = dynamic_cast<Mouse_button_event*> (event)->pos;

        if (dynamic_cast<Mouse_button_event*> (event)->action == PRESSED &&
            dynamic_cast<Mouse_button_event*> (event)->button == button_to_press) {
            if (contains_point ({cur_coordinates.x, cur_coordinates.y}))
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
                Point window_size,
                Point window_start,
                Point frame_size,
                double button_width,
                Point real_size,
                Point image_size,
                Point image_start):
        Background   (name, image_size,
                      {window_start.x + image_start.x, window_start.y + image_start.y},
                      real_size),
        close_window (Close_functor (this), {127, 0, 0}, {button_width, frame_size.y},
                                                         {window_start.x + window_size.x - button_width, window_start.y}),
        window_size  (window_size),
        window_start (window_start),
        frame_size   (frame_size)
{ }

Window::Window (Color color,
                Point window_size,
                Point window_start,
                Point frame_size,
                double button_width,
                Point real_size):
            Background   (color, {window_size.x - 2*frame_size.x,window_size.x - 2*frame_size.y},
                          {window_start.x + frame_size.x, window_start.y + frame_size.y},
                          real_size),
            close_window (Close_functor (this), {127, 0, 0}, {button_width, frame_size.y}, {window_start.x + window_size.x - button_width, window_size.y}),
            window_size  (window_size),
            window_start (window_start),
            frame_size   (frame_size)
{ }


bool Window::contains_point (Point mouse)
{
    auto [mouse_x, mouse_y] = mouse;
    auto [x, y] = get_start();
    auto [width, height] = get_size();

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
    draw_rectangle      (get_start(), get_start() + get_size(), {225, 225, 225}, 0);
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

Window_manager::codes Window_manager::manage_windows ()
{
    if (top_manager)
    {
        store_events ();
    }

    while (!empty_queue ())
    {
        Event* cur_event = get_event ();
        if (top_manager && cur_event->get_type () == PROGRAM_CLOSE) {
            return CLOSE;
        }
        process_event (cur_event);
    }

    render ();
    return OK;
}

bool Window_manager::process_event (Event* event)
{
    if (event->get_type () == CLOSE_WINDOW)
    {
        auto it = std::find (windows.begin (), windows.end (), dynamic_cast<Close_window_event*> (event)->get_window ());
        windows.erase (it);
        return true;
    }

    for (int i = windows.size () - 1; i >= 0; i--)
    {
        if (windows[i]->process_event (event))
        {
            return true;
        }
    }

    return false;
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



Scrollbar::Scroller::Scroller (Point size, Point start, Color init_color):
        cur_place (start),
        start     (start),
        size      (size),
        color     (init_color)
{ }

void Scrollbar::Scroller::render ()
{
    set_line_color (NO_COLOR, 1);
    set_fill_color (color);
    draw_rectangle (cur_place, cur_place + size);
}

bool Scrollbar::Scroller::process_event (Event* event)
{ return false; }

bool Scrollbar::Scroller::contains_point (Point mouse)
{
    auto [mouse_x, mouse_y] = mouse;
    auto [cur_x, cur_y]     = cur_place;
    auto [width, height]    = size;

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

Scrollbar::Scrollbar(Point _start, Point _size, Point scroller_size, Color init_scroller_color,
                     const char* button_up, const char* button_down, Point init_real_size, Point shown_size, char kind, Color color):
            start      (_start),
            size       (_size),
            real_size  (init_real_size),
            shown_size (shown_size),
            what       (kind),
            page_up    (kind == 0 ? Key_functor (ARROW_UP) : Key_functor (ARROW_LEFT),
                        button_up,   {kind == 0 ? size.x : size.y, kind == 0 ? size.x : size.y}, start),
            page_down  (kind == 0 ? Key_functor (ARROW_DOWN) : Key_functor (ARROW_RIGHT),
                        button_down, {kind == 0 ? size.x : size.y, kind == 0 ? size.x : size.y},
                        {start.x + (kind == 0 ? 0 : size.x - 2*size.y), start.y + (kind == 0 ? size.y - 2*size.x : 0)}),
            rect       (Key_functor (), color,   {kind == 0 ? size.x : size.x - 3*size.y,  kind == 0 ? size.y - 3*size.x : size.y},
                        {kind == 0 ? start.x : start.x + size.y, kind == 0 ? start.y + size.x : start.y}),
            scroller  (scroller_size, {kind == 0 ? start.x : start.x + size.y, kind == 0 ? start.y + size.x : start.y}, init_scroller_color)
{ }

bool Scrollbar::contains_point (Point mouse)
{
    auto [mouse_x, mouse_y] = mouse;
    auto [x, y]             = start;
    auto [width, height]    = size;

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

    auto [cur_x, cur_y]  = scroller.cur_place;
    auto [x, y]          = start;
    auto [width, height] = size;

    if (pressed || scroller.contains_point ({dynamic_cast<Mouse_button_event*> (event)->pos.x,
                                             dynamic_cast<Mouse_button_event*> (event)->pos.y}))
    {
        pressed = true;
        auto new_y = dynamic_cast<Mouse_button_event*> (event)->pos.y;
        auto new_x = dynamic_cast<Mouse_button_event*> (event)->pos.x;

        if (what == 0)
        {

            if (new_y < y + height &&
                new_y > y)
            {

                if (cur_y - new_y < 0)
                {
                    add_event (new Scroll_event (Scroll_event::DOWN, new_y - cur_y));
                } else
                {
                    add_event (new Scroll_event (Scroll_event::UP, cur_y - new_y));
                }
                double cur_shift = (height - 3*width)/(real_size.y - height)*(new_y - cur_y);
                set_up_down (cur_shift);
            }
        }
        else
        {
            if (new_x < x + width &&
                new_x > x)
            {
                if (cur_x - new_x < 0)
                {
                    add_event (new Scroll_event (Scroll_event::RIGHT, new_x - cur_x));
                } else
                {
                    add_event (new Scroll_event (Scroll_event::LEFT, cur_x - new_x));
                }
                double cur_shift = (width - 3*height)/(real_size.x - width)*(new_x - cur_x);
                set_right_left (cur_shift);
            }
        }
        return true;
    }

    if (rect.contains_point ({dynamic_cast<Mouse_button_event*> (event)->pos.x,
                              dynamic_cast<Mouse_button_event*> (event)->pos.y}))
    {
        if (what == 0)
        {
            if (dynamic_cast<Mouse_button_event*> (event)->pos.y < cur_y)
            {
                rect.change_action (Key_functor (ARROW_UP));
            } else if (dynamic_cast<Mouse_button_event*> (event)->pos.y > cur_y + scroller.size.y)
                {
                    rect.change_action (Key_functor (ARROW_DOWN));
                }
        }
        else
        {
            if (dynamic_cast<Mouse_button_event*> (event)->pos.x < cur_x)
            {
                rect.change_action (Key_functor (ARROW_LEFT));
            } else if (dynamic_cast<Mouse_button_event*> (event)->pos.x > cur_x + scroller.size.x)
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
    auto& [cur_x, cur_y] = scroller.cur_place;
    auto [x, y]          = scroller.start;
    auto [width, height] = size;

    if (cur_y + shift < y + height - scroller.size.y - 3*width && cur_y + shift > y)
    {
        cur_y += shift;
    } else if (cur_y + shift >= y + height - scroller.size.y - 3*width) {
            cur_y = y + height - scroller.size.y - 3*width;
        } else if (cur_y + shift <= y) {
                cur_y = y;
            }
}

void Scrollbar::up_down (double shift)
{
    auto [width, height] = size;

    double cur_shift = (height - scroller.size.y - 3*width)*shift/(real_size.y - shown_size.y);
    if (real_size.y - shown_size.y == 0) {
        cur_shift = 0;
    }
    set_up_down (cur_shift);
}

void Scrollbar::set_right_left (double shift)
{
    auto& [cur_x, cur_y]  = scroller.cur_place;
    auto [x, y]          = scroller.start;
    auto [width, height] = size;

    if (cur_x + shift < x  + width - scroller.size.x - 3*height && cur_x + shift > x)
    {
        cur_x += shift;
    } else if (cur_x + shift >= x  + width - scroller.size.x - 3*height) {
            cur_x = x  + width - scroller.size.x - 3*height;
        } else if (cur_x + shift  <= x)  {
                cur_x = x;
            }
}

void Scrollbar::right_left (double shift)
{
    auto [width, height] = size;

    double cur_shift = (width - scroller.size.x - 3*height)*shift/(real_size.x - shown_size.x);
    set_right_left (cur_shift);
}

void Scrollbar::set_real_size (Point size)
{
    real_size = size;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Scroller Helper Functions   ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void page_up (Scrollable* image, double shift)
{
    auto cur = image->get_cur_start ();
    auto cur_y = cur.y;
    if (cur_y > 0)
    {
        double plus_y = std::min (shift, cur_y);

        image->shift_coordinates ({0, -plus_y});
    }
}

void page_left (Scrollable* image, double shift)
{
    auto cur = image->get_cur_start ();
    auto cur_x = cur.x;
    if (cur_x > 0)
    {
        double plus_x = std::min (shift, cur_x);

        image->shift_coordinates ({-plus_x, 0});
    }
}

void page_down (Scrollable* image, double shift)
{
    auto scale = image->get_scale ();
    auto cur = image->get_cur_start ();
    auto cur_y = cur.y*scale.x;

    auto real_size = image->get_full_size ();
    auto size = image->get_size ();
    auto max_y = real_size.y*scale.y - size.y;
    if (cur_y < max_y)
    {
        double plus_y = std::min (shift, max_y - cur_y);

        image->shift_coordinates ({0, plus_y});
    }
}

void page_right (Scrollable* image, double shift)
{
    auto scale = image->get_scale ();
    auto cur = image->get_cur_start ();
    auto cur_x = cur.x*scale.x;

    auto real_size = image->get_full_size ();
    auto size = image->get_size ();
    auto max_x = real_size.x*scale.x - size.x;
    if (cur_x < max_x)
    {
        double plus_x = std::min (shift, max_x - cur_x);

        image->shift_coordinates ({plus_x, 0});
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class View_port   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool View_port::process_event (Event* event) {
    auto size = scrollable_image->get_full_size ();
    auto scale = scrollable_image->get_scale ();

    if (X_Y_BAR == kind_of_bar) {
        bar[0].set_real_size ({size.x*scale.x, size.y*scale.y});
        bar[1].set_real_size ({size.x*scale.x, size.y*scale.y});
    }
    bar[kind_of_bar].set_real_size ({size.x*scale.x, size.y*scale.y});

    if (event->get_type () == SCROLL_EVENT) {
        double shift = dynamic_cast<Scroll_event*> (event)->shift;
        switch (dynamic_cast<Scroll_event*> (event)->direction)
        {
            case Scroll_event::UP:
            {
                page_up (scrollable_image, shift);
                break;
            }
            case Scroll_event::DOWN:
            {
                page_down (scrollable_image, shift);
                break;
            }
            case Scroll_event::RIGHT:
            {
                page_right (scrollable_image, shift);
                break;
            }
            case Scroll_event::LEFT:
            {
                page_left (scrollable_image, shift);
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

        auto scale = scrollable_image->get_scale ();

        switch (key_event->key)
        {
            case ARROW_UP:
            {
                page_up (scrollable_image, 5.0 / scale.y);
                bar[0].up_down (-5.0);
                break;
            }

            case ARROW_DOWN:
            {
                page_down (scrollable_image, 5.0 / scale.y);
                bar[0].up_down (5.0);
                break;
            }

            case ARROW_RIGHT:
            {
                page_right (scrollable_image, 5.0 / scale.x);
                bar[1].right_left (5.0);
                break;
            }

            case ARROW_LEFT:
            {
                page_left (scrollable_image, 5.0 / scale.x);
                bar[1].right_left (-5.0);
                break;
            }

            case PAGE_UP:
            {
                if (!pressed && key_event->action == PRESSED) {
                    pressed = true;
                    page_up (scrollable_image, scrollable_image->get_size ().y / scale.y);
                    bar[0].up_down (-scrollable_image->get_size ().y);
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
                    page_down (scrollable_image, scrollable_image->get_size ().y / scale.y);
                    bar[0].up_down (scrollable_image->get_size ().y);
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

void View_port::render ()
{
        if (kind_of_bar == X_Y_BAR)
        {
            bar[0].render ();
            bar[1].render ();
            return;
        }
        bar[kind_of_bar].render ();
}

View_port::View_port (Scrollable* image, Settings settings[2]):
    scrollable_image (image),
    bar    {{settings[0].left_button,
            {settings[0].button_size.x, settings[0].right_button.y - settings[0].left_button.y + settings[0].button_size.y},
            settings[0].scrl_settings.scroller_size, settings[0].scrl_settings.scroller_color,
            "graphic/scroll-bar-arrow-up.bmp", "graphic/scroll-bar-arrow-down.bmp",
            image->get_full_size (), image->get_size(), Y_BAR,
            settings[0].color},
            {settings[1].left_button,
            {settings[1].right_button.x - settings[1].left_button.x + settings[1].button_size.x, settings[1].button_size.y},
            settings[1].scrl_settings.scroller_size, settings[1].scrl_settings.scroller_color,
            "graphic/scroll-bar-arrow-left.bmp", "graphic/scroll-bar-arrow-right.bmp",
            image->get_full_size (), image->get_size(), X_BAR,
            settings[1].color}},
    bar_setting {settings[0], settings[1]}
{ }



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
        draw_line (points[i], points[i + 1], color, line_thickness);
    }
}

void draw_circled_rectangle (Point start, Point size, Color fill_color, Color line_color, int thickness) {
    auto [start_x, start_y] = start;

    draw_rectangle ({start_x + thickness, start_y}, // up
                    {start_x + thickness + size.x, start_y + thickness}, line_color, 0);
    draw_rectangle ({start_x + thickness, start_y + thickness + size.y},// down
                    {start_x + thickness + size.x, start_y + 2*thickness + size.y}, line_color, 0);
    draw_rectangle ({start_x, start_y + thickness}, // left
                    {start_x + thickness, start_y + thickness + size.y}, line_color, 0);
    draw_rectangle ({start_x + thickness + size.x, start_y + thickness}, // right
                    {start_x + 2*thickness + size.x, start_y + thickness + size.y}, line_color, 0);

    draw_circle ({start_x + thickness, start_y + thickness}, thickness, line_color, 0);  // left up
    draw_circle ({start_x + thickness + size.x, start_y + thickness}, thickness, line_color, 0); // right up
    draw_circle ({start_x + thickness, start_y + thickness + size.y}, thickness, line_color, 0); //  left down
    draw_circle ({start_x + thickness + size.x, start_y + thickness + size.y}, thickness, line_color, 0); // right down

    draw_rectangle ({start_x + thickness, start_y + thickness},
                    {start_x + thickness + size.x, start_y + thickness + size.y}, fill_color, 0);
}

#endif // GRAPHIC_LIBRARY

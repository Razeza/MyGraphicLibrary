//
// Created by dunka on 15.11.2020.
//

#ifndef GRAPH_LIB_EVENT_HPP
#define GRAPH_LIB_EVENT_HPP


#include "abstract_classes.cpp"


enum keys
{
    PAGE_UP     = VK_PRIOR,
    PAGE_DOWN   = VK_NEXT,
    ARROW_UP    = VK_UP,
    ARROW_DOWN  = VK_DOWN,
    ARROW_RIGHT = VK_RIGHT,
    ARROW_LEFT  = VK_LEFT,
    LEFT_UP = WM_LBUTTONUP
};


enum event_type
{
    KEY_CLICKED    = 69,
    BUTTON_CLICKED = 96,
    BUTTON_PRESSED = 187,
    CLOSE_WINDOW   = 59,
    PROGRAM_CLOSE  = 89,
    SCROLL_EVENT   = 84,
    NO_EVENT       = 0
};

enum Type_of_action {
    PRESSED,
    RELEASED
};




class Close_window_event: public Event
{
private:
    Abstract_window* to_close;
public:
    Abstract_window* get_window ();
    Close_window_event (Abstract_window* init_to_close);
    virtual ~Close_window_event () = default;
};

class Mouse_button_event: public Event
{
public:
    enum Mouse_button {
        NOTHING = 0,
        LEFT_BUTTON = 1,
        RIGHT_BUTTON = 2,
        MIDDLE_BUTTON
    };

    Point pos;
    Mouse_button button;
    Type_of_action action;

    Mouse_button_event (Point init_pos, Mouse_button init_button, Type_of_action init_action);
    virtual ~Mouse_button_event () = default;
};


class Keybord_event: public Event
{
public:
    keys key;
    Type_of_action action;

    Keybord_event (keys key, Type_of_action action = PRESSED);
    virtual ~Keybord_event () = default;
};



class Program_close: public Event
{
public:
    Program_close ();
    virtual ~Program_close () = default;
};

class Scroll_event: public Event
{
public:
    enum type {
        UP,
        DOWN,
        RIGHT,
        LEFT
    };

    type direction;
    double shift;
    Scroll_event (type init_direction, double init_shift);
    virtual ~Scroll_event () = default;
};


#endif //GRAPH_LIB_EVENT_HPP

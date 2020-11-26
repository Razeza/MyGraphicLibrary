//
// Created by dunka on 15.11.2020.
//

#ifndef EVENT_CPP
    #define EVENT_CPP
#include "event.hpp"


int Event::get_type ()
{
    return type;
}

Event::Event (int init_type):
        type (init_type)
{ }


Close_window_event::Close_window_event (Abstract_window* init_to_close):
        Event    (CLOSE_WINDOW),
        to_close (init_to_close)
{ }

Abstract_window* Close_window_event::get_window ()
{
    return to_close;
}

Mouse_button_event::Mouse_button_event (Point init_pos, Mouse_button init_button, Type_of_action init_action):
        Event  (BUTTON_CLICKED),
        pos    (init_pos),
        button (init_button),
        action (init_action)
{ }

Keybord_event::Keybord_event (keys init_key, Type_of_action action):
        Event (KEY_CLICKED),
        key   (init_key),
        action (action)
{ }

Program_close::Program_close ():
    Event (PROGRAM_CLOSE)
{ }

Scroll_event::Scroll_event (type init_direction, double init_shift):
    Event     (SCROLL_EVENT),
    direction (init_direction),
    shift     (init_shift)
{ }



Canvas_event::Canvas_event (Canvas_event::tools init_tool):
    Event (CANVAS_EVENT),
    tool (init_tool)
{ }

Thickness_event::Thickness_event (int plus) :
    Event (THICKNESS_EVENT),
    plus (plus)
{ }


#endif



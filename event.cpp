//
// Created by dunka on 15.11.2020.
//

#ifndef EVENT_CPP
    #define EVENT_CPP
#include "event.hpp"




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Close_window_event /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Close_window_event::Close_window_event (Abstract_window* init_to_close):
        Event    (CLOSE_WINDOW),
        to_close (init_to_close)
{ }

Abstract_window* Close_window_event::get_window ()
{
    return to_close;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Mouse_button_event /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Mouse_button_event::Mouse_button_event (Point init_pos, Mouse_button init_button, Type_of_action init_action):
        Event  (BUTTON_CLICKED),
        pos    (init_pos),
        button (init_button),
        action (init_action)
{ }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Keybord_event //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Keybord_event::Keybord_event (int init_key, Type_of_action action):
        Event (KEY_CLICKED),
        key   (init_key),
        action (action)
{ }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Program_close //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Program_close::Program_close ():
    Event (PROGRAM_CLOSE)
{ }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Scroll_event ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Scroll_event::Scroll_event (type init_direction, double init_shift):
    Event     (SCROLL_EVENT),
    direction (init_direction),
    shift     (init_shift)
{ }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Canvas_event ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Canvas_event::Canvas_event (Canvas_event::tools init_tool):
    Event (CANVAS_EVENT),
    tool (init_tool)
{ }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Thickness_event ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Thickness_event::Thickness_event (int plus) :
    Event (THICKNESS_EVENT),
    plus (plus)
{ }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Hue_event //////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Hue_event::Hue_event (float init_hue):
        Event (HUE_CHANGED),
        hue (init_hue)
{ }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Changed_color //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Changed_color::Changed_color (Color init_color):
    Event (CHANGED_COLOR),
    color (init_color)
{ }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Text_event /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Text_event::Text_event (char init_symbol):
    Event (CHAR_ENTERED),
    symbol (init_symbol)
{ }



#endif



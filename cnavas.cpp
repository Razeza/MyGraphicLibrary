//
// Created by dunka on 24.11.2020.
//
#ifndef GRAPH_LIB_CNAVAS_HPP
#define GRAPH_LIB_CNAVAS_HPP

#include <iostream>
#include "cnavas.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Abstract_tool   //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Abstract_tool::~Abstract_tool ()
{}

Paint::Paint (int init_x, int init_y, double width, double height) :
    Image (nullptr, width, height),
    memory  (width, height),
    x (init_x),
    y (init_y),
    tools_manager (thickness)
{
    set_pos ({x, y});
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class ToolManager   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ToolManager::ToolManager (const int& init_thickness):
        buttons {new Button<Pencil::Pencil_action> (Pencil::Pencil_action(), "pencil.bmp", tool_size.x, tool_size.y, space.x, space.y),
                 new Button<Eraser::Eraser_action> (Eraser::Eraser_action(), "eraser.bmp", tool_size.x, tool_size.y, space.x*2 + tool_size.x, space.y),
                 new Button<Change_thickness::Change_thickness_action> (Change_thickness::Change_thickness_action(Change_thickness::Change_thickness_action::UP),
                                                                        "scroll-bar-arrow-up.bmp", tool_size.x/2, tool_size.y/2, space.x*3 + 2*tool_size.x, space.y),
                 new Button<Change_thickness::Change_thickness_action> (Change_thickness::Change_thickness_action(Change_thickness::Change_thickness_action::DOWN),
                                                                        "scroll-bar-arrow-down.bmp", tool_size.x/2, tool_size.y/2, space.x*3 + 2*tool_size.x, space.y + tool_size.y/2),
                 new Button<Zoom::Zoom_action> (Zoom::Zoom_action (), "zoom.bmp", tool_size.x, tool_size.y, space.x*5 + tool_size.x*4, space.y)},
        tools {new Pencil(dynamic_cast<Button<Pencil::Pencil_action>*> (buttons[0])),
               new Eraser(dynamic_cast<Button<Eraser::Eraser_action>*> (buttons[1])),
               new Change_thickness(dynamic_cast<Button<Change_thickness::Change_thickness_action>*> (buttons[2])),
               new Change_thickness(dynamic_cast<Button<Change_thickness::Change_thickness_action>*> (buttons[3])),
               new Zoom (dynamic_cast<Button<Zoom::Zoom_action>*> (buttons[4]))},
        thickness (init_thickness),
        thickness_text (std::to_string (thickness))
{
    load_font ("font.ttf");
    thickness_text.set_font ();
    thickness_text.set_color (ORANGE);
    thickness_text.set_character_size (tool_size.y);
}

ToolManager::~ToolManager ()
{
    for (auto&i : buttons) {
        delete i;
    }

    for (auto& i : tools) {
        delete i;
    }
}

void ToolManager::render ()
{
    draw_rectangle (start_point.x, start_point.y, tool_manager_size.x, tool_manager_size.y, manger_color, 0);


    for (auto& i : buttons) {
        i->render ();
    }

    draw_rectangle (space.x*3 + 2.5*tool_size.x, space.y, space.x*4 + 4*tool_size.x, space.y + tool_size.y, WHITE, 0);
    thickness_text.set_str (std::to_string (thickness));
    if (thickness < 10) {
        thickness_text.set_position ({space.x*3 + 3*tool_size.x, 0});
    } else {
        thickness_text.set_position ({space.x*3 + 2.75*tool_size.x, 0});
    }
    thickness_text.render ();
}

bool ToolManager::process_event (Event* event)
{
    bool flag = false;
    for (auto& i : buttons) {
        flag = i->process_event (event);
        if (flag) {
            return true;
        }
    }

    return flag;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Helper_function   /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_line (ImageMemory& memory, Point last_pos, Point coordinates, Color color, int thickness) {

    int begin_x = std::min (static_cast<int>(coordinates.x), static_cast<int> (last_pos.x));
    int begin_y = std::min (static_cast<int>(coordinates.y), static_cast<int> (last_pos.y));

    auto end_x = std::max (static_cast<int>(coordinates.x), static_cast<int> (last_pos.x));
    auto end_y = std::max (static_cast<int>(coordinates.y), static_cast<int> (last_pos.y));

    int width = end_x - begin_x;
    int height = end_y - begin_y;


    if (width > height)
    {
        for (auto i = 0; i <= width && width != 0; i++)
        {
            int j = 0;
            if ((coordinates.y >= last_pos.y && coordinates.x >= last_pos.x) || (coordinates.y <= last_pos.y && coordinates.x <= last_pos.x))
            {
                j = memory.get_width () * (i * height / width + begin_y) + i + begin_x;
            }
            else
            {
                j = memory.get_width () * ((width - i) * height / width + begin_y) + i + begin_x;
            }
            memory.set_pixel (j, color, thickness);
        }
    }
    else
    {
        for (auto i = 0; i <= height && height != 0; i++)
        {
            int j = 0;
            if ((coordinates.y >= last_pos.y && coordinates.x >= last_pos.x) || (coordinates.y <= last_pos.y && coordinates.x <= last_pos.x))
            {
                j = memory.get_width () * (i + begin_y) + begin_x + width * i / height;
            }
            else
            {
                j = memory.get_width () * (i + begin_y) + begin_x + width - width * i / height;
            }
            memory.set_pixel (j, color, thickness);
        }

    }
}

void draw_pencil (Image* img, ImageMemory& memory, Mouse_button_event* event, Color color, Point shift, int thickness, Point& last_pos) {
    Point coordinates = {event->pos.x - shift.x, event->pos.y - shift.y};

    auto scale = img->get_scale ();
    auto start = img->get_cur_start ();
    auto pos = img->get_pos ();
    Point start_zone = {start.x, start.y};


    if (last_pos.x == -1)
    {
        coordinates = {coordinates.x / scale.x, coordinates.y / scale.y};
        memory.set_pixel (start_zone.x + (coordinates.x),
                          start_zone.y + (coordinates.y), color, thickness);
        coordinates = {coordinates.x + start_zone.x, coordinates.y + start_zone.y};
    }
    else
    {
        coordinates = {start_zone.x + coordinates.x / scale.x, start_zone.y + coordinates.y / scale.y};
        draw_line (memory, last_pos, coordinates, color, thickness);
    }

    last_pos = coordinates;
    if (event->action == Type_of_action::RELEASED) {
        last_pos = {-1, -1};
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Tool Classes   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Pencil::Pencil (Button<Pencil_action>* init_button):
        button (init_button)
{ }

void Pencil::process (Image* img, ImageMemory& memory, Mouse_button_event* event, Color color, Point shift, int thickness) {
    draw_pencil (img, memory, event, color, shift, thickness, last_pos);
}

Eraser::Eraser (Button <Eraser_action>* init_button):
        button (init_button)
{ }

void Eraser::process (Image* img, ImageMemory& memory, Mouse_button_event* event, Color color, Point shift, int thickness) {
    draw_pencil (img, memory, event, WHITE, shift, thickness, last_pos);
}


Change_thickness::Change_thickness (Button <Change_thickness_action>* init_button):
        button (init_button)
{ }

Zoom::Zoom (Button <Zoom_action>* init_button):
        button (init_button)
{ }

void Zoom::process (Image* img, ImageMemory& memory, Mouse_button_event* event, Color color, Point shift, int thickness)
{
    if (event->action != Type_of_action::RELEASED) {
        return;
    }

    if (!img->contains_point (event->pos)) {
        return;
    }

    auto prev_scale = img->get_scale ();
    double scale_up = 1.2;
    double scale_down = 0.8;

    if (event->button == Mouse_button_event::Mouse_button::LEFT_BUTTON) {
        if (scale_up*prev_scale.x > 13) {
            return;
        }
        img->scale ({scale_up, scale_up});
    } else {

        if (scale_down*prev_scale.x < 1) {
            img->scale ({1/prev_scale.x, 1/prev_scale.y});
            img->change_coordinates ({0, 0});
            img->change_size (img->get_size ());
            return;
        }
        img->scale ({scale_down, scale_down});
    }

    auto scale = img->get_scale ();
    double x = scale.x;
    double y = scale.y;

    auto [coor_x, coor_y] = event->pos;
    auto [x_start, y_start] = img->get_cur_start ();

    auto [cur_x, cur_y] = img->get_pos ();
    auto [shown_width, shown_height] = img->get_size ();
    auto [picture_width, picture_height] = img->get_full_size ();

    x_start = x_start + (coor_x - cur_x) *(1/prev_scale.x - 1/scale.x);
    y_start = y_start + (coor_y - cur_y) *(1/prev_scale.y - 1/scale.y);


    if (x_start + shown_width/scale.x > picture_width) {
        x_start = picture_width - shown_width/scale.x;
    }

    if (y_start + shown_height/scale.y > picture_height) {
        y_start = picture_height - shown_height/scale.y;
    }

    if (x_start < 0) {
        x_start = 0;
    }

    if (y_start < 0) {
        y_start = 0;
    }

    if (x*(picture_width - x_start) < shown_width) {
        x = picture_width - x_start;
    } else {
        x = shown_width / x;
    }
    if (y*(picture_height - y_start) < shown_height) {
        y = picture_height - y_start;
    } else {
        y = shown_height / y;
    }

    img->change_coordinates ({x_start, y_start});
    img->change_size ({x, y});
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Paint   //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Paint::Paint (int x_screen, int y_screen, const std::string &file_name):
    Image (file_name.c_str()),
    memory  (get_size ().x, get_size ().y),
    x ((x_screen - memory.get_width ())/2),
    y ((y_screen - memory.get_height ())/2),
    tools_manager (thickness)
{
    set_pos ({x, y});
    memory.set_with_image (this);
}

void Paint::render ()
{
    Image::draw ();
    tools_manager.render ();
}

bool Paint::process_event (Event* event)
{
    if (event->get_type () == THICKNESS_EVENT) {
        int plus = dynamic_cast<Thickness_event*> (event)->plus;
        if (thickness + plus >= 1 && thickness + plus <= max_thickness)
        {
            thickness += plus;
        }
        return true;
    }
    if (event->get_type () == CANVAS_EVENT) {
        auto new_tool = dynamic_cast<Canvas_event*> (event)->tool;
        current_tool = (current_tool == new_tool ? Canvas_event::NO_TOOL : new_tool);
        return true;
    }

    if (tools_manager.process_event (event)) {
        return true;
    }

    if (event->get_type () == BUTTON_CLICKED && current_tool != Canvas_event::NO_TOOL) {

        auto mouse_event = dynamic_cast<Mouse_button_event*> (event);
        if (contains_point (mouse_event->pos.x, mouse_event->pos.y))
        {
            tools_manager[current_tool]->process (this, memory, mouse_event, cur_color, {x, y}, thickness);
            update (memory);
            return true;
        }
    }

    return false;
}

bool Paint::clicked (double mouse_x, double mouse_y)
{
    return false;
}

Paint::~Paint ()
{ }

bool Paint::contains_point (double mouse_x, double mouse_y)
{
    return (mouse_x >= x && (x + get_size ().x > mouse_x) &&
            mouse_y >= y && (y + get_size ().y > mouse_y));
}

void Paint::hover ()
{ }







#endif //GRAPH_LIB_CNAVAS_HPP



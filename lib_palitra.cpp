//
// Created by dunka on 30.11.2020.
//
#ifndef GRAPH_LIB_LIB_PALITRA_HPP
#define GRAPH_LIB_LIB_PALITRA_HPP

#include "lib_palitra.hpp"


Palitra::Palitra (Palitra::Palitra_settings init_settings):
    settings (init_settings),
    square (settings.square_coordinates.x + settings.line_thickness,
            settings.square_coordinates.y + settings.line_thickness,
            settings.square_size.x - 2*settings.line_thickness,
            settings.square_size.y - 2*settings.line_thickness),
    line (settings.line_coordinates.x + settings.line_thickness,
          settings.line_coordinates.y + settings.line_thickness,
          settings.line_size.x - 2*settings.line_thickness,
          settings.line_size.y - 2*settings.line_thickness),
    cur_point ({square.get_pos ().x, square.get_pos ().y + square.get_size().y - std::min (settings.line_thickness, 6)}),
    slider (settings.line_coordinates + Point(settings.line_thickness, 0),
            {5, settings.line_size.y}, line.get_size().x)
{
    set_line ();
    set_square ();
}

void Palitra::render ()
{
    set_line_color (settings.line_color, settings.line_thickness);
    draw_rectangle (settings.square_coordinates, settings.square_coordinates + settings.square_size);
    draw_rectangle (settings.line_coordinates, settings.line_coordinates + settings.line_size);

    square.draw ();
    line.draw ();
    set_line_color (settings.line_color, 2);
    set_fill_color (NO_COLOR);
    draw_circle ({cur_point.x - std::min (settings.line_thickness, 6)/2, cur_point.y - std::min (settings.line_thickness, 6)/2}, std::min (settings.line_thickness, 6));
    slider.render ();
}

void Palitra::set_line ()
{
    float r = 0,
          g = 0,
          b = 0;

    float h = 0,
          s = 1,
          v = 1;

    int cur_x = 0;
    for (std::size_t i = 0; i < 360; i++) {
        h = i;
        HSVtoRGB (r, g, b, h, s, v);

        Color cur_color = {static_cast<int>(r * 255),
                           static_cast<int>(g * 255),
                           static_cast<int>(b * 255)};

        auto [width, height] = line.get_size();
        for (std::size_t j = cur_x; j < cur_x + width / 360; j++) {
            for (int k = 0; k < height; k++) {
                line.set_pixel (j, k, cur_color);
            }
        }
        cur_x += (width + 1) / 360;
    }
    line.update ();
}

void Palitra::set_square ()
{
    float r = 0;
    float g = 0;
    float b = 0;

    float h = cur_hue;
    float s = 1;
    float v = 1;

    auto [width, height] = square.get_size();

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < width; ++y) {
            s = static_cast<float>(x) / width;
            v = 1.f - static_cast<float>(y) / height;

            HSVtoRGB(r, g, b, h, s, v);

            Color cur_color = Color(r * 255, g * 255, b * 255);

            square.set_pixel (x, y, cur_color);
        }
    }
    square.update ();
}

bool Palitra::process_event (Event* event)
{
    if (event->get_type () == BUTTON_CLICKED) {
        auto x_y = dynamic_cast<Mouse_button_event*> (event)->pos;
        if (square.contains_point (x_y))
        {
            cur_point = x_y;

            cur_color = square.get_pixel (x_y.x - square.get_pos ().x,
                                          x_y.y - square.get_pos ().y);

            if (dynamic_cast<Mouse_button_event*> (event)->action == Type_of_action::RELEASED) {
                add_event (new Changed_color (cur_color));
            }
            return true;
        }

        if (line.contains_point (x_y)) {
            return slider.process_event (event);
        }
    }

    if (event->get_type () == HUE_CHANGED) {
        cur_hue = dynamic_cast<Hue_event*> (event)->hue;
        cur_color = square.get_pixel (cur_point.x - square.get_pos ().x,
                                      cur_point.y - square.get_pos ().y);
        set_square ();
        add_event (new Changed_color (cur_color));
    }
    return false;
}

Color Palitra::get_color ()
{
    return cur_color;
}

bool Palitra::contains_point (Point x_y)
{
    return line.contains_point (x_y) || square.contains_point (x_y);
}

int Palitra::get_line_thickness ()
{
    return settings.line_thickness;
}

void Palitra::Hue_slider::render ()
{
    set_fill_color (color);
    set_line_color (line_color, thickness);
    draw_rectangle ({cur_pos.x, start.y},
                    {cur_pos.x + size.x + thickness, start.y + size.y});
}

bool Palitra::Hue_slider::process_event (Event* event)
{
    if (event->get_type () == BUTTON_CLICKED) {
        auto mouse_event = dynamic_cast<Mouse_button_event*> (event);
        cur_pos = mouse_event->pos;

        add_event (new  Hue_event ((cur_pos.x - start.x) * 360 / width));
        return true;
    }
    return false;
}

Palitra::Hue_slider::Hue_slider (Point init_start, Point init_size, int init_width):
    start (init_start),
    size (init_size),
    cur_pos (init_start),
    width (init_width)
{ }






#endif //GRAPH_LIB_LIB_PALITRA_HPP




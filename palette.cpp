#ifndef GRAPH_LIB_PALETTE_HPP
#define GRAPH_LIB_PALETTE_HPP

#include "palette.hpp"

Palette::Palette (Button <Palette_action>* init_button, Palitra::Palitra_settings settings):
        palette (settings),
        cur_color (settings.square_coordinates.x + settings.square_size.x + 50 + settings.line_thickness,
                   settings.square_coordinates.y + 50 + settings.line_thickness,
                   50, 50),
        button (init_button),
        text ()
{
    load_font ("graphic/font.ttf");
    text.set_font ();
    text.set_color (palette.get_color ());
    text.set_character_size (20);
    text.set_str ("Current color:");
    text.set_position ({settings.square_coordinates.x + settings.square_size.x + 50,
                        settings.square_coordinates.y + 28});
}

bool Palette::process_event (Event* event)
{
    if (event->get_type () == OPEN_PALETTE) {
        open = true;
        return true;
    }

    if (!open) {
        return false;
    }

    if (event->get_type () == BUTTON_CLICKED) {
        auto mouse_event = dynamic_cast<Mouse_button_event*> (event);
        if (contains_point (mouse_event->pos)) {
            return palette.process_event (event);
        } else {
            if (open) {
                if (mouse_event->action == Type_of_action::RELEASED) {
                    open ^= open;
                }
                return true;
            }
        }
    }

    if (event->get_type () == HUE_CHANGED) {
        return palette.process_event (event);
    }

    return false;
}

void Palette::render ()
{
    if (!open) {
        return;
    }


    draw_rectangle (start, size, {153, 153, 255});
    if (cur_color.get_pixel (0, 0) != palette.get_color ()) {
        cur_color.memset (palette.get_color ());

        text.set_color (palette.get_color ());
    }

    palette.render ();
    int thickness = palette.get_line_thickness();
    draw_rectangle (cur_color.get_pos () - Point(thickness, thickness),
                    {cur_color.get_pos ().x + cur_color.get_size().x + thickness,
                    cur_color.get_pos ().y + cur_color.get_size().y + thickness},
                    GRAY, thickness);
    cur_color.render ();
    text.render ();
}

Palitra* Palette::get_palette ()
{
    return &palette;
}

bool Palette::contains_point (Point pos)
{
    return pos.x >= start.x && pos.x <= start.x + size.x &&
           pos.y >= start.y && pos.y <= start.y + size.y;
}

#endif //GRAPH_LIB_PALETTE_HPP

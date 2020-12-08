#ifndef GRAPH_LIB_INPUT_BOX_HPP
#define GRAPH_LIB_INPUT_BOX_HPP

#include "input_box.hpp"

void Input_box::render ()
{
    draw_circled_rectangle (settings.start, settings.size, settings.fill_color, settings.line_color, settings.thickness);

    auto [x, y] = settings.start + Point(settings.thickness, settings.thickness) + (settings.size - settings.letters_box_size) / 2;
    y = 2*(settings.size.y - 2*settings.letters_box_size.y)/3 + settings.letters_box_size.y + settings.start.y + settings.thickness;

    set_fill_color ({240, 240, 240});
    set_line_color (GRAY, settings.letter_box_thickness);
    draw_rectangle (x, y, x + settings.letters_box_size.x, y + settings.letters_box_size.y);

    answer.render ();
    question.render ();

    ok_button.render ();
    ok.render ();
}

Input_box::Input_box (const std::string& str_question, std::string& ans, Input_box::Settings settings):
    settings (settings),
    str_answer (ans),
    answer (),
    question (),
    ok_button (Stop_input (), GRAY, settings.letters_box_size.y, settings.letters_box_size.y,
               (settings.size.x - 2*settings.thickness - settings.letters_box_size.x) / 2 + settings.start.x + settings.thickness + settings.letters_box_size.x + 30,
               2*(settings.size.y - 2*settings.letters_box_size.y)/3 + settings.letters_box_size.y + settings.start.y + settings.thickness,
               {100, 100, 100}, settings.letter_box_thickness)
{
    load_font ("font.ttf");
    answer.set_font ();
    answer.set_color (BLACK);
    answer.set_character_size (settings.letters_box_size.y - 2*settings.letter_box_thickness);
    answer.set_position ({settings.start.x + settings.letter_box_thickness + settings.thickness + (settings.size.x - settings.letters_box_size.x) / 2,
                          2*(settings.size.y - 2*settings.letters_box_size.y)/3 + settings.letters_box_size.y + settings.start.y + settings.thickness - 2*settings.letter_box_thickness});
    answer.set_str ("");

    question.set_font ();
    question.set_color (BLACK);
    question.set_character_size (settings.letters_box_size.y - 2*settings.letter_box_thickness);
    question.set_position ({settings.start.x + settings.thickness + (settings.size.x - settings.letters_box_size.x) / 2,
                            (settings.size.y - 2*settings.letters_box_size.y)/3 + settings.start.y + settings.thickness});
    question.set_str (str_question);

    ok.set_font ();
    ok.set_color (BLACK);
    ok.set_character_size (settings.letters_box_size.y - 3*settings.letter_box_thickness);
    ok.set_position ({settings.start.x + settings.letters_box_size.x + 4*settings.letter_box_thickness + settings.thickness + (settings.size.x - settings.letters_box_size.x) / 2,
                      2*(settings.size.y - 2*settings.letters_box_size.y)/3 + settings.letters_box_size.y + settings.start.y + settings.thickness - 2*settings.letter_box_thickness});

    ok.set_str ("ok");
}

bool Input_box::process_event (Event* event)
{
    if (event->get_type () == KEY_CLICKED) {
        if (dynamic_cast<Keybord_event*> (event)->key == ENTER) {
            Stop_input () ();
            return true;
        }
    }

    if (ok_button.process_event (event)) {
        return true;
    }

    if (event->get_type () == CHAR_ENTERED) {
        char symbol = dynamic_cast<Text_event*> (event)->symbol;
        if (symbol == '\b') {
            if (str_answer.size () > 0) {
                str_answer.pop_back ();
                answer.set_str (str_answer);
            }
        }
        if (!isgraph (symbol)) {
            return true;
        }
        else {
            str_answer = str_answer + symbol;
        }
        if (str_answer.size () < shown_letters) {
            answer.set_str (str_answer);
        }
        else {
            answer.set_str (str_answer.substr (str_answer.size () - shown_letters, shown_letters));
        }
        return true;
    }



    return false;
}

void Input_box::run ()
{
    Window_manager manage_box ({this}, true);

    while (true) {
        if (Window_manager::CLOSE == manage_box.manage_windows ()) {
            break;
        }
        manage_box.render ();
        render_window ();
    }
}

std::string Input_box::make_input (Input_box::Settings settings, const std::string& question) {
    std::string ans = "";
    Input_box box (question, ans, settings);
    box.run ();

    return ans;
}

std::string Input_box::make_input (const std::string &question)
{
    std::string ans = "";
    Input_box box (question, ans, Input_box::Settings ());
    box.run ();

    return ans;
}

#endif //GRAPH_LIB_INPUT_BOX_HPP



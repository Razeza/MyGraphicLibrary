#include "application.hpp"

void Graphic_Functor::operator () () {
    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////   Realisation of Class Helper   ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Helper::Helper (init* data):
    data (data)
{ }


void Helper::draw (sf::RenderTarget& target, sf::RenderStates states) const {

    for (std::size_t j = 0; j < quantity_of_sorts; j++) {
        for (const auto& i : data[j].buttons) {
            target.draw (i);
        }

        for (std::size_t i = 0; i < quantity_of_sorts; i++) {
            if (!data[j].graph_to_draw[i]) {
                continue;
            }

            if (data[j].sort_graphic[i].get_array_size () == 0) {
                continue;
            }

            target.draw (data[j].sort_graphic[i]);
        }
    }
}


void Helper::button_pressed (std::size_t x, std::size_t y) {
    for (std::size_t j = 0; j < quantity_of_sorts; j++) {
        int k = 0;
        for (const auto& i : data[j].buttons) {
            if (i.is_window (x, y)) {
                if (data[j].graph_to_draw[k] == true) {
                    data[j].graph_to_draw[k++] = false;
                    continue;
                }

                data[j].graph_to_draw[k] = true;
                data[j].sort_graphic[k] = Research_graphic (Research::make_research (quantity_of_experiments, data[j].sort_functions[k], data[j].functions_names[k]), 
                                                            data[j].buttons[k].get_text_color (),
                                                            graphic_1_x, graphics_y, 
                                                            graphic_2_x, graphics_y, graphic_size - 2 * paragraph);
            }
            k++;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////   Realisation of Class Application   ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Application::draw (sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw (back,            states);
        target.draw (sort_graphic[0], states);
        target.draw (sort_graphic[1], states);
        target.draw (sort_info,       states);
        target.draw (exit_button,     states);
        
    }

#define init_help_button(j, but_x, text, text_color, text_size) Button_with_text<Graphic_Functor> (&button_texture, but_width, but_height, but_x, 800, text, font, text_color, text_size)  
Application::Application ():
    window       (sf::VideoMode (width, height), "sort analyzer"),
    back         (background_image, 0, 0, width, height),

    sort_graphic ({ Graphic (graphic_size, graphic_size, 2 * paragraph, "arr_size", "cmp" ), 
                    Graphic (graphic_size, graphic_size, 2 * paragraph, "arr_size", "swap")}),

    exit_button  (&button_texture, but_width, but_height, width/2 - but_width/2, 500, "Exit", font, sf::Color::Magenta, 75),

    data         ({ { { qsort<int, Research::functor<int>>       }, { "qsort"       }, { }, { init_help_button (1, 330,  "Qsort",  sf::Color::Green, 70) }, { false } },
                    { { bubble_sort<int, Research::functor<int>> }, { "bubble sort" }, { }, { init_help_button (2, 860,  "Bubble", sf::Color::Red,   60) }, { false } },
                    { { heapSort<int, Research::functor<int>>    }, { "heap"        }, { }, { init_help_button (3, 1390, "Heap",   sf::Color::Blue,  70) }, { false } } }),

    sort_info    (data)
{ 
    set_font ();
    set_button_texture ();

    sort_graphic[0].setPosition (x_start, y_start, init_first_graphic);
    sort_graphic[1].setPosition (width - x_start - graphic_size, y_start, init_second_graphic);

    sort_graphic[0].set_text (font, sf::Color::Black, 20);
    sort_graphic[1].set_text (font, sf::Color::Black, 20);
}
#undef init_help_button


void Application::set_button_texture () {

    if (!button_texture.loadFromFile (button_image_up)) {
        exit (ImageLoadFailed);
    }
    button_texture.setRepeated (true);
}


void Application::set_font () {

    if (!font.loadFromFile (font_path)) {
        std::cout << "Font hasn't been loaded\n";
        exit (FontLoadFailed);
    }
}

int Application::button_pressed (std::size_t x, std::size_t y) {

    sort_info.button_pressed (x, y);

    if (exit_button.is_window (x, y)) {
        return CLOSE;
    }

    return NOTHING;
}

auto Application::is_open () -> decltype (window.isOpen ()) {
    return window.isOpen ();
}

auto Application::poll_event (sf::Event& event) -> decltype (window.pollEvent (event)) {
    return window.pollEvent (event);
}

auto Application::close () -> decltype (window.close ()) {
    return window.close ();
}

auto Application::clear (const sf::Color& color) -> decltype (window.clear (color)) {
    return window.clear (color);
}

auto Application::display () -> decltype (window.display ()) {
    return window.display ();
}

void Application::draw_app ()  {
    clear (sf::Color::Black);
    window.draw (*this);
}

decltype (sf::RenderWindow())& Application::get_window () {
    return window;
}





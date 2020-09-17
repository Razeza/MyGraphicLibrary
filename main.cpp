#include <iostream>



#define DEB

#ifdef DEB
    #define DEBUG(exp) exp
    #define check() std::cout << "here" << std::endl;
#else
    #define DEBUG(exp)
    #define check() 
#endif






#include "research.h"
#include "menu.cpp"
#include "graphics.cpp"

void ShowStart ();

const char* background_image   = "graphic/blue.png";
const char* button_image_up    = "graphic/button1_big.png";
const char* button_image_down  = "graphic/button1_big.png";

const unsigned int width  = 1920;
const unsigned int height = 1080; 

const unsigned int but_width  = 200;
const unsigned int but_height = 100; 

const char* font              = "graphic/font.ttf";

const int paragraph = 60; 

const int x_start = 40;
const int y_start = 40;

const int graphic_size = 700;

int main () {
    ShowStart ();
}


struct helper: sf::Drawable {
    using sort_function_type  = decltype (qsort<int, Research::functor<int>>);

    static constexpr std::size_t quantity_of_sorts = 1;
    static constexpr std::size_t quantity_of_experiments = 100;
    
    static constexpr std::size_t graphic_1_x = x_start + paragraph;
    static constexpr std::size_t graphic_2_x = width - graphic_size - x_start + paragraph;
    
    static constexpr std::size_t graphics_y  = graphic_size + y_start - paragraph;
    

    struct init {
        sort_function_type*      sort_functions [quantity_of_sorts];
        std::string              functions_names[quantity_of_sorts];
        Research_graphic         sort_graphic   [quantity_of_sorts];
        Button_with_text         buttons        [quantity_of_sorts];
        bool                     graph_to_draw  [quantity_of_sorts]; 
    };

    init& data;

    helper (init& data):
        data (data)
    { }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {

        for (const auto& i : data.buttons) {
            target.draw (i);
        }

        for (std::size_t i = 0; i < quantity_of_sorts; i++) {
            if (!data.graph_to_draw[i]) {
                continue;
            }

            if (data.sort_graphic[i].get_array_size () == 0) {

            }

            target.draw (data.sort_graphic[i]);
        }

    }

public:

    void button_pressed (std::size_t x, std::size_t y) {
        int k = 0;
        for (const auto& i : data.buttons) {
            if (i.is_button (x, y)) {
                if (data.graph_to_draw[k] == true) {
                    data.graph_to_draw[k++] = false;
                    continue;
                }

                data.graph_to_draw[k] = true;
                data.sort_graphic[k] = Research_graphic (make_research (quantity_of_experiments, data.sort_functions[k], data.functions_names[k]), 
                                                         data.buttons[k].get_text_color (),
                                                         helper::graphic_1_x, helper::graphics_y, 
                                                         helper::graphic_2_x, helper::graphics_y, graphic_size - 2 * paragraph);
            }
            k++;
        }
    }
};

void ShowStart ()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "sort analyzer");


    Sprite back   (background_image, 0, 0, width, height);
    Button_with_text exit_button (button_image_up, but_width, but_height, width/2 - but_width/2, 500, "Exit", font, sf::Color::Magenta, 100);

    Graphic sort_graphic[2] = {Graphic (graphic_size, graphic_size, 2 * paragraph, "arr_size", "cmp"), 
                               Graphic (graphic_size, graphic_size, 2 * paragraph, "arr_size", "swap")};

    sf::Vector2f init_first_graphic[2] = { sf::Vector2f (graphic_size - x_start, graphic_size - y_start + 20),
                                           sf::Vector2f (x_start, y_start + paragraph / 2 + 20) };

    sf::Vector2f init_second_graphic[2] = { sf::Vector2f (width - x_start - paragraph / 2 - 50, graphic_size - y_start + 20),
                                            sf::Vector2f (width - graphic_size - x_start, y_start + paragraph / 2 + 20) };

    sort_graphic[0].setPosition (x_start, y_start, init_first_graphic);
    sort_graphic[1].setPosition (width - x_start - graphic_size, y_start, init_second_graphic);

    sort_graphic[0].set_text (font, sf::Color::Black, 20);
    sort_graphic[1].set_text (font, sf::Color::Black, 20);

    auto origin1 = sort_graphic[0].getOrigin ();
    auto origin2 = sort_graphic[1].getOrigin ();
    std::cout << origin1.x << " " << origin1.y << "\n";
    std::cout << origin2.x << " " << origin2.y << "\n";
    
    // Research_graphic qsort_graph (make_research (100, qsort<int, Research::functor<int>>, "qsort"), origin1.x, origin1.y,
    //                                                                                                 origin2.x, origin2.y, graphic_size - 2 * paragraph);
    

    #define init_help_button(but_x, text, text_color, text_size) Button_with_text (button_image_up, but_width, but_height, but_x, 700, text, font, text_color, text_size)

    helper::init data = { { qsort<int, Research::functor<int>>   }, { "qsort" }, { }, { init_help_button (800, "Qsort", sf::Color::Green, 50) },  {false }  };
    helper test (data);
    #undef init_help_button

    while (window.isOpen())
    {
           
        sf::Event event;
        while (window.pollEvent (event))
        {
            if (event.type == sf::Event::Closed)
                window.close ();

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                const auto& mouse_coordinates = sf::Mouse::getPosition(window); 
                
                DEBUG (std::cout << mouse_coordinates.x << " " <<  mouse_coordinates.y << "\n";)
 
                if (exit_button.is_button (mouse_coordinates.x, mouse_coordinates.y)) {
                    window.close ();
                } 

                test.button_pressed (mouse_coordinates.x, mouse_coordinates.y);
            }
        }
        window.clear (sf::Color::Black);

        window.draw (back);   
        window.draw (exit_button);
        window.draw (sort_graphic[0]);
        window.draw (sort_graphic[1]);
        window.draw (test);
        window.display();
    }
}

// Declared for class Helper
class Application;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////   Runtime commands   ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum RUNTIME {
    CLOSE   = 0,
    NOTHING = -1,
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////   Application constants   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Constants {
    const char* font_path          = "graphic/font.ttf";
    const char* button_image_up    = "graphic/button1_big.png";
    const char* background_image   = "graphic/blue.png";

    constexpr int paragraph = 60; 

    constexpr int x_start = 40;
    constexpr int y_start = 40;

    constexpr int graphic_size = 700;

    constexpr unsigned int width  = 1920;
    constexpr unsigned int height = 1080; 

    constexpr unsigned int but_width  = 200;
    constexpr unsigned int but_height = 100; 

    constexpr std::size_t quantity_of_sorts = 3;
    constexpr std::size_t quantity_of_experiments = 100;
    
    constexpr std::size_t graphic_1_x = x_start + paragraph;
    constexpr std::size_t graphic_2_x = width - graphic_size - x_start + paragraph;
    
    constexpr std::size_t graphics_y  = graphic_size + y_start - paragraph;

    const sf::Vector2f init_first_graphic[2]  = { sf::Vector2f (graphic_size - x_start, graphic_size - y_start + 20 ),
                                                  sf::Vector2f (x_start,                y_start + paragraph / 2 + 20) };

    const sf::Vector2f init_second_graphic[2] = { sf::Vector2f (width - x_start - paragraph / 2 - 50, graphic_size - y_start + 20 ),
                                                  sf::Vector2f (width - graphic_size - x_start,       y_start + paragraph / 2 + 20) };
}


using namespace Constants;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////   Declaration of Class Helper   ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Helper: sf::Drawable {
private:

    using sort_function_type  = decltype (qsort<int, Research::functor<int>>);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////   Declaration of Class init   //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct init {
        sort_function_type*      sort_functions [quantity_of_sorts];
        std::string              functions_names[quantity_of_sorts];
        Research_graphic         sort_graphic   [quantity_of_sorts];
        Button_with_text         buttons        [quantity_of_sorts];
        bool                     graph_to_draw  [quantity_of_sorts]; 
    };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////   End of Declaration of Class init   //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    init* data;

    Helper (init* data);

    
    virtual void draw           (sf::RenderTarget& target, sf::RenderStates states) const;
            void button_pressed (std::size_t x, std::size_t y);

    friend class Application;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////   Declaration of Class Application   ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Application: public sf::Drawable {
private:
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::RenderWindow window;    
    Sprite           back;
    sf::Font         font;
    sf::Texture      button_texture;
    Graphic          sort_graphic[2];
    Button_with_text exit_button;
    Helper::init     data[quantity_of_sorts];
    Helper           sort_info;

public:

    Application ();

    void set_button_texture ();
    void set_font           ();
    int  button_pressed     (std::size_t x, std::size_t y);
    void draw_app           ();

    auto is_open            ()                 -> decltype (window.isOpen ());
    auto poll_event         (sf::Event& event) -> decltype (window.pollEvent (event));
    auto close              ()                 -> decltype (window.close ());
    auto display            ()                 -> decltype (window.display ());    

    auto clear              (const sf::Color& color = sf::Color (0, 0, 0, 255)) -> decltype (window.clear (color));

    decltype (sf::RenderWindow())& get_window ();
};




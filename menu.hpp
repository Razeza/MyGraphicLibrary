#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////   Error codes  /////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


enum Erroes {
    ImageLoadFailed = -7,
    FontLoadFailed  = -6,
    MusicLoadFailed = -5,
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////   Declaration of Class Sprite   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Sprite: public sf::Drawable {
protected:
    std::pair<sf::Sprite*, sf::Texture*> makeSpr (const char* fileName, size_t x_lu = 0, size_t y_lu = 0, 
                                                  size_t x_rd = 800,    size_t y_ld = 600);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    sf::Sprite*  sprite;
    sf::Texture* image;

    bool if_delete = true;

public:
    
    Sprite                 ();
    Sprite                 (sf::Texture* texture, size_t x_lu = 0, size_t y_lu = 0, size_t x_rd = 800, size_t y_ld = 600);
    Sprite                 (const char* fileName, size_t x_lu = 0, size_t y_lu = 0, size_t x_rd = 800, size_t y_ld = 600);
    ~Sprite                ();
    sf::Sprite* get_sprite ();
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////   Declaration of Class Button   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Button: public sf::Drawable
{
protected:

    virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;

    Sprite sprite;

    std::size_t width;
    std::size_t height;
    std::size_t x;
    std::size_t y;

public:

    Button         () = default;
    Button         (sf::Texture* texture, size_t x_size, size_t y_size, size_t x_lu = 0, size_t y_lu = 0);
    Button         (const char* fileName, size_t x_size, size_t y_size, size_t x_lu = 0, size_t y_lu = 0);
    bool is_button (std::size_t mouse_x, std::size_t mouse_y) const;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////   Declaration of Class Button_with_text   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Button_with_text: public Button  {
private:

    sf::Text   text;

    virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;

public:

    Button_with_text                () = default;
    const sf::Color& get_text_color () const;
    Button_with_text                (sf::Texture* texture, size_t x_size, size_t y_size, size_t x_lu, size_t y_lu, 
                                     const std::string& text_on_button, const sf::Font& new_font, const sf::Color& new_color, std::size_t character_size);
    Button_with_text                (const char* fileName, size_t x_size, size_t y_size, size_t x_lu, size_t y_lu, 
                                     const std::string& text_on_button, const sf::Font& new_font, const sf::Color& new_color, std::size_t character_size);
    void set_button                 (const std::string& text_on_button, const sf::Font& new_font, const sf::Color& new_color, std::size_t character_size = 0);
};






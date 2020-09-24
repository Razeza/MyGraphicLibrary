#include "menu.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////   Realisation of Class Sprite   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::draw (sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw (*sprite, states);
}

Sprite::Sprite (sf::Texture* texture, size_t x_lu, size_t y_lu, size_t x_rd, size_t y_rd):
    sprite    (new sf::Sprite),
    image     (texture),
    if_delete (false) 
{ 
    sprite->setTexture     (*image);
    sprite->setTextureRect (sf::IntRect (x_lu, y_lu, x_rd, y_rd));
}


Sprite::Sprite ():
    sprite (new sf::Sprite),
    image  (new sf::Texture)
{ }

Sprite::~Sprite () {
    delete sprite;
    if (if_delete) {
        delete image;
    }
}

sf::Sprite* Sprite::get_sprite () {
    return sprite;
}

Sprite::Sprite (const char* fileName, size_t x_lu, size_t y_lu, size_t x_rd, size_t y_ld)
{
    std::pair<sf::Sprite*, sf::Texture*> tmp = makeSpr (fileName, x_lu, y_lu, x_rd, y_ld);

    sprite = tmp.first;
    image  = tmp.second;
}

std::pair<sf::Sprite*, sf::Texture*> Sprite::makeSpr (const char* fileName, size_t x_lu, size_t y_lu, 
                                                        size_t x_rd, size_t y_rd)
{
    auto bkg = new sf::Texture;
    if (!bkg->loadFromFile (fileName))
    {
        exit (ImageLoadFailed);
    }
    bkg->setRepeated (true);

    auto back = new sf::Sprite;
    back->setTexture (*bkg);
    back->setTextureRect(sf::IntRect (x_lu, y_lu, x_rd, y_rd));

    return {back, bkg};
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////   Realisation of Class Button   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Button::draw (sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw (sprite, states);
}

bool Button::is_button (std::size_t mouse_x, std::size_t mouse_y) const {
    return (mouse_x >= x && mouse_x <= x + width) &&  
           (mouse_y >= y && mouse_y <= y + height);
}

Button::Button (sf::Texture* texture, size_t x_size, size_t y_size, size_t x_lu, size_t y_lu):
    sprite (texture, 0, 0, x_size, y_size),
    x      (x_lu),
    y      (y_lu),
    width  (x_size),
    height (y_size)
{
    sprite.get_sprite()->move(sf::Vector2f(x_lu, y_lu)); 
}

Button::Button (const char* fileName, size_t x_size, size_t y_size, size_t x_lu, size_t y_lu): 
    sprite (fileName, 0, 0, x_size, y_size),
    x      (x_lu),
    y      (y_lu),
    width  (x_size),
    height (y_size)
{
    sprite.get_sprite()->move(sf::Vector2f(x_lu, y_lu));
}   


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////   Realisation of Class Button_with_text   ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const sf::Color& Button_with_text::get_text_color () const {
    return text.getFillColor ();
}

void Button_with_text::draw (sf::RenderTarget& target, sf::RenderStates states) const {
    Button::draw (target, states);
    target.draw  (text, states);
}

Button_with_text::Button_with_text (const char* file_name, size_t x_size, size_t y_size, size_t x_lu, size_t y_lu, const std::string& text_on_button, const sf::Font& new_font, const sf::Color& new_color, std::size_t character_size):
    Button (file_name, x_size, y_size, x_lu, y_lu)
{ 
    set_button (text_on_button, new_font, new_color, character_size);
}

Button_with_text::Button_with_text (sf::Texture* texture, size_t x_size, size_t y_size, size_t x_lu, size_t y_lu, const std::string& text_on_button, const sf::Font& new_font, const sf::Color& new_color, std::size_t character_size):
    Button (texture, x_size, y_size, x_lu, y_lu)
{
    set_button (text_on_button, new_font, new_color, character_size);
}

void Button_with_text::set_button (const std::string& text_on_button, const sf::Font& font, const sf::Color& new_color, std::size_t character_size) {
    text.setFont          (font);
    text.setString        (text_on_button);
    text.setPosition      (x + 10, y + 10);
    text.setFillColor     (new_color);
    text.setCharacterSize (character_size == 0 ? Button::height : character_size);
}





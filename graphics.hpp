#include <SFML/Graphics.hpp>
#include <cmath>   

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////   Declaration of Class Arrow   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Arrow: public sf::Drawable {
private: 

    sf::RectangleShape line;
    sf::CircleShape triangle;

protected:

    virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;
    
public:

    Arrow                                             (std::size_t length, std::size_t thickness, int rotate = 0);
    inline void setPosition                           (std::size_t x, std::size_t y);
    inline const sf::Vector2f& getOrigin              () const;
    inline const sf::Vector2f& get_end_arrow_position () const;
    inline float get_width                            () const;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////   Declaration of Class Graphic   ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Graphic: public sf::Drawable {
private:

    sf::RectangleShape rectangle;
    Arrow              arrows[2];
    std::size_t        paragraph;
    sf::Text           text[2];

protected:

    virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;

public:

    Graphic                              (std::size_t x, std::size_t y, std::size_t paragraph, const std::string& init_x, const std::string& init_y);
    void setPosition                     (std::size_t x, std::size_t y, const sf::Vector2f text_position[2]);
    inline void set_text                 (const sf::Font& font, const sf::Color& color, std::size_t character_size);
    inline const sf::Vector2f& getOrigin () const;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////   Declaration of Class Points_of_graphic   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Points_of_graphic: public sf::Drawable {
public:

    sf::VertexArray points;
    std::size_t number_of_points;

    Points_of_graphic () = default;
    Points_of_graphic (const std::size_t number_of_points);

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
 
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////   Declaration of Class Research_graphic   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Research_graphic: public sf::Drawable {

public:

    Research_graphic () = default;

    Research_graphic (const Research& research, sf::Color color,
                      std::size_t add_x_swap, std::size_t add_y_swap,
                      std::size_t add_x_cmp,  std::size_t add_y_cmp, std::size_t arrow_length);

    std::size_t get_array_size () const;

private:

    Points_of_graphic cmps;
    Points_of_graphic swaps;
    std::size_t       array_size;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};



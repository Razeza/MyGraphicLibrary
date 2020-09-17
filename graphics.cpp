#include </home/razeza/Рабочий стол/sort/graphics.hpp>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////   Realisation of Class Arrow   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw (line,     states);
    target.draw (triangle, states);
}

inline float Arrow::get_width () const {
    return triangle.getRadius ();
}

inline const sf::Vector2f& Arrow::get_end_arrow_position () const {
    return triangle.getPosition ();
}


inline const sf::Vector2f& Arrow::getOrigin () const {
    return line.getPosition ();
}

Arrow::Arrow (std::size_t length, std::size_t thickness, int rotate):
    line (sf::Vector2f (length, thickness)),
    triangle (thickness * 3, 3)
{
    line.rotate           (rotate);
    triangle.rotate       (90 + rotate);
    triangle.setPosition  ( thickness * (2.5) * std::sin ( (rotate) * M_PI / 180) +  (length + 2*thickness)    * std::cos ( (rotate) * M_PI / 180), 
                           (length + 2 * thickness)    * std::sin ((rotate) * M_PI / 180)  - thickness * 2.5 * std::cos ( (rotate) * M_PI / 180));

    line.setFillColor     (sf::Color::Black);
    triangle.setFillColor (sf::Color::Black);
}

inline void Arrow::setPosition (std::size_t x, std::size_t y) {
    line.setPosition (x, y);

    const auto& triangle_point = triangle.getPosition ();

    triangle.setPosition (triangle_point.x +  x, 
                          triangle_point.y + y);
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////   Realisation of Class Graphic   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Graphic::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw (rectangle, states);
    target.draw (arrows[0], states);
    target.draw (arrows[1], states);
    target.draw (text[0],   states);
    target.draw (text[1],   states);
}

inline void Graphic::set_text (const std::string& font, const sf::Color& color, std::size_t character_size) {
    
    if (!cur_font.loadFromFile (font)) {
        exit (FontLoadFailed);
    }


    for (auto& init_text : text) {
        init_text.setFont (cur_font);
        init_text.setFillColor (color);
        init_text.setCharacterSize (character_size);
    }
}


inline const sf::Vector2f& Graphic::getOrigin () const{
    return arrows[0].getOrigin ();
}

Graphic::Graphic (std::size_t x, std::size_t y, std::size_t paragraph, const std::string& init_x, const std::string& init_y):
    rectangle (sf::Vector2f (x, y)),
    arrows    ({Arrow (x - paragraph, 2.f), Arrow (y - paragraph, 2.f, 270)}),
    paragraph (paragraph / 2)
{
    text[0].setString (init_x);
    text[1].setString (init_y);  

    DEBUG (std::cout << text[0].getString ().toAnsiString () << " " << text[1].getString ().toAnsiString ());

    int i = 0;
    for (const auto& arrow : arrows) {
       const auto& coordinates = arrow.get_end_arrow_position ();
       const auto& arrow_width = arrow.get_width ();

        text[i++].setPosition (coordinates.x + arrow_width, coordinates.y);
    }   
}

void Graphic::setPosition (std::size_t x, std::size_t y, sf::Vector2f text_position[2]) {
    rectangle.setPosition (x, y);
        
    int i = 0;
    int character_size = text[0].getCharacterSize ();
    for (auto& arrow : arrows) {
        arrow.setPosition  (x + paragraph, y + rectangle.getSize().y - paragraph);
        text[i].setPosition (text_position[i++]);
    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////   Realisation of Class Points_of_graphic   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Points_of_graphic::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw (points, states);
}
 

Points_of_graphic::Points_of_graphic (const std::size_t number_of_points):
    points (sf::LineStrip, number_of_points),
    number_of_points (number_of_points)
{ }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////   Realisation of Class Research_graphic   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::size_t Research_graphic::get_array_size () const {
    return array_size;
}


void Research_graphic::draw (sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw (cmps, states);
    target.draw (swaps, states);
}

Research_graphic::Research_graphic (const Research& research, sf::Color color,
                                    std::size_t add_x_swap, std::size_t add_y_swap,
                                    std::size_t add_x_cmp,  std::size_t add_y_cmp, std::size_t arrow_length):
    cmps       (research.number_of_experiments),
    swaps      (research.number_of_experiments),
    array_size (research.number_of_experiments) 
{
    int i = 0;
    double x_coordinate      = 0;
    double y_coordinate_cmp  = 0;
    double y_coordinate_swap = 0; 
    double x_cdh             = (double) arrow_length / research.number_of_experiments;

    for (const auto& experiment: research.get_results ()) {
        x_coordinate      =   (double) i * x_cdh;
        y_coordinate_cmp  = - (double) experiment.number_of_cmp   * arrow_length / research.max_cmp  + add_y_cmp;
        y_coordinate_swap = - (double) experiment.number_of_swaps * arrow_length / research.max_swap + add_y_swap;

        cmps.points[i].position    = sf::Vector2f (x_coordinate + add_x_cmp,  y_coordinate_cmp);
        cmps.points[i].color       = color;
        swaps.points[i].position   = sf::Vector2f (x_coordinate + add_x_swap, y_coordinate_swap);
        swaps.points[i++].color    = color;
    }
}  

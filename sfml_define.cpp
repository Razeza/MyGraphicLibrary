#ifndef SFML_CPP
#define SFML_CPP

#include <cmath>
#include "sfml_define.hpp"



Engine::Engine (): cur_font (), name_of_cur_font ()
{}


Engine::~Engine ()
{ }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////  Realisation of Event Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Mouse_button_event::Mouse_button init_button_pressed (const sf::Event& event, Type_of_action action) {
    Mouse_button_event::Mouse_button mouse_event = Mouse_button_event::MIDDLE_BUTTON;
    Point coordinates = {static_cast<double>(event.mouseButton.x),
                         static_cast<double>(event.mouseButton.y)};
    if (event.mouseButton.button == sf::Mouse::Right) {
        mouse_event = Mouse_button_event::RIGHT_BUTTON;
    } else if (event.mouseButton.button == sf::Mouse::Left) {
            mouse_event = Mouse_button_event::LEFT_BUTTON;
        }

    if (mouse_event != Mouse_button_event::MIDDLE_BUTTON) {
        add_event (new Mouse_button_event (coordinates, mouse_event, action));
    }

    return mouse_event;
}

void init_key_pressed (const sf::Event& event, Type_of_action action) {
    int i = event.key.code;
    switch (i)
    {
        case sf::Keyboard::PageUp:
        {
            i = PAGE_UP;
            break;
        }
        case sf::Keyboard::PageDown:
        {
            i = PAGE_DOWN;
            break;
        }
        case sf::Keyboard::Up:
        {
            i = ARROW_UP;
            break;
        }
        case sf::Keyboard::Down:
        {
            i = ARROW_DOWN;
            break;
        }
        case sf::Keyboard::Right:
        {
            i = ARROW_RIGHT;
            break;
        }
        case sf::Keyboard::Left:
        {
            i = ARROW_LEFT;
            break;
        }
        case sf::Keyboard::Enter:
        {
            i = ENTER;
            break;
        }
    }

    add_event (new Keybord_event (static_cast<keys>(i), action));
}


// const keys check[] = {PAGE_UP, PAGE_DOWN, ARROW_UP, ARROW_DOWN, ARROW_RIGHT, ARROW_LEFT};
const sf::Keyboard::Key check[] = {sf::Keyboard::PageUp, sf::Keyboard::PageDown, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Enter};

void store_events ()
{
    sf::Event event;

    if (xxx.pressed) {
        Point coordinates = get_mouse_coordinates ();
        add_event (new Mouse_button_event(coordinates, xxx.pressed_button, PRESSED));
    }

    while (xxx.window.pollEvent (event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
            {
                add_event (new Program_close);
                break;
            }

            case sf::Event::KeyPressed:
            {
                init_key_pressed (event, PRESSED);
                break;
            }

            case sf::Event::KeyReleased:
            {
                init_key_pressed (event, RELEASED);
                break;
            }

            case sf::Event::MouseButtonPressed:
            {
                xxx.pressed = true;
                xxx.pressed_button = init_button_pressed (event, PRESSED);
                break;
            }

            case sf::Event::MouseButtonReleased:
            {
                xxx.pressed = false;
                init_button_pressed (event, RELEASED);
                break;
            }

            case sf::Event::TextEntered:
            {
                add_event (new Text_event (event.text.unicode));
            }
        }
    }
}

void add_event (Event* new_event)
{
    xxx.queue_of_events.push (new_event);
}

Event* get_event ()
{
    auto return_value = xxx.queue_of_events.front ();
    xxx.queue_of_events.pop ();
    return return_value;
}

bool empty_queue ()
{
    return xxx.queue_of_events.empty ();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Window Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void create_window (double size_x, double size_y)
{
    xxx.window.create (sf::VideoMode(size_x, size_y), "App");
}

void render_window ()
{
    xxx.window.display ();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Shape Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_line_color (Color color, int line_thickness)
{
    if (color.red == -2)
    {
        xxx.cur_line_color = sf::Color::Transparent;
    }
    else
    {
        xxx.cur_line_color = {static_cast<sf::Uint8> (color.red),
                              static_cast<sf::Uint8> (color.green),
                              static_cast<sf::Uint8> (color.blue),
                              static_cast<sf::Uint8> (color.alfa)};
    }
    xxx.thickness = line_thickness;
}

void set_fill_color (Color color)
{
    if (color.red == -2)
    {
        xxx.cur_fill_color = sf::Color::Transparent;
    }
    else
    {
        xxx.cur_fill_color = {static_cast<sf::Uint8> (color.red),
                              static_cast<sf::Uint8> (color.green),
                              static_cast<sf::Uint8> (color.blue),
                              static_cast<sf::Uint8> (color.alfa)};
    }
}

void set_line_and_fill_color (Color color, int line_thickness)
{
    set_fill_color (color);
    set_line_color (color, line_thickness);
}



void draw_line (double x0, double y0, double x1, double y1, Color color, int line_thickness)
{
    if (color.red != -1)
    {
        set_line_color (color, line_thickness);
    }

    sf::RectangleShape line (sf::Vector2f(x1 - x0, y1 - y0));
    line.setFillColor (xxx.cur_fill_color);
    line.move (x0, y0);
    line.rotate (atan ((x1 - x0) / (y1 - y0)));
    xxx.window.draw (line);
}

void draw_rectangle (double x0, double y0, double x1, double y1, Color color, int line_thickness)
{
    if (color.red != -1)
    {
        set_line_and_fill_color (color, line_thickness);
    }

    sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(x1 - x0 - 2*xxx.thickness, y1 - y0 - 2*xxx.thickness));
    rect.setPosition(sf::Vector2f (x0 + xxx.thickness, y0 + xxx.thickness));
    rect.setFillColor(xxx.cur_fill_color);
    rect.setOutlineColor (xxx.cur_line_color);
    rect.setOutlineThickness (xxx.thickness);
    xxx.window.draw(rect);
}

void draw_triangle (double x0, double y0, double x1, double y1, double x2, double y2, Color color, int line_thickness) // fix accurancy
{
    if (color.red != -1)
    {
        set_line_and_fill_color (color, line_thickness);
    }

    double side_size = sqrt ((x1 - x0 - 2*line_thickness)*(x1- x0 - 2*line_thickness) + (y1 - y0 - 2*line_thickness)*(y1 - y0 - 2*line_thickness));
    sf::CircleShape rect(side_size, 3);
    rect.setPosition(sf::Vector2f (x0 + xxx.thickness, y0 + xxx.thickness));
    rect.setFillColor(xxx.cur_fill_color);
    rect.setOutlineColor (xxx.cur_line_color);
    rect.setOutlineThickness (xxx.thickness);
    xxx.window.draw(rect);
}

void draw_circle (double x0, double y0, double r, Color color, int line_thickness)
{
    if (color.red != -1)
    {
        set_line_and_fill_color (color, line_thickness);
    }

    sf::CircleShape rect(r);
    rect.setPosition(sf::Vector2f (x0 - xxx.thickness - r, y0 - xxx.thickness - r));
    rect.setFillColor(xxx.cur_fill_color);
    rect.setOutlineColor (xxx.cur_line_color);
    rect.setOutlineThickness (xxx.thickness);
    xxx.window.draw(rect);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Text Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool load_font (const char* name)
{
    xxx.name_of_cur_font = name;
    return xxx.cur_font.loadFromFile (name);
}



Text::Text (const std::string &init_str)
{
    str.setFont (xxx.cur_font);
    str.setString (init_str);
    str.setFillColor (xxx.cur_fill_color);
}

void Text::set_character_size (int size)
{
    str.setCharacterSize (size);
}

void Text::set_color (Color color)
{
    str.setFillColor ({static_cast<sf::Uint8> (color.red),
                       static_cast<sf::Uint8> (color.green),
                       static_cast<sf::Uint8> (color.blue),
                       static_cast<sf::Uint8> (color.alfa)});
}

void Text::set_position (Point pos)
{
    str.setPosition (pos.x, pos.y);
}

void Text::set_font ()
{
    str.setFont (xxx.cur_font);
}

void Text::set_str (const std::string& init_str)
{
    str.setString (init_str);
}

void Text::render ()
{
    xxx.window.draw (str);
}

std::string Text::get_str ()
{
    return str.getString ();
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Image  ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



Image::Image (const char* name, double init_width = 0, double init_height = 0, double x = 0, double y = 0):
    width (init_width),
    height (init_height),
    x (x),
    y (y),
    shown_width (width),
    shown_height (height)
{
    if (name == nullptr || !image.loadFromFile (name)) {
        image.create (width, height, sf::Color::White);
    }


    if (height == 0) {
        auto size = image.getSize ();
        width = size.x;
        height = size.y;

        shown_width = size.x;
        shown_height = size.y;
    }

    full_image.loadFromImage (image);
    drawable_image.setTexture (full_image);
}


void Image::draw ()
{
    drawable_image.setPosition (x, y);
    drawable_image.setTextureRect ({static_cast<int>(x_shift), static_cast<int>(y_shift), static_cast<int>(width), static_cast<int>(height)});
    xxx.window.draw(drawable_image);
}

bool Image::exist ()
{
    return (drawable_image.getTexture ());
}

Point Image::get_size ()
{
    return {shown_width, shown_height};
}

void Image::update (ImageMemory &memory)
{
    full_image.update (memory.memory);
}

Point Image::get_cur_start ()
{
    return {static_cast<double>(drawable_image.getTextureRect ().left),
            static_cast<double>(drawable_image.getTextureRect ().top)};
}

Point Image::get_full_size ()
{
    return {static_cast<double>(image.getSize ().x),
            static_cast<double>(image.getSize ().y)};;
}

Point Image::get_scale ()
{
    auto scale = drawable_image.getScale ();
    return {scale.x, scale.y};
}

void Image::set_pos (Point pos)
{
    x  = pos.x;
    y  = pos.y;
}

const sf::Uint8* Image::data ()
{
    return image.getPixelsPtr ();
}

Point Image::get_pos ()
{
    return {x, y};
}

bool Image::contains_point (Point pos)
{
    return pos.x >= x && pos.x <= x + shown_width &&
            pos.y >= y && pos.y <= y + shown_height;
}

void Image::scale (Point scale)
{
    drawable_image.scale (scale.x, scale.y);
}

void Image::shift_coordinates (Point shift)
{
    x_shift += shift.x;
    y_shift += shift.y;
}

void Image::change_coordinates (Point change)
{
    x_shift = change.x;
    y_shift = change.y;
}

void Image::change_size (Point new_size)
{
    width = new_size.x;
    height = new_size.y;
}

void Image::save_image (const std::string &name)
{
    image = full_image.copyToImage ();
    if (!image.saveToFile (name)) {
        printf ("save is failed");
    };
}


Image load_image (const char* name, double width, double height)
{
    return Image (name, width, height);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Mouse Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Point get_mouse_coordinates ()
{
    auto coordinates = sf::Mouse::getPosition(xxx.window);
    return {static_cast<double>(coordinates.x),
            static_cast<double>(coordinates.y)};
}




// returns 0 - if nothing pressed
//         1 - if left button pressed
//         2 - if right button pressed
Mouse_button_event::Mouse_button is_clicked ()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        return Mouse_button_event::LEFT_BUTTON;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        return Mouse_button_event::RIGHT_BUTTON;
    }

    return Mouse_button_event::NOTHING;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class ImageMemory   //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ImageMemory::ImageMemory (int init_width, int init_height):
        memory (new sf::Uint8[4*init_width*init_height]),
        width (init_width),
        height (init_height)
{
    memset (memory, 255, 4*init_width*init_height);
}

void ImageMemory::set_pixel (int i, Color color, int thickness)
{
    set_pixel (i % width, i / width, color, thickness);
}

void ImageMemory::operator() (int i, Color color, int thickness)
{
    set_pixel (i, color, thickness);
}

ImageMemory::~ImageMemory ()
{
    delete [] memory;
}


void ImageMemory::operator() (int x, int y, Color color, int thickness)
{
    set_pixel (x, y, color, thickness);
}

void ImageMemory::set_pixel (int x, int y, Color color, int thickness)
{
    for (int k = std::max (y - thickness, 0); k <= std::min(y + thickness, height - 1); k++)
        for (int l = std::max(x - thickness, 0); l <= std::min(x + thickness, width - 1); l++)
        {
            if (((k - y)*(k - y) + (l - x)*(l - x)) < thickness)
            {
                memory[4 * (k * width + l) + 0] = color.red;
                memory[4 * (k * width + l) + 1] = color.green;
                memory[4 * (k * width + l) + 2] = color.blue;
                memory[4 * (k * width + l) + 3] = color.alfa;
            }
        }
}

int ImageMemory::get_width ()
{
    return width;
}

int ImageMemory::get_height ()
{
    return height;
}

void ImageMemory::set_with_image (Image* img) {
    memcpy ((sf::Uint8*) memory, img->data (), width*height*4);
}

Color ImageMemory::get_pixel (int x, int y)
{
    return {memory[4 * (y * width + x) + 0],
            memory[4 * (y * width + x) + 1],
            memory[4 * (y * width + x) + 2],
            memory[4 * (y * width + x) + 3]};
}

void ImageMemory::_memset (Color color)
{
    for (std::size_t i = 0; i < height; i++)
        for (std::size_t j = 0; j < width; j++) {
            set_pixel (j, i, color);
        }
}


#endif



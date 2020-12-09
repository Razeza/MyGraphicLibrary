#ifndef SFML_CPP
#define SFML_CPP

#include <cmath>
#include <iostream>
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
        default:
        {
            std::cout << "Unkown key";
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

void create_window (Point size)
{
    xxx.window.create (sf::VideoMode(size.x, size.y), "App");
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



void draw_line (Point start, Point end, Color color, int line_thickness)
{
    if (color.red != -1)
    {
        set_line_color (color, line_thickness);
    }

    sf::RectangleShape line (sf::Vector2f(end.x - start.x, end.y - start.y));

    line.setFillColor (xxx.cur_fill_color);

    line.move (start.x, start.y);
    line.rotate (atan ((end.x - start.x) / (end.y - start.y)));

    xxx.window.draw (line);
}

void draw_rectangle (Point start, Point end, Color color, int line_thickness)
{
    if (color.red != -1)
    {
        set_line_and_fill_color (color, line_thickness);
    }

    sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(end.x - start.x - 2*xxx.thickness, end.y - start.y - 2*xxx.thickness));

    rect.setPosition(sf::Vector2f (start.x + xxx.thickness, start.y + xxx.thickness));

    rect.setFillColor(xxx.cur_fill_color);
    rect.setOutlineColor (xxx.cur_line_color);
    rect.setOutlineThickness (xxx.thickness);

    xxx.window.draw(rect);
}


void draw_circle (Point start, double r, Color color, int line_thickness)
{
    if (color.red != -1)
    {
        set_line_and_fill_color (color, line_thickness);
    }

    sf::CircleShape rect(r);

    rect.setPosition(sf::Vector2f (start.x - xxx.thickness - r, start.y - xxx.thickness - r));

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

std::string Text::get_str () const
{
    return str.getString ();
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Image  ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



Image::Image (const char* name, Point _size = {0, 0}, Point start = {0, 0}):
    size (_size),
    start (start),
    shown_size (size)
{
    if (name == nullptr || !image.loadFromFile (name)) {
        image.create (size.x, size.y, sf::Color::White);
    } else {
        image.loadFromFile(name);
    }


    if (size.y == 0) {
        auto real_size = image.getSize ();
        size.x = real_size.x;
        size.y = real_size.y;

        shown_size = size;
    }

    full_image.loadFromImage (image);
    drawable_image.setTexture (full_image);
}


void Image::draw ()
{
    drawable_image.setPosition (start.x, start.y);
    drawable_image.setTextureRect ({static_cast<int>(shift.x), static_cast<int>(shift.y), static_cast<int>(size.x), static_cast<int>(size.y)});
    xxx.window.draw(drawable_image);
}

bool Image::exist () const
{
    return (drawable_image.getTexture ());
}

Point Image::get_size () const
{
    return shown_size;
}

void Image::update (ImageMemory &memory)
{
    full_image.update (memory.memory);
}

Point Image::get_cur_start () const
{
    return {static_cast<double>(drawable_image.getTextureRect ().left),
            static_cast<double>(drawable_image.getTextureRect ().top)};
}

Point Image::get_full_size () const
{
    if (exist()) {
        return {static_cast<double>(image.getSize ().x),
                static_cast<double>(image.getSize ().y)};
    }
    return size;
}

Point Image::get_scale () const
{
    auto scale = drawable_image.getScale ();
    return {scale.x, scale.y};
}

void Image::set_pos (Point pos)
{
    start = pos;
}

const sf::Uint8* Image::data ()
{
    return image.getPixelsPtr ();
}

Point Image::get_pos () const
{
    return start;
}

bool Image::contains_point (Point pos) const
{
    return pos.x >= start.x && pos.x <= start.x + shown_size.x &&
           pos.y >= start.y && pos.y <= start.y + shown_size.y;
}

void Image::set_scale (Point scale)
{
    drawable_image.scale (scale.x, scale.y);
}

void Image::shift_coordinates (Point plus_shift)
{
    shift = shift + plus_shift;
}

void Image::set_shift (Point change)
{
    shift = change;
}

void Image::change_size (Point new_size)
{
    size = new_size;
}

void Image::save_image (const std::string &name)
{
    image = full_image.copyToImage ();
    if (!image.saveToFile (name)) {
        printf ("save is failed");
    };
}

Point Image::get_start() const {
    return start;
}


Image load_image (const char* name, Point size)
{
    return Image (name, size);
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
///////////////////////////////////////   Realisation of Class ImageMemory   ////////////////////////////////////////////
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

Point ImageMemory::get_size() const {
    return {static_cast<double>(width), static_cast<double>(height)};
}

void ImageMemory::set_with_image (Image* img) {
    memcpy ((sf::Uint8*) memory, img->data (), width*height*4);
}

Color ImageMemory::get_pixel (int x, int y) const
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

uint8_t *ImageMemory::get_data() {
    return memory;
}


#endif



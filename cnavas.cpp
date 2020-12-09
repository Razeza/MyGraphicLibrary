//
// Created by dunka on 24.11.2020.
//
#ifndef GRAPH_LIB_CNAVAS_HPP
#define GRAPH_LIB_CNAVAS_HPP

#include <iostream>
#include "cnavas.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Abstract_tool   //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Abstract_tool::~Abstract_tool ()
{}

void Abstract_tool::set_thickness (int init_thickness)
{
    thickness = init_thickness;
}

void Abstract_tool::set_color (Color init_color)
{
    color = init_color;
}

int Abstract_tool::get_thickness ()
{
    return thickness;
}

Color Abstract_tool::get_color ()
{
    return color;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class ToolManager   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ToolManager::ToolManager (const Canvas_event::tools& init_tool, Palitra::Palitra_settings init_settings):
        buttons {new Button<Pencil::Pencil_action> (Pencil::Pencil_action(), "graphic/pencil.bmp", tool_size, space),
                 new Button<Eraser::Eraser_action> (Eraser::Eraser_action(), "graphic/eraser.bmp", tool_size, {space.x*2 + tool_size.x, space.y}),
                 new Button<Change_thickness::Change_thickness_action> (Change_thickness::Change_thickness_action(Change_thickness::Change_thickness_action::UP),
                                                                        "graphic/scroll-bar-arrow-up.bmp", tool_size/2, {space.x*3 + 2*tool_size.x, space.y}),
                 new Button<Change_thickness::Change_thickness_action> (Change_thickness::Change_thickness_action(Change_thickness::Change_thickness_action::DOWN),
                                                                        "graphic/scroll-bar-arrow-down.bmp", tool_size/2, {space.x*3 + 2*tool_size.x, space.y + tool_size.y/2}),
                 new Button<Zoom::Zoom_action> (Zoom::Zoom_action (), "graphic/zoom.bmp", tool_size, {space.x*5 + tool_size.x*4, space.y}),
                 new Button<Palette::Palette_action> (Palette::Palette_action (), "graphic/palette.bmp", tool_size, {space.x*6 + tool_size.x*5, space.y}),
                 new Button<Trash::Trash_action> (Trash::Trash_action (), "graphic/trash.bmp", tool_size, {space.x*7 + tool_size.x*6, space.y}),
                 new Button<Save::Save_action> (Save::Save_action (), "graphic/save.bmp", tool_size, {space.x*8 + tool_size.x*7, space.y})},
        tools {new Pencil(dynamic_cast<Button<Pencil::Pencil_action>*> (buttons[0])),
               new Eraser(dynamic_cast<Button<Eraser::Eraser_action>*> (buttons[1])),
               new Change_thickness(dynamic_cast<Button<Change_thickness::Change_thickness_action>*> (buttons[2])),
               new Change_thickness(dynamic_cast<Button<Change_thickness::Change_thickness_action>*> (buttons[3])),
               new Zoom (dynamic_cast<Button<Zoom::Zoom_action>*> (buttons[4])),
               new Palette (dynamic_cast<Button<Palette::Palette_action>*> (buttons[5]), init_settings),
               new Trash (dynamic_cast<Button<Trash::Trash_action>*> (buttons[6])),
               new Save (dynamic_cast<Button<Save::Save_action>*> (buttons[7]))},
        cur_tool (init_tool),
        thickness_text ()
{
    load_font ("graphic/font.ttf");
    thickness_text.set_font ();
    thickness_text.set_color (ORANGE);
    thickness_text.set_character_size (tool_size.y);
}

ToolManager::~ToolManager ()
{
    for (auto&i : buttons) {
        delete i;
    }

    for (auto& i : tools) {
        delete i;
    }

    for (auto&i : plugin_buttons) {
        delete i;
    }

    for (auto& i : plugins) {
        i->deinit ();
    }
}

void ToolManager::render ()
{
    draw_rectangle (start_point, {tool_manager_size.x, tool_manager_size.y*2}, manger_color, 0);


    for (auto& i : buttons) {
        i->render ();
    }

    draw_rectangle ({space.x*3 + 2.5*tool_size.x, space.y}, {space.x*4 + 4*tool_size.x, space.y + tool_size.y}, WHITE, 0);

    if (cur_tool != Canvas_event::NO_TOOL && cur_tool != Canvas_event::ZOOM &&
        cur_tool != Canvas_event::THICKNESS && cur_tool != Canvas_event::SHOW_THICKNESS)
    {
        auto thickness = tools[cur_tool]->get_thickness ();
        thickness_text.set_str (std::to_string (thickness));
        if (thickness < 10)
        {
            thickness_text.set_position ({space.x * 3 + 3 * tool_size.x, 0});
        }
        else
        {
            thickness_text.set_position ({space.x * 3 + 2.75 * tool_size.x, 0});
        }
        thickness_text.render ();
    }

    for (auto& i : plugin_buttons) {
        i->draw ();
    }

    dynamic_cast<Palette*> (tools[Canvas_event::PALETTE])->render ();
}

bool ToolManager::process_event (Event* event)
{
    std::size_t size = plugins.size();

    if (event->get_type () == BUTTON_CLICKED) {
        for (std::size_t i = 0; i < size; i++) {
            auto [x, y] = dynamic_cast<Mouse_button_event*> (event)->pos;
            if (plugin_buttons[i]->contains_point({x, y})) {
                if (dynamic_cast<Mouse_button_event*> (event)->action != Type_of_action::RELEASED) {
                    continue;
                }
                if (cur_plugin == plugins[i]) {
                    cur_plugin = nullptr;
                } else {
                    cur_plugin = plugins[i];
                }
                return true;
            }
        }
    }

    bool flag = dynamic_cast<Palette*> (tools[Canvas_event::PALETTE])->process_event (event);
    for (auto& i : buttons) {
        if (flag) {
            return true;
        }
        flag = i->process_event (event);
    }

    return flag;
}

PluginAPI::Plugin* load_plugin (const char* str) {

    void *handle = dlopen(str, RTLD_NOW);
    if (nullptr == handle) {
        printf("%s\n", dlerror());
        exit (2);
    }

    PluginAPI::Plugin *(*get_plugin)() =
    reinterpret_cast<PluginAPI::Plugin *(*)()>(dlsym(handle, "get_plugin"));

    if (nullptr == get_plugin) {
        printf("%s\n", dlerror());
        dlclose(handle);
        exit (3);
    }

    PluginAPI::Plugin *my_plugin = get_plugin();

    my_plugin->init();

    dlclose(handle);

    return my_plugin;
}


void ToolManager::load_plugins (const std::vector<std::string>& path) {
    for (auto& i : path) {
        plugin_buttons.push_back (new Image (("plugins/" + i + "/icon.bmp").c_str(), tool_size, {space.x*(plugin_buttons.size() + 1) + tool_size.x*plugin_buttons.size(), tool_manager_size.y + space.y}));
        plugins.push_back (load_plugin(("plugins/" + i + "/" + i + ".so").c_str()));
        plugins.back()->properties[PluginAPI::Property::TYPE::PRIMARY_COLOR].int_value = RGBA (init_color);
        plugins.back()->properties[PluginAPI::Property::TYPE::THICKNESS].int_value = init_thickness;
    }
}

PluginAPI::Plugin* ToolManager::get_cur_plugin() {
    return cur_plugin;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Helper_function   /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_line (Canvas& memory, Point last_pos, Point coordinates, Color color, int thickness) {

    int begin_x = std::min (static_cast<int>(coordinates.x), static_cast<int> (last_pos.x));
    int begin_y = std::min (static_cast<int>(coordinates.y), static_cast<int> (last_pos.y));

    auto end_x = std::max (static_cast<int>(coordinates.x), static_cast<int> (last_pos.x));
    auto end_y = std::max (static_cast<int>(coordinates.y), static_cast<int> (last_pos.y));

    int width = end_x - begin_x;
    int height = end_y - begin_y;


    auto [mem_width, mem_height] = memory.get_size ();
    if (width > height)
    {
        for (auto i = 0; i <= width && width != 0; i++)
        {
            int j = 0;
            if ((coordinates.y >= last_pos.y && coordinates.x >= last_pos.x) || (coordinates.y <= last_pos.y && coordinates.x <= last_pos.x))
            {
                j = mem_width * (i * height / width + begin_y) + i + begin_x;
            }
            else
            {
                j = mem_width * ((width - i) * height / width + begin_y) + i + begin_x;
            }
            memory.set_pixel (j, color, thickness);
        }
    }
    else
    {
        for (auto i = 0; i <= height && height != 0; i++)
        {
            int j = 0;
            if ((coordinates.y >= last_pos.y && coordinates.x >= last_pos.x) || (coordinates.y <= last_pos.y && coordinates.x <= last_pos.x))
            {
                j = mem_width * (i + begin_y) + begin_x + width * i / height;
            }
            else
            {
                j = mem_width * (i + begin_y) + begin_x + width - width * i / height;
            }
            memory.set_pixel (j, color, thickness);
        }

    }
}

void draw_pencil (Canvas& img, Mouse_button_event* event, Color color, Point shift, int thickness, Point& last_pos) {
    Point coordinates = {event->pos.x - shift.x, event->pos.y - shift.y};

    auto scale = img.get_scale ();
    auto start = img.get_cur_start ();
    auto pos = img.get_pos ();
    Point start_zone = {start.x, start.y};


    auto& memory = img;
    if (last_pos.x == -1)
    {
        coordinates = {coordinates.x / scale.x, coordinates.y / scale.y};
        memory.set_pixel (start_zone.x + (coordinates.x),
                          start_zone.y + (coordinates.y), color, thickness);
        coordinates = {coordinates.x + start_zone.x, coordinates.y + start_zone.y};
    }
    else
    {
        coordinates = {start_zone.x + coordinates.x / scale.x, start_zone.y + coordinates.y / scale.y};
        draw_line (memory, last_pos, coordinates, color, thickness);
    }

    last_pos = coordinates;
    if (event->action == Type_of_action::RELEASED) {
        last_pos = {-1, -1};
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Tool Classes   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Pencil::Pencil (Button<Pencil_action>* init_button):
        button (init_button)
{ }

void Pencil::process (Canvas& img, Mouse_button_event* event, Color color, Point shift, int thickness) {
    draw_pencil (img, event, color, shift, thickness, last_pos);
}

Eraser::Eraser (Button <Eraser_action>* init_button):
        button (init_button)
{ }

void Eraser::process (Canvas& img, Mouse_button_event* event, Color color, Point shift, int thickness) {
    draw_pencil (img, event, WHITE, shift, thickness, last_pos);
}


Change_thickness::Change_thickness (Button <Change_thickness_action>* init_button):
        button (init_button)
{ }

Zoom::Zoom (Button <Zoom_action>* init_button):
        button (init_button)
{ }

void Zoom::process (Canvas& img, Mouse_button_event* event, Color color, Point shift, int thickness)
{
    if (event->action != Type_of_action::RELEASED) {
        return;
    }

    if (!img.contains_point (event->pos)) {
        return;
    }

    auto prev_scale = img.get_scale ();
    double scale_up = 1.2;
    double scale_down = 0.8;

    if (event->button == Mouse_button_event::Mouse_button::LEFT_BUTTON) {
        if (scale_up*prev_scale.x > 13) {
            return;
        }
        img.set_scale ({scale_up, scale_up});
    } else {

        if (scale_down*prev_scale.x < 1) {
            img.set_scale ({1/prev_scale.x, 1/prev_scale.y});
            img.set_shift ({0, 0});
            img.change_size (img.get_size ());
            return;
        }
        img.set_scale ({scale_down, scale_down});
    }

    auto scale = img.get_scale ();
    double x = scale.x;
    double y = scale.y;

    auto [coor_x, coor_y] = event->pos;
    auto [x_start, y_start] = img.get_cur_start ();

    auto [cur_x, cur_y] = img.get_pos ();
    auto [shown_width, shown_height] = img.get_size ();
    auto [picture_width, picture_height] = img.get_full_size ();

    x_start = x_start + (coor_x - cur_x) *(1/prev_scale.x - 1/scale.x);
    y_start = y_start + (coor_y - cur_y) *(1/prev_scale.y - 1/scale.y);


    if (x_start + shown_width/scale.x > picture_width) {
        x_start = picture_width - shown_width/scale.x;
    }

    if (y_start + shown_height/scale.y > picture_height) {
        y_start = picture_height - shown_height/scale.y;
    }

    if (x_start < 0) {
        x_start = 0;
    }

    if (y_start < 0) {
        y_start = 0;
    }

    if (x*(picture_width - x_start) < shown_width) {
        x = picture_width - x_start;
    } else {
        x = shown_width / x;
    }
    if (y*(picture_height - y_start) < shown_height) {
        y = picture_height - y_start;
    } else {
        y = shown_height / y;
    }

    img.set_shift ({x_start, y_start});
    img.change_size ({x, y});
}

Save::Save (Button <Save_action>* init_button):
        button (init_button)
{ }

Trash::Trash (Button <Trash_action>* init_button):
    button (init_button)
{ }


Canvas::~Canvas ()
{ }

bool Canvas::clicked (double mouse_x, double mouse_y)
{
    return false;
}

bool Canvas::process_event (Event* event)
{
    return false;
}

void Canvas::render ()
{
    Image::draw ();
}

bool Canvas::contains_point (Point mouse) const
{
    auto [mouse_x, mouse_y] = mouse;
    auto [x, y] = get_pos ();

    return (mouse_x >= x && (x + get_size ().x > mouse_x) &&
            mouse_y >= y && (y + get_size ().y > mouse_y));
}

void Canvas::hover ()
{ }

Canvas::Canvas (int x_screen, int y_screen, const std::string &file_name):
    Image (file_name.c_str ()),
    memory (Image::get_size ().x, Image::get_size ().y)
{
    set_pos ({static_cast<double>((x_screen - memory.get_size ().x)/2),
              static_cast<double>((y_screen - memory.get_size ().y)/2)});
    memory.set_with_image (this);
}

Canvas::Canvas (int init_x, int init_y, double width, double height):
    Image (nullptr, {width, height}),
    memory  (width, height)
{
    set_pos ({static_cast<double>(init_x),
              static_cast<double>(init_y)});
}

void Canvas::update ()
{
    Image::update (memory);
}

void Canvas::set_pixel (int x, int y, Color color, int thickness)
{
    memory.set_pixel (x, y, color, thickness);
}

void Canvas::operator() (int x, int y, Color color, int thickness)
{
    memory.set_pixel (x, y, color, thickness);
}


void Canvas::set_pixel (int i, Color color, int thickness)
{
    memory.set_pixel (i, color, thickness);
}

void Canvas::operator() (int i, Color color, int thickness)
{
    memory.set_pixel (i, color, thickness);
}

Color Canvas::get_pixel (int x, int y)
{
    return memory.get_pixel (x, y);
}

void Canvas::memset (Color color)
{
    memory._memset (color);
    update ();
}

Point Canvas::get_coordinates(Point x_y) {
    auto scale = get_scale ();
    return {(x_y.x - get_cur_start ().x) / scale.x, (x_y.y - get_cur_start ().y) / scale.y};;
}

uint8_t *Canvas::get_data() {
    return memory.get_data();
}

Point Canvas::get_canvas_size() const {
    return memory.get_size ();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Paint   //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Paint::Paint (int init_x, int init_y, double width, double height, Palitra::Palitra_settings settings) :
    canvas (init_x, init_y, width, height),
    tools_manager (current_tool, settings)
{ }

Paint::Paint (int x_screen, int y_screen, const std::string &file_name, Palitra::Palitra_settings settings):
    canvas  (x_screen, y_screen, file_name),
    tools_manager (current_tool, settings)
{ }

void Paint::render ()
{
    canvas.render ();
    tools_manager.render ();
}

bool Paint::process_event (Event* event)
{
    if (tools_manager.get_cur_plugin() != nullptr) {
        current_tool = Canvas_event::NO_TOOL;
    }


    if (current_tool != Canvas_event::NO_TOOL && event->get_type () == CHANGED_COLOR) {
        tools_manager[current_tool]->set_color (dynamic_cast<Changed_color*> (event)->color);
        return true;
    }

    if (tools_manager.get_cur_plugin() != nullptr && event->get_type () == CHANGED_COLOR) {
        auto new_color = dynamic_cast<Changed_color*> (event)->color;
        std::cout << "Color changed" << std::endl;
        tools_manager.get_cur_plugin()->properties[PluginAPI::Property::TYPE::PRIMARY_COLOR].int_value = RGBA (new_color);
        return true;
    }

    if (tools_manager.process_event (event)) {
        return true;
    }

    if (event->get_type () == THICKNESS_EVENT) {
        int plus = dynamic_cast<Thickness_event*> (event)->plus;
        int thickness = 0;
        if (current_tool != Canvas_event::NO_TOOL) {
            thickness = tools_manager[current_tool]->get_thickness ();
        }
        if (tools_manager.get_cur_plugin() != nullptr) {
            thickness = tools_manager.get_cur_plugin()->properties[PluginAPI::Property::TYPE::THICKNESS].int_value;
        }

        if (thickness + plus >= 1 && thickness + plus <= max_thickness) {
            if (current_tool != Canvas_event::NO_TOOL) {
                tools_manager[current_tool]->set_thickness(thickness + plus);
            }
            if (tools_manager.get_cur_plugin() != nullptr) {
                tools_manager.get_cur_plugin()->properties[PluginAPI::Property::TYPE::THICKNESS].int_value = thickness + plus;
            }
        }
        return true;
    }

    if (event->get_type () == CANVAS_EVENT) {

        auto canvas_event = dynamic_cast<Canvas_event*> (event);
        if (canvas_event->tool == Canvas_event::SAVE) {
            canvas.save_image (Input_box::make_input ("Enter file name"));
            return true;
        }

        if (canvas_event->tool == Canvas_event::TRASH) {
            canvas.memset (WHITE);
            return true;
        }

        auto new_tool = canvas_event->tool;
        current_tool = (current_tool == new_tool ? Canvas_event::NO_TOOL : new_tool);
        return true;
    }


    if (event->get_type () == BUTTON_CLICKED) {
        auto mouse_event = dynamic_cast<Mouse_button_event*> (event);
        auto pos = mouse_event->pos;
        if (canvas.contains_point (pos))
        {
            if (current_tool != Canvas_event::NO_TOOL) {
                auto cur_color = tools_manager[current_tool]->get_color();
                auto thickness = tools_manager[current_tool]->get_thickness();

                tools_manager[current_tool]->process(canvas, mouse_event, cur_color, canvas.get_pos(), thickness);
            }

            auto cur_plugin = tools_manager.get_cur_plugin ();
            if (cur_plugin != nullptr) {

                auto new_pos = canvas.get_coordinates (pos);

                if (mouse_event->action == Type_of_action::PRESSED && plugin_used == false) {
                    cur_plugin->start_apply ({canvas.get_data(), static_cast<std::size_t>(canvas.get_size().y),
                                                                static_cast<std::size_t>(canvas.get_size().x)},
                                                               {static_cast<int64_t> (new_pos.x),
                                                                static_cast<int64_t> (new_pos.y)});
                    plugin_used = true;
                }

                if (mouse_event->action == Type_of_action::RELEASED && plugin_used == true) {
                    cur_plugin->stop_apply ({canvas.get_data(), static_cast<std::size_t>(canvas.get_size().y),
                                                                static_cast<std::size_t>(canvas.get_size().x)},
                                                               {static_cast<int64_t> (new_pos.x),
                                                                static_cast<int64_t> (new_pos.y)});
                    plugin_used = false;
                }

                if (mouse_event->action == Type_of_action::PRESSED && plugin_used == true) {
                    cur_plugin->apply({canvas.get_data(), static_cast<std::size_t>(canvas.get_size().y),
                                                          static_cast<std::size_t>(canvas.get_size().x)},
                                                         {static_cast<int64_t> (new_pos.x),
                                                          static_cast<int64_t> (new_pos.y)});
                }
            }
            canvas.update();
            return true;
        }
    }



    return false;
}



Paint::~Paint ()
{ }

void Paint::load_plugins (const std::vector<std::string>& path) {
    tools_manager.load_plugins (path);
}

Canvas &Paint::get_canvas ()
{
    return canvas;
}




#endif //GRAPH_LIB_CNAVAS_HPP



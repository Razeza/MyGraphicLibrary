//
// Created by dunka on 24.11.2020.
//




#include "graphic_library.cpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Abstract_tool   //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Abstract_tool {
public:
    virtual ~Abstract_tool () = 0;
    virtual void process (Image* img, ImageMemory &memory, Mouse_button_event* event, Color color, Point shift, int i) = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class ToolManager   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ToolManager: public Abstract_window, public Clickable {
private:

    static const int number_of_tools = 5;
    static const int number_of_buttons = 5;

    Point tool_size = {32, 32};
    Point space = {10, 5};

    Point start_point = {0, 0};
    Color manger_color = GREEN;

    Point tool_manager_size  = {tool_size.x*number_of_tools + space.x*(number_of_tools + 1),
                                tool_size.y + 2*space.y};

    Abstract_window* buttons[number_of_buttons];
    Abstract_tool* tools[number_of_tools];

    const int& thickness;
    Text thickness_text;
public:
    ToolManager (const int& init_thickness);

    Abstract_tool* operator [] (Canvas_event::tools i) {
        return tools[i];
    }

    virtual void render ();
    virtual bool process_event (Event* event);
    virtual bool contains_point (double mouse_x, double mouse_y) {};
    virtual void hover () {};
    virtual bool clicked (double mouse_x, double mouse_y) {};
    virtual ~ToolManager ();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Class Abstract_tool   //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class Pencil: public Abstract_tool {
private:
    friend class ToolManager;

    struct Pencil_action {
        void operator () () {
            add_event (new Canvas_event(Canvas_event::PENCIL));
        }
    };

    Point last_pos = {-1, -1};

public:
    Pencil (Button<Pencil_action>* init_button);

    Button<Pencil_action>* button;

    virtual ~Pencil () = default;
    
    virtual void process (Image* img, ImageMemory& memory, Mouse_button_event* event, Color color, Point shift, int thickness) override;
};

class Eraser: public Abstract_tool {
private:
    friend class ToolManager;

    struct Eraser_action {
        void operator () () {
            add_event (new Canvas_event(Canvas_event::ERASER));
        }
    };

    Point last_pos = {-1, -1};

public:
    Eraser (Button<Eraser_action>* init_button);

    Button<Eraser_action>* button;

    virtual ~Eraser () = default;

    virtual void process (Image* img, ImageMemory& memory, Mouse_button_event* event, Color color, Point shift, int thickness) override;
};

class Change_thickness: public Abstract_tool {
private:
    friend class ToolManager;

    struct Change_thickness_action {
    public:
        enum direction {
            UP = true,
            DOWN = false
        };
    private:
        direction up;
    public:
        Change_thickness_action (direction dir):
                up (dir)
        { }

        void operator () () {
            add_event (new Thickness_event (up == UP ? 1 : -1));
        }
    };

public:
    Change_thickness (Button<Change_thickness_action>* init_button);

    Button<Change_thickness_action>* button;

    virtual ~Change_thickness () = default;

    virtual void process (Image* img, ImageMemory& memory, Mouse_button_event* event, Color color, Point shift, int thickness)
    { }
};

class Zoom: public Abstract_tool {
private:
    friend class ToolManager;

    struct Zoom_action {
        void operator () () {
            add_event (new Canvas_event(Canvas_event::ZOOM));
        }
    };
public:
    Zoom (Button<Zoom_action>* init_button);

    Button<Zoom_action>* button;

    virtual ~Zoom () = default;

    virtual void process (Image* img, ImageMemory& memory, Mouse_button_event* event, Color color, Point shift, int thickness) override;
};

class Paint: public Image, public Clickable, public Abstract_window
{
private:
    Color cur_color = RED;
    int thickness = 10;

    const int max_thickness = 50;

    ImageMemory memory;
    ToolManager tools_manager;


    Canvas_event::tools current_tool = Canvas_event::NO_TOOL;

    int x;
    int y;



public:
    Paint (int x, int y, const std::string& file_name);
    Paint (int x, int y, double width, double height);

    virtual bool contains_point (double mouse_x, double mouse_y) override;
    virtual void hover () override;
    virtual void render () override ;
    virtual bool process_event (Event* event) override;
    virtual bool clicked (double mouse_x, double mouse_y) override ;
    virtual ~Paint () override;
};





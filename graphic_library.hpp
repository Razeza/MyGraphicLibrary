#ifndef GRAPH_LIB_GRAPHIC_LIBRARY_H
#define GRAPH_LIB_GRAPHIC_LIBRARY_H



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Abstract_window   ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Abstract_window
{
public:
    virtual void render () = 0;
    virtual bool process_event (const Event& event) = 0;
    virtual ~Abstract_window () {};
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Background   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Background: public Abstract_window
{
private:
    Image back_img;
    Color color;

protected:
    double x;
    double y;

    double x_shift = 0;
    double y_shift = 0;

    double width;
    double height;

    int thickness;
    Color line_color;

    double real_width;
    double real_height;

    Point get_size ();

public:
    Background () = default;
    Background (const char* name, double init_width, double init_height, double init_x = 0, double init_y = 0);
    Background (Color init_color, double init_width, double init_height, double init_x = 0, double init_y = 0, Color line_color = NO_COLOR, int thickness = 0);
    Background (const char* name, double init_width, double init_height, double init_x, double init_y, double init_real_width, double init_real_height);
    Background (Color init_color, double init_width, double init_height, double init_x, double init_y, double init_real_width, double init_real_height, Color line_color = NO_COLOR, int thickness = 0);

    void change_coordinates (Point shift)
    {
        x_shift += shift.x;
        y_shift += shift.y;
    }

    virtual bool process_event (const Event& event) override;
    virtual void render        ()                   override;

    virtual ~Background () { }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Hoverable   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Hoverable
{
public:
    virtual bool contains_point (double mouse_x, double mouse_y) = 0;
    virtual void hover () = 0;
    virtual ~Hoverable () {};
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Clickable   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Clickable: public Hoverable
{
public:
    virtual bool clicked (double mouse_x, double mouse_y) = 0;
    virtual ~Clickable () {};
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Button   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Action>
class Button: public Background, public Clickable
{
private:
    Action action;
    int thickness;
public:

    Button() = default;

    Button (Action action_init, const char* name, double init_width, double init_height, double x_init, double y_init);
    Button (Action action_init, Color color     , double init_width, double init_height, double x_init, double y_init, Color line_color = NO_COLOR, int thickness = 0);

    virtual bool contains_point (double mouse_x, double mouse_y) override;
    virtual void hover    () override;
    virtual bool clicked  (double mouse_x, double mouse_y) override;
    virtual void render   () override;
    virtual bool process_event (const Event& event) override;

    virtual ~Button () { }

    void change_action (Action new_acton);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Key_functor   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Key_functor
{
    Key_functor () = default;
    Key_functor (keys init_key):
            key (init_key)
    { }

    int key;
    void operator () ()
    {
        Event::Type_event init;
        init.event2 = {KEY_CLICKED, key};
        add_event (init);
    }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Window   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Window: public Background, public Clickable
{
private:
    struct Close_functor
    {
        Close_functor (void* init):
                window (init)
        { }

        void* window;
        void operator () ()
        {
            Event::Type_event init;
            init.event4 = Event::Close_window (CLOSE_WINDOW, window);
            add_event (init);
        }
    };

    Button<Close_functor> close_window;

protected:
    double width;
    double height;
    double x;
    double y;

    double frame_width;
    double frame_height;



public:
    Window (const char* name,
            double init_width,
            double init_height,
            double init_x,
            double init_y,
            double init_frame_width,
            double init_frame_height,
            double button_width,
            double init_real_width,
            double init_real_height);

    Window (Color color,
            double init_width,
            double init_height,
            double init_x,
            double init_y,
            double init_frame_width,
            double init_frame_height,
            double button_width,
            double init_real_width,
            double init_real_height);


    virtual bool contains_point (double mouse_x, double mouse_y) override;

    virtual void hover    () override;

    virtual bool clicked  (double mouse_x, double mouse_y) override;

    virtual void render   () override;

    virtual bool process_event (const Event& event) override;

    virtual ~Window ();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Window_manager   /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Window_manager: public Abstract_window
{
protected:
    std::vector<Abstract_window*> windows;
    bool top_manager;
public:


    Window_manager (std::vector<Abstract_window*> init_windows, bool init_top_manager);

    void manage_windows ();

    virtual bool process_event (const Event& event) override;

    virtual void render () override ;
};

class Scrollbar: public Abstract_window, public Clickable
{
private:
    double x;
    double y;
    double width;
    double height;

    Point real_size;
    char what;

    struct Scroller: Abstract_window
    {
        double cur_x;
        double cur_y;

        double x;
        double y;
        double width;
        double height;

        Color color;

        Scroller (double init_width, double init_height, double init_x, double init_y, Color init_color);

        virtual void render ();

        virtual bool process_event (const Event& event);
    };



    Button<Key_functor> page_up;
    Button<Key_functor> page_down;
    Button<Key_functor> rect;
    Scroller            scroller;



public:

    Scrollbar() = default;

    Scrollbar(double init_x, double init_y, double init_width, double init_height,
              double init_scroller_width, double init_scroller_height, Color init_scroller_color,
              const char* button_up, const char* button_down, char kind, Point init_real_size, Color color = {160, 160, 160});

    virtual bool contains_point (double mouse_x, double mouse_y) override;

    virtual void hover () override;

    virtual bool clicked (double mouse_x, double mouse_y) override;

    virtual void render () override;

    virtual bool process_event (const Event& event) override;

    virtual ~Scrollbar ();

    void up_down (double shift);

    void right_left (double shift);
};

enum bar
{
    Y_BAR   = 0,
    X_BAR   = 1,
    X_Y_BAR = 2
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Window_with_scrollbar   //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Window_with_scrollbar: public Window
{
private:
    Scrollbar bar[2];
    char kind_of_bar;

    double max_x;
    double max_y;
    double cur_x;
    double cur_y;


    void page_up (double shift);

    void page_left (double shift);

    void page_down (double shift);

    void page_right (double shift);

public:


    Window_with_scrollbar  (const char* name,
                            double init_width,
                            double init_height,
                            double init_real_width,         // real size of image
                            double init_real_height,        // real size of image
                            double init_x,
                            double init_y,
                            double init_frame_width,
                            double init_frame_height,
                            double button_width,
                            double bar_width,
                            const char* button_up,
                            const char* button_down,
                            char  init_kind_of_bar,
                            const char* button_left  = nullptr,
                            const char* button_right = nullptr,
                            Color bar_color = {160, 160, 160},
                            double init_scroller_width = 0,
                            double init_scroller_height = 0,
                            Color init_scroller_color   = {255, 255, 255});

    virtual bool contains_point (double mouse_x, double mouse_y) override;

    virtual void hover () override;

    virtual bool clicked  (double mouse_x, double mouse_y) override;

    virtual void render () override;

    virtual bool process_event (const Event& event) override;

    virtual ~Window_with_scrollbar ();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Exit_functor   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Exit_functor
{
    [[noreturn]] void operator () ();
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Draw Functions   ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_connected_line (std::vector<Point> points, std::size_t size, Color color = {-1, -1, -1}, int line_thickness = 10);



#endif //GRAPH_LIB_GRAPHIC_LIBRARY_H

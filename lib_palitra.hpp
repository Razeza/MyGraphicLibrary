#include "graphic_library.cpp"
#include "cnavas.cpp"
#include "color_utilities.cpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Palitra   ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Palitra: public Abstract_window
{
public:
    struct Palitra_settings {
        int line_thickness;
        Color line_color;

        Point square_coordinates;
        Point square_size;

        Point line_coordinates;
        Point line_size;
    };
private:
    struct Hue_slider: public Abstract_window {
        Point start;
        Point size;
        Point cur_pos;

        int width;

        Hue_slider (Point init_start, Point init_size, int init_width);

        Color color = {90, 90, 90};
        Color line_color = {100, 100, 100};
        int thickness = 2;

        virtual void render () override;
        virtual bool process_event (Event* event) override;
        virtual ~Hue_slider () = default;
    };

    Palitra_settings settings;

    Canvas square;
    Canvas line;

    Point cur_point;

    Hue_slider slider;

    float cur_hue = 0;
    Color cur_color = BLACK;

    void set_line ();
    void set_square ();

    bool process_button_event (Event* event);
    bool process_hue_event    (Event* event);
public:
    Palitra (Palitra_settings init_settings);

    Color get_color        () const;
    int get_line_thickness () const;

    bool contains_point (Point x_y) const;

    virtual void render        () override ;
    virtual bool process_event (Event* event) override;
    virtual ~Palitra           () = default;
};






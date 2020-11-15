#ifndef GRAPH_LIB_BASICS_H
#define GRAPH_LIB_BASICS_H


#define NO_COLOR {-2, -2, -2}
struct Color
{
    int red;
    int green;
    int blue;
};

struct Point
{
    double x;
    double y;
};




enum event_type
{
    KEY_CLICKED    = 69,
    BUTTON_CLICKED = 96,
    CLOSE_WINDOW   = 59,
    NO_EVENT       = 0
};



class Event
{
public:


    class No_event
    {
    public:

        No_event () = default;
        No_event (event_type type):
                event_type (type)
        { }

        char event_type;
    };

    class Close_window
    {
    public:
        Close_window () = default;
        Close_window (event_type type, void* init):
                event_type (type),
                window     (init)
        { }

        char event_type;
        void* window;
    };

    class Key_clicked
    {
    public:

        Key_clicked () = default;
        Key_clicked (event_type type, int init_key):
                event_type (type),
                key        (init_key)
        { }

        char event_type;
        int key;

    };

    class Mouse_clicked
    {
    public:
        Mouse_clicked () = default;
        Mouse_clicked (event_type type, char button, Point init_coor):
                event_type      (type),
                button_of_mouse (button),
                coordinates     (init_coor)
        { }


        char event_type;
        char button_of_mouse;
        Point coordinates;
    };

    union Type_event
    {
        Mouse_clicked event1;
        Key_clicked   event2;
        No_event      event3;
        Close_window  event4;
    };

    Type_event event;

    Event (Type_event init):
            event (init)
    { }
};




#endif //GRAPH_LIB_BASICS_H

//
// Created by dunka on 15.11.2020.
//

#ifndef GRAPH_LIB_ABSTRACT_WINDOW_HPP
#define GRAPH_LIB_ABSTRACT_WINDOW_HPP

class Event
{
private:
    int type;
public:
    int get_type ();

    Event () = delete; // not to be used

    Event (int type);
    Event (const Event& event) = delete;
    virtual ~Event () = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Abstract_window   ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Abstract_window
{
public:
    virtual void render () = 0;
    virtual bool process_event (Event* event) = 0;
    virtual ~Abstract_window () {};
};



#endif //GRAPH_LIB_ABSTRACT_WINDOW_HPP
//
// Created by dunka on 15.11.2020.
//

#ifndef ABSTRACT_CPP
    #define ABSTRACT_CPP

#include "abstract_classes.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Realisation of Event //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Event::get_type () const
{
    return type;
}

Event::Event (int init_type):
        type (init_type)
{ }

Event::~Event ()
{ }



#endif



#ifndef BASICS_CPP
    #define BASICS_CPP

#include "basics.hpp"

Color::Color (int init_red, int init_green, int init_blue, int init_alfa):
        red (init_red),
        green (init_green),
        blue (init_blue),
        alfa (init_alfa)
{ }

bool Color::operator!= (Color another)
{
    return red   != another.red   ||
           green != another.green ||
           blue  != another.blue  ||
           alfa  != another.alfa;
}

Point Point::operator+ (Point another)
{
    return {x + another.x, y + another.y};
}

Point Point::operator- (Point another)
{
    return {x - another.x, y - another.y};
}

Point Point::operator/ (int div)
{
    return {x / div, y / div};
}


Point::Point (double x, double y):
    x (x),
    y (y)
{ }

#endif




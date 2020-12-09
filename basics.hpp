#ifndef GRAPH_LIB_BASICS_H
#define GRAPH_LIB_BASICS_H


#include <cstdint>



struct Point
{
    Point (double x, double y);

    double x;
    double y;

    Point operator + (Point another);
    Point operator - (Point another);
    Point operator / (int div);
};

#define NO_COLOR {-2, -2, -2}
struct Color
{
    int red;
    int green;
    int blue;
    int alfa;

    bool operator != (Color another);

    Color () = default;
    Color (int init_red, int init_green, int init_blue, int init_alfa = 255);
};

#define RED {255, 0, 0}
#define PINK {255, 192, 203}
#define ORANGE {255, 165, 0}
#define YELLOW {255, 255, 0}
#define GREEN {0, 128, 0}
#define DARK_GREEN {0, 100, 0}
#define BLUE {0, 0, 255}
#define DARK_BLUE {0, 0, 139}
#define PURPLE {128, 0, 128}
#define WHITE {255, 255, 255}
#define BLACK  {0, 0,0}
#define GRAY {128, 128, 128}

uint32_t RGBA (uint8_t r, uint8_t g, uint8_t b, uint8_t a);
uint32_t RGBA (Color color);











#endif //GRAPH_LIB_BASICS_H

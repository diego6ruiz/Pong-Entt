#ifndef COMPONENTS
#define COMPONENTS

#include <iostream>
#include <SDL2/SDL.h>
#include <string.h>

struct Name
{
    std::string name;
};

struct BoxComponent
{
    int w, h;
};

struct PositionComponent
{
    int x, y;
};

struct VelocityComponent
{
    float vel_x, vel_y;
};

#endif
#pragma once

#include "gui/window.hpp"
#include <raylib.h>

class Frame
{
  public:
    Frame(Window *parent) : parent(parent) {}
    virtual void update() = 0;
    virtual void draw() = 0;

    Window *parent;

  protected:
    const int &get_width() { return parent->window_width; }
    const int &get_height() { return parent->window_height; }
};

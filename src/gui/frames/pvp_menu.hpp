#pragma once

#include "gui/frame.hpp"
#include "gui/widgets.hpp"
#include "gui/window.hpp"
#include "raylib.h"

class PvPMenuFrame : public Frame
{
  public:
    PvPMenuFrame(Window *parent);
    void update();
    void draw();
    void set_selected_option(int value);
    ~PvPMenuFrame();

  private:
    Font *font;

    Button *button_local_game, *button_network_game;
};

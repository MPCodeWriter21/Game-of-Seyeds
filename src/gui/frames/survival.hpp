#pragma once

#include "gui/frame.hpp"
#include "gui/window.hpp"
#include "raylib.h"

enum CurrentScreen
{
    CHOOSING_DIFFICULTY = 0,
    ANSWERING_QUESTION = 1,
    IN_BETWEEN_QUESTIONS = 2
};

class SurvivalFrame : private Frame
{
  public:
    SurvivalFrame(Window *parent);
    void update();
    void draw();
    ~SurvivalFrame();

  private:
    CurrentScreen current_screen;
};

#pragma once

#include "gui/frames/menu.hpp"
#include "gui/window.hpp"
#include "raylib.h"

enum CurrentFrame
{
    menu = 0,
    pvp_local = 1,
    pvp_network = 2,
    survival = 3,
    pvp_results = 4,
    survival_results = 5
};

class MainApp : public Window
{
  public:
    MainApp(
        int window_width,
        int window_height,
        bool sound = false,
        const std::string &title = "Window",
        int fps = 60
    );

    void update() override;
    void draw() override;
    ~MainApp();

  private:
    Image background_image;
    Texture2D background;
    Music background_music;

    CurrentFrame current_frame;
    MenuFrame menu_frame;
};

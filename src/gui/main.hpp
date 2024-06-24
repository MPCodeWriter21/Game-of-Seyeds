#pragma once

#include "gui/menu.hpp"
#include "gui/window.hpp"
#include "raylib.h"

enum CurrentFrame
{
    menu = 0,
    multi_player_local = 1,
    multi_player_network = 2,
    single_player = 3,
    multi_player_results = 4,
    single_player_results = 5
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

    CurrentFrame current_frame;
    MenuFrame menu_frame;
};

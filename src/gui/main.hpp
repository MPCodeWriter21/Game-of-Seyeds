#pragma once

#include "gui/frames/menu.hpp"
#include "gui/frames/survival.hpp"
#include "gui/window.hpp"
#include "raylib.h"

enum CurrentFrame
{
    menu = 0,
    pvp_menu = 1,
    pvp_local = 2,
    pvp_network = 3,
    survival = 4,
    pvp_results = 5,
    survival_results = 6
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
    void change_frame(int frame) override;
    ~MainApp();

  private:
    Image background_image;
    Texture2D background;
    Music background_music;

    CurrentFrame current_frame;
    MenuFrame menu_frame;
    SurvivalFrame survival_frame;
};

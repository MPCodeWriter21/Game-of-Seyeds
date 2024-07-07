#pragma once

#include "gui/frame.hpp"
#include "gui/widgets.hpp"
#include "gui/window.hpp"
#include "raylib.h"

class MenuFrame : private Frame
{
  public:
    MenuFrame(Window *parent);
    void update();
    void draw();
    void set_selected_option(int value);
    ~MenuFrame();

  private:
    int selected_option = 0;
    float button_height;
    Vector2 mouse_point, survival_btn_size, pvp_btn_size, exit_btn_size,
        survival_btn_pos, pvp_btn_pos, exit_btn_pos;
    Rectangle survival_btn_rect, pvp_btn_rect, exit_btn_rect;
    Image background_image;
    Texture2D background;

    Sound *menu_move_fx;
    Font *font, *bold_font;
};

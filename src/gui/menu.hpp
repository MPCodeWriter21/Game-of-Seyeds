#pragma once

#include "gui/frame.hpp"
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
    bool button_action;
    int selected_option = 0;
    float button_height;
    Vector2 mouse_point, single_player_btn_size, multi_player_btn_size, exit_btn_size,
        single_player_btn_pos, multi_player_btn_pos, exit_btn_pos;
    Rectangle single_player_btn_rect, multi_player_btn_rect, exit_btn_rect;
    Image background_image;
    Texture2D background;

    Sound *menu_move_fx;
    Font *font, *bold_font;
};

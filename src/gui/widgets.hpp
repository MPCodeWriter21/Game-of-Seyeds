#pragma once

#include "gui/window.hpp"
#include <raylib.h>

class Widget
{
  public:
    Widget(Window *parent) : parent(parent) {}
    virtual void update() = 0;
    virtual void draw() = 0;
    ~Widget() {}

    Window *parent;
};

enum ButtonState
{
    STATE_LESS = -1,
    NORMAL = 0,
    FOCUSED = 1,
    PRESSED = 2
};

class Button : Widget
{
  public:
    Button(
        Window *parent,
        const Vector2 position,
        const char *text,
        const Font *font,
        const int font_size = 0,
        const ButtonState forced_state = ButtonState::STATE_LESS,
        const unsigned int text_color_normal = 0xeff6ffff,
        const unsigned int text_color_focused = 0x78e782ff,
        const unsigned int text_color_pressed = 0xb04d5fff
    );
    void update() override;
    void draw() override;
    ~Button();

    const Vector2 position;
    const char *text;
    const Font *font;
    int font_size;
    const ButtonState forced_state;
    const unsigned int *text_color;
    Rectangle bounds;
    Texture2D *texture_npatch = nullptr;
    NPatchInfo npatch_info;
    Sound *button_click_sound = nullptr;
    ButtonState state = ButtonState::NORMAL;
    bool is_pressed = false;
};

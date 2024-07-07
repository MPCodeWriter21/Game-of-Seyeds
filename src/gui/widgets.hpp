#pragma once

#include "gui/window.hpp"
#include <raylib.h>

class Widget
{
  public:
    Widget(Window *parent) : parent(parent) {}
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual ~Widget() {}

    Window *parent;
};

enum ButtonState
{
    STATE_LESS = -1,
    NORMAL = 0,
    FOCUSED = 1,
    PRESSED = 2
};

class Button : public Widget
{
  public:
    Button(
        Window *parent,
        const Vector2 position,
        const char *text,
        const Font *font,
        const int font_size = 0,
        bool background = true,
        Vector2 padding = {5, 0},
        const ButtonState forced_state = ButtonState::STATE_LESS,
        const Color text_color_normal = {239, 246, 255, 255},
        const Color text_color_focused = {120, 231, 130, 255},
        const Color text_color_pressed = {176, 77, 95, 255}
    );
    void update() override;
    void draw() override;
    void set_text_position(const Vector2);
    const Vector2 &get_position() const { return text_position; }
    void set_position(const Vector2);
    void set_text(const std::string &);
    const std::string &get_text() const { return text; }
    const Vector2 &get_text_size() const { return text_size; }
    Vector2 get_size() const { return {bounds.width, bounds.height}; }
    const Rectangle &get_bounds() const { return bounds; }
    bool is_pressed() const { return state == ButtonState::PRESSED; }
    ~Button();

    bool pressed = false;

  private:
    Vector2 text_position, text_size;
    std::string text;
    const Font *font;
    int font_size;
    bool background = true;
    Vector2 padding;
    const ButtonState forced_state;
    const Color *text_color;
    Rectangle bounds;
    Texture2D *texture_npatch = nullptr;
    NPatchInfo npatch_info;
    Sound *button_click_sound = nullptr;
    ButtonState state = ButtonState::NORMAL;
};

#include "gui/widgets.hpp"
#include "gui/window.hpp"
#include "raylib.h"

Button::Button(
    Window *parent,
    const Vector2 text_position,
    const char *text,
    const Font *font,
    const int font_size,
    bool background,
    Vector2 padding,
    const ButtonState forced_state,
    const Color text_color_normal,
    const Color text_color_focused,
    const Color text_color_pressed
)
    : Widget(parent), text_position(text_position), text(text), font(font),
      font_size(font_size > 0 ? font_size : font->baseSize), background(background),
      padding(padding), forced_state(forced_state)
{
    text_color =
        new Color[3]{text_color_normal, text_color_focused, text_color_pressed};
    button_click_sound =
        parent->load_sound("_button_click_sound", "resources/button-click.wav");
    text_size = MeasureTextEx(*font, text, this->font_size, 1);
    bounds = Rectangle{
        text_position.x - text_size.x / 8 - padding.x,
        text_position.y - text_size.y / 4 - padding.y,
        text_size.x + text_size.x / 4 + padding.x * 2,
        text_size.y + text_size.y / 2 + padding.y * 2
    };
    texture_npatch = parent->load_texture_2d("_button_npatch", "resources/npatch.png");
    npatch_info = NPatchInfo{
        Rectangle{0, 0, 80, (float)texture_npatch->height},
        24, 24, 24, 24, 0
    };
}

void Button::update()
{
    state = (ButtonState)((forced_state >= 0) ? forced_state : 0);
    pressed = false;

    // Update control
    if ((state < 3) && (forced_state < 0))
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                state = ButtonState::PRESSED;
            else
                state = ButtonState::FOCUSED;

            if (state == ButtonState::PRESSED && IsGestureDetected(GESTURE_TAP))
            {
                pressed = true;
                PlaySound(*button_click_sound);
            }
        }
    }

    npatch_info.source.x = 80 * state;
}

void Button::draw()
{
    Vector2 text_size = MeasureTextEx(*font, text.c_str(), font_size, 1);
    if (background)
        DrawTextureNPatch(
            *texture_npatch, npatch_info, bounds, Vector2{0.0f, 0.0f}, 0.0f, WHITE
        );
    DrawTextEx(
        *font, text.c_str(),
        Vector2{
            bounds.x + bounds.width / 2 - text_size.x / 2,
            bounds.y + bounds.height / 2 - text_size.y / 2
        },
        font_size, 1, text_color[state]
    );
}

void Button::set_text_position(const Vector2 position)
{
    this->text_position = position;
    bounds.x = position.x - text_size.x / 10 - padding.x;
    bounds.y = position.y - text_size.y / 4 - padding.y;
}

void Button::set_position(const Vector2 position)
{
    this->text_position = Vector2{
        position.x + text_size.x / 10 + padding.x,
        position.y + text_size.y / 4 + padding.y
    };
    bounds.x = position.x;
    bounds.y = position.y;
}

void Button::set_text(const std::string &text)
{
    this->text = text;
    text_size = MeasureTextEx(*font, this->text.c_str(), this->font_size, 1);
    bounds = Rectangle{
        text_position.x - text_size.x / 8 - padding.x,
        text_position.y - text_size.y / 4 - padding.y,
        text_size.x + text_size.x / 4 + padding.x * 2,
        text_size.y + text_size.y / 2 + padding.y * 2
    };
}

Button::~Button() { delete[] text_color; }

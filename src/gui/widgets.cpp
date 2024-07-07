#include "gui/widgets.hpp"
#include "gui/window.hpp"
#include "raylib.h"

Button::Button(
    Window *parent,
    const Vector2 position,
    const char *text,
    const Font *font,
    const int font_size,
    const ButtonState forced_state,
    const unsigned int text_color_normal,
    const unsigned int text_color_focused,
    const unsigned int text_color_pressed
)
    : Widget(parent), position(position), text(text), font(font),
      font_size(font_size > 0 ? font_size : font->baseSize), forced_state(forced_state)
{
    text_color =
        new unsigned int[3]{text_color_normal, text_color_focused, text_color_pressed};
    button_click_sound =
        parent->load_sound("_button_click_sound", "resources/button-click.wav");
    Vector2 text_size = MeasureTextEx(*font, text, this->font_size, 1);
    bounds = Rectangle{
        position.x - text_size.x / 10, position.y - text_size.y / 4,
        text_size.x + text_size.x / 5, text_size.y + text_size.y / 2
    };
    texture_npatch = parent->load_texture_2d("_button_npatch", "resources/npatch.png");
    npatch_info = NPatchInfo{
        Rectangle{0, 0, 80, (float)texture_npatch->height},
        24, 24, 24, 24, 0
    };
}

void Button::update()
{
    int state = (forced_state >= 0) ? forced_state : 0; // NORMAL
    is_pressed = false;

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
                is_pressed = true;
                PlaySound(*button_click_sound);
            }
        }
    }

    npatch_info.source.x = 80 * state;
}

void Button::draw()
{
    Vector2 text_size = MeasureTextEx(*font, text, font_size, 1);
    DrawTextureNPatch(
        *texture_npatch, npatch_info, bounds, Vector2{0.0f, 0.0f}, 0.0f, WHITE
    );
    DrawTextEx(
        *font, text,
        Vector2{
            bounds.x + bounds.width / 2 - text_size.x / 2,
            bounds.y + bounds.height / 2 - text_size.y / 2 + 4
        },
        font_size, 1, GetColor(text_color[state])
    );
}

Button::~Button() { delete[] text_color; }

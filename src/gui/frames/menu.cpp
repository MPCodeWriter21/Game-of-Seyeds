#include "gui/frames/menu.hpp"
#include "gui/widgets.hpp"
#include "raylib.h"
#include <iostream>

static Color BLUE1 = {169, 134, 190, 255};
static Color BLUE2 = {117, 105, 171, 255};
static Color BLUE3 = {29, 39, 74, 255};
static Color BLUE4 = {78, 81, 178, 100};

static float currentGamepadAxisValue[8] = {0};
static float previousGamepadAxisValue[8] = {0};

static bool IsGamepadAxisAsButtonPressed(int axis, bool positiveAxis)
{
    bool pressed = false;

    if ((int)roundf(previousGamepadAxisValue[axis]) == 0)
    {
        if (positiveAxis && (currentGamepadAxisValue[axis] > 0.2f))
            pressed = true; // Positive axis pressed
        if (!positiveAxis && (currentGamepadAxisValue[axis] < -0.2f))
            pressed = true; // Negative axis pressed
    }

    return pressed;
}

MenuFrame::MenuFrame(Window *parent) : Frame(parent)
{
    // Load textures
    background_image = LoadImage("resources/menu-background.png");
    ImageResize(&background_image, get_width(), get_height());
    background = LoadTextureFromImage(background_image);
    UnloadImage(background_image);

    // Load fonts
    font = parent->load_font("main", "resources/m04.TTF");
    bold_font = parent->load_font("main", "resources/m04b.TTF");

    // Load sound
    menu_move_fx = parent->load_sound("menu_move_fx", "resources/menu-move.wav");

    // Define button bounds on screen
    survival_btn_size = MeasureTextEx(*font, "SURVIVAL", font->baseSize, 2);
    survival_btn_pos = Vector2{
        get_width() / 2 - survival_btn_size.x / 2,
        get_height() / 2 - survival_btn_size.y / 2
    };
    survival_btn_rect = Rectangle{
        survival_btn_pos.x - font->baseSize, survival_btn_pos.y - font->baseSize / 2,
        survival_btn_size.x + font->baseSize * 2, survival_btn_size.y + font->baseSize
    };
    pvp_btn_size = MeasureTextEx(*font, "PvP Mode", font->baseSize, 2);
    pvp_btn_pos = Vector2{
        get_width() / 2 - pvp_btn_size.x / 2,
        get_height() / 2 - pvp_btn_size.y / 2 + survival_btn_size.y + font->baseSize
    };
    pvp_btn_rect = Rectangle{
        survival_btn_rect.x, survival_btn_rect.y + pvp_btn_pos.y - survival_btn_pos.y,
        survival_btn_rect.width, pvp_btn_size.y + font->baseSize
    };
    exit_btn_size = MeasureTextEx(*font, "EXIT GAME", font->baseSize, 2);
    exit_btn_pos = Vector2{
        get_width() / 2 - exit_btn_size.x / 2,
        get_height() / 2 - exit_btn_size.y / 2 + survival_btn_size.y + font->baseSize +
            pvp_btn_size.y + font->baseSize
    };
    exit_btn_rect = Rectangle{
        survival_btn_rect.x, pvp_btn_rect.y + exit_btn_pos.y - pvp_btn_pos.y,
        survival_btn_rect.width, exit_btn_size.y + font->baseSize
    };
}

void MenuFrame::update()
{
    mouse_point = GetMousePosition();
    button_action = false;

    if (IsKeyPressed(KEY_J) || IsKeyPressed(KEY_DOWN) ||
        IsGamepadAxisAsButtonPressed(7, true))
        set_selected_option(selected_option + 1);
    else if (IsKeyPressed(KEY_K) || IsKeyPressed(KEY_UP) || IsGamepadAxisAsButtonPressed(7, false))
        set_selected_option(selected_option - 1);

    // Check button state
    if (CheckCollisionPointRec(
            mouse_point,
            Rectangle{
                survival_btn_pos.x, survival_btn_pos.y, survival_btn_size.x,
                survival_btn_size.y
            }
        ))
        set_selected_option(0);
    else if (CheckCollisionPointRec(
                 mouse_point,
                 Rectangle{pvp_btn_pos.x, pvp_btn_pos.y, pvp_btn_size.x, pvp_btn_size.y}
             ))
        set_selected_option(1);
    else if (CheckCollisionPointRec(
                 mouse_point,
                 Rectangle{
                     exit_btn_pos.x, exit_btn_pos.y, exit_btn_size.x, exit_btn_size.y
                 }
             ))
        set_selected_option(2);

    if (button_action) {}
}

void MenuFrame::draw()
{
    DrawTexture(background, 0, 0, WHITE);
    if (selected_option == 0)
    {
        DrawRectangleRec(survival_btn_rect, BLUE4);
        DrawTextEx(*bold_font, "SURVIVAL", survival_btn_pos, font->baseSize, 2, BLUE1);
        DrawTextEx(*bold_font, "PvP Mode", pvp_btn_pos, font->baseSize, 2, BLUE1);
        DrawTextEx(*bold_font, "EXIT GAME", exit_btn_pos, font->baseSize, 2, BLUE1);
        DrawTextEx(
            *font, "SURVIVAL", survival_btn_pos, font->baseSize, 2,
            (selected_option == 0) ? BLUE3 : BLUE2
        );
        DrawTextEx(
            *font, "PvP Mode", pvp_btn_pos, font->baseSize, 2,
            (selected_option == 1) ? BLUE3 : BLUE2
        );
        DrawTextEx(
            *font, "EXIT GAME", exit_btn_pos, font->baseSize, 2,
            (selected_option == 2) ? BLUE3 : BLUE2
        );
    }
    else if (selected_option == 1)
    {
        DrawRectangleRec(pvp_btn_rect, BLUE4);
        DrawTextEx(*bold_font, "SURVIVAL", survival_btn_pos, font->baseSize, 2, BLUE1);
        DrawTextEx(*bold_font, "PvP Mode", pvp_btn_pos, font->baseSize, 2, BLUE1);
        DrawTextEx(*bold_font, "EXIT GAME", exit_btn_pos, font->baseSize, 2, BLUE1);
        DrawTextEx(
            *font, "SURVIVAL", survival_btn_pos, font->baseSize, 2,
            (selected_option == 0) ? BLUE3 : BLUE2
        );
        DrawTextEx(
            *font, "PvP Mode", pvp_btn_pos, font->baseSize, 2,
            (selected_option == 1) ? BLUE3 : BLUE2
        );
        DrawTextEx(
            *font, "EXIT GAME", exit_btn_pos, font->baseSize, 2,
            (selected_option == 2) ? BLUE3 : BLUE2
        );
    }
    else if (selected_option == 2)
    {
        DrawRectangleRec(exit_btn_rect, BLUE4);
        DrawTextEx(*bold_font, "SURVIVAL", survival_btn_pos, font->baseSize, 2, BLUE1);
        DrawTextEx(*bold_font, "PvP Mode", pvp_btn_pos, font->baseSize, 2, BLUE1);
        DrawTextEx(*bold_font, "EXIT GAME", exit_btn_pos, font->baseSize, 2, BLUE1);
        DrawTextEx(
            *font, "SURVIVAL", survival_btn_pos, font->baseSize, 2,
            (selected_option == 0) ? BLUE3 : BLUE2
        );
        DrawTextEx(
            *font, "PvP Mode", pvp_btn_pos, font->baseSize, 2,
            (selected_option == 1) ? BLUE3 : BLUE2
        );
        DrawTextEx(
            *font, "EXIT GAME", exit_btn_pos, font->baseSize, 2,
            (selected_option == 2) ? BLUE3 : BLUE2
        );
    }
}

void MenuFrame::set_selected_option(int value)
{
    if (selected_option == value)
        return;

    if (value < 0)
        selected_option = 0;
    else if (value > 2)
        selected_option = 2;
    else
        selected_option = value;

    PlaySound(*menu_move_fx);
}

MenuFrame::~MenuFrame() { UnloadTexture(background); }

#include "gui/frames/pvp_menu.hpp"
#include "gui/main.hpp"
#include "gui/widgets.hpp"
#include "raylib.h"

PvPMenuFrame::PvPMenuFrame(Window *parent) : Frame(parent)
{
    font = parent->load_font("text-font", "resources/text-font.png");

    button_local_game =
        new Button(parent, {0, 0}, "Local PvP", font, font->baseSize);
    button_network_game =
        new Button(parent, {0, 0}, "PvP on the Network", font, font->baseSize);
    Vector2 button_local_game_size = button_local_game->get_size();
    Vector2 button_network_game_size = button_network_game->get_size();
    button_local_game->set_position(Vector2(
        get_width() / 2 - button_local_game_size.x / 2,
        get_height() / 2 - button_local_game_size.y - button_network_game_size.y / 2
    ));
    button_network_game->set_position(Vector2(
        get_width() / 2 - button_network_game_size.x / 2,
        get_height() / 2 - button_local_game_size.y / 2 + button_network_game_size.y
    ));
}

void PvPMenuFrame::update()
{
    button_local_game->update();
    button_network_game->update();

    if (button_local_game->pressed)
        parent->change_frame(CurrentFrame::pvp_local);
    if (button_network_game->pressed)
        parent->change_frame(CurrentFrame::pvp_network);
}

void PvPMenuFrame::draw()
{
    button_local_game->draw();
    button_network_game->draw();
}

PvPMenuFrame::~PvPMenuFrame()
{
    delete button_local_game;
    delete button_network_game;
}

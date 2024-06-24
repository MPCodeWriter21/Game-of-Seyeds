#include "gui/main.hpp"
#include "gui/menu.hpp"
#include "raylib.h"

MainApp::MainApp(
    int window_width, int window_height, bool sound, const std::string &title, int fps
)
    : Window(window_width, window_height, sound, title, fps),
      current_frame(CurrentFrame::menu), menu_frame(this)
{
    // Load textures
    background_image = LoadImage("resources/background.png");
    ImageResize(&background_image, window_width, window_height);
    background = LoadTextureFromImage(background_image);
    UnloadImage(background_image);
}

void MainApp::update()
{
    switch (current_frame)
    {
        case CurrentFrame::menu: menu_frame.update(); break;
    }
}

void MainApp::draw()
{
    ClearBackground(RAYWHITE);
    DrawTexture(background, 0, 0, WHITE);

    switch (current_frame)
    {
        case CurrentFrame::menu: menu_frame.draw(); break;
    }
}

MainApp::~MainApp()
{
    UnloadTexture(background);
}

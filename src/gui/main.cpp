#include "gui/frames/menu.hpp"
#include "gui/main.hpp"
#include "raylib.h"

MainApp::MainApp(
    int window_width, int window_height, bool sound, const std::string &title, int fps
)
    : Window(window_width, window_height, sound, title, fps),
      current_frame(CurrentFrame::menu), menu_frame(this), survival_frame(this)
{
    // Load textures
    background_image = LoadImage("resources/background.png");
    ImageResize(&background_image, window_width, window_height);
    background = LoadTextureFromImage(background_image);
    UnloadImage(background_image);
    background_music = LoadMusicStream("resources/background-music.mp3");
    SetMusicVolume(background_music, 0.4f);
    PlayMusicStream(background_music);
}

void MainApp::update()
{
    UpdateMusicStream(background_music);
    switch (current_frame)
    {
        case CurrentFrame::menu: menu_frame.update(); break;
        case CurrentFrame::survival:
            survival_frame.update();
            break;
            // case CurrentFrame::pvp_local: pvp_local_frame.update();
            // break; case CurrentFrame::pvp_network:
            //     pvp_network_frame.update();
            //     break;
            // case CurrentFrame::survival_results: survival_results_frame.update();
            // case CurrentFrame::pvp_results:
            // pvp_results_frame.update();
    }
}

void MainApp::draw()
{
    ClearBackground(RAYWHITE);
    DrawTexture(background, 0, 0, WHITE);

    switch (current_frame)
    {
        case CurrentFrame::menu: menu_frame.draw(); break;
        case CurrentFrame::survival: survival_frame.draw(); break;
    }
}

void MainApp::change_frame(int frame) { current_frame = (CurrentFrame)frame; }

MainApp::~MainApp()
{
    UnloadTexture(background);
    UnloadMusicStream(background_music);
}

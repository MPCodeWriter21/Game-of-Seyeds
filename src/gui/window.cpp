#include "gui/window.hpp"
#include <raylib.h>

Window::Window(
    int window_width, int window_height, bool sound, const std::string &title, int fps
)
    : window_width(window_width), window_height(window_height), fps(fps), audio(sound),
      title(title)
{
    InitWindow(window_width, window_height, title.c_str());

    if (sound)
        InitAudioDevice(); // Initialize audio device

    SetTargetFPS(fps);
}

Image *Window::load_image(const std::string &name, const char *path)
{
    // Return the existing pointer if the image is already loaded.
    if (image_map.find(name) != image_map.end())
        return image_map[name];
    return image_map[name] = new Image(LoadImage(path));
}

Texture2D *Window::load_texture_2d(const std::string &name, const char *path)
{
    // Return the existing pointer if the texture_2d is already loaded.
    if (texture_2d_map.find(name) != texture_2d_map.end())
        return texture_2d_map[name];
    return texture_2d_map[name] = new Texture2D(LoadTexture(path));
}

Sound *Window::load_sound(const std::string &name, const char *path)
{
    // Return the existing pointer if the sound is already loaded.
    if (sound_map.find(name) != sound_map.end())
        return sound_map[name];
    return sound_map[name] = new Sound(LoadSound(path));
}

Font *Window::load_font(const std::string &name, const char *path)
{
    // Return the existing pointer if the font is already loaded.
    if (font_map.find(name) != font_map.end())
        return font_map[name];
    return font_map[name] = new Font(LoadFont(path));
}

void Window::main_loop()
{
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        update();
        BeginDrawing();
        draw();
        EndDrawing();
    }
}

Window::~Window()
{
    // De-Initialization
    //--------------------------------------------------------------------------------------
    for (auto i = image_map.begin(); i != image_map.end(); ++i)
        UnloadImage(*i->second); // Unload images
    for (auto i = image_map.begin(); i != image_map.end(); ++i)
        delete i->second; // Unload images
    for (auto i = texture_2d_map.begin(); i != texture_2d_map.end(); ++i)
        UnloadTexture(*i->second); // Unload textures
    for (auto i = texture_2d_map.begin(); i != texture_2d_map.end(); ++i)
        delete i->second; // Unload textures
    for (auto i = sound_map.begin(); i != sound_map.end(); ++i)
        UnloadSound(*i->second); // Unload sounds
    for (auto i = sound_map.begin(); i != sound_map.end(); ++i)
        delete i->second; // Unload sounds
    for (auto i = font_map.begin(); i != font_map.end(); ++i)
        UnloadFont(*i->second); // Unload fonts
    for (auto i = font_map.begin(); i != font_map.end(); ++i)
        delete i->second; // Unload fonts

    if (audio)
        CloseAudioDevice(); // Close audio device

    CloseWindow(); // Close window and OpenGL context
}

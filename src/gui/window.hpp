#pragma once

#include <map>
#include <raylib.h>
#include <string>

class Window
{
    friend class Frame;

  public:
    Window(
        int window_width,
        int window_height,
        bool sound = false,
        const std::string &title = "Window",
        int fps = 60
    );
    Image *load_image(const std::string &name, const char *path);
    Texture2D *load_texture_2d(const std::string &name, const char *path);
    Sound *load_sound(const std::string &name, const char *path);
    Font *load_font(const std::string &name, const char *path);
    void main_loop();
    virtual void update() = 0;
    virtual void draw() = 0;
    ~Window();

  protected:
    const int window_width, window_height, fps;
    const bool audio;
    const std::string title;
    std::map<std::string, Image *> image_map;
    std::map<std::string, Texture2D *> texture_2d_map;
    std::map<std::string, Sound *> sound_map;
    std::map<std::string, Font *> font_map;
};

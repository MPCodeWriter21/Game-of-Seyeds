#include "gui/frames/survival.hpp"
#include "gui/widgets.hpp"
#include "raylib.h"

SurvivalFrame::SurvivalFrame(Window *parent) : Frame(parent)
{
    m04_font = parent->load_font("m04", "resources/m04.TTF");
    blink_sound = parent->load_sound("blink-sound", "resources/blink-sound.wav");
    Font *font = parent->load_font("_button_text_font", "resources/text-font.png");
    choosing_difficulty_easy_button = new Button{parent, Vector2(0, 0), "Easy", font};
    choosing_difficulty_medium_button =
        new Button{parent, Vector2(0, 0), "Medium", font};
    choosing_difficulty_hard_button = new Button{parent, Vector2(0, 0), "Hard", font};

    int vertical_size = choosing_difficulty_easy_button->get_size().y * 2 +
                        choosing_difficulty_medium_button->get_size().y +
                        choosing_difficulty_hard_button->get_size().y;
    Vector2 easy_pos(
        this->get_width() / 2 - choosing_difficulty_easy_button->get_size().x / 2,
        this->get_height() / 2 - vertical_size / 2
    );
    Vector2 medium_pos(
        this->get_width() / 2 - choosing_difficulty_medium_button->get_size().x / 2,
        easy_pos.y + choosing_difficulty_easy_button->get_size().y +
            choosing_difficulty_easy_button->get_size().y / 2
    );
    Vector2 hard_pos(
        this->get_width() / 2 - choosing_difficulty_hard_button->get_size().x / 2,
        medium_pos.y + choosing_difficulty_medium_button->get_size().y +
            choosing_difficulty_easy_button->get_size().y / 2
    );
    choosing_difficulty_easy_button->set_position(easy_pos);
    choosing_difficulty_medium_button->set_position(medium_pos);
    choosing_difficulty_hard_button->set_position(hard_pos);
}

void SurvivalFrame::update()
{
    switch (current_screen)
    {
        case SurvivalScreen::CHOOSING_DIFFICULTY: update_choosing_difficulty(); break;
        case SurvivalScreen::ANSWERING_QUESTION: update_answering_question(); break;
        case SurvivalScreen::IN_BETWEEN_QUESTIONS: update_in_between_questions(); break;
    }
}

void SurvivalFrame::draw()
{
    switch (current_screen)
    {
        case SurvivalScreen::CHOOSING_DIFFICULTY: draw_choosing_difficulty(); break;
        case SurvivalScreen::ANSWERING_QUESTION: draw_answering_question(); break;
        case SurvivalScreen::IN_BETWEEN_QUESTIONS: draw_in_between_questions(); break;
    }
}

void SurvivalFrame::update_choosing_difficulty()
{
    choosing_difficulty_easy_button->update();
    choosing_difficulty_medium_button->update();
    choosing_difficulty_hard_button->update();
    if (choosing_difficulty_easy_button->pressed ||
        choosing_difficulty_medium_button->pressed ||
        choosing_difficulty_hard_button->pressed)
        current_screen = SurvivalScreen::IN_BETWEEN_QUESTIONS;
    if (choosing_difficulty_easy_button->pressed)
        difficulty = "easy";
    else if (choosing_difficulty_medium_button->pressed)
        difficulty = "medium";
    else if (choosing_difficulty_hard_button->pressed)
        difficulty = "hard";
}

void SurvivalFrame::draw_choosing_difficulty()
{
    choosing_difficulty_easy_button->draw();
    choosing_difficulty_medium_button->draw();
    choosing_difficulty_hard_button->draw();
}

void SurvivalFrame::update_answering_question() {}

void SurvivalFrame::draw_answering_question() {}

void SurvivalFrame::update_in_between_questions()
{
    // Go to the ANSWERING_QUESTION screen when Enter button is pressed.
    if (IsKeyPressed(KEY_ENTER))
        current_screen = SurvivalScreen::ANSWERING_QUESTION;
}

void SurvivalFrame::draw_in_between_questions()
{
    // Draw a blinking text in the middle of the screen saying "Press enter to
    // continue..."
    static int frame_counter = 0;
    // Blink sound
    if (frame_counter == 1)
        PlaySound(*blink_sound);
    static Vector2 text_size =
        MeasureTextEx(*m04_font, "Press enter to continue...", m04_font->baseSize, 1);
    if (frame_counter < 50)
    {
        DrawTextEx(
            *m04_font, "Press enter to continue...",
            Vector2(
                this->get_width() / 2 - text_size.x / 2,
                this->get_height() / 2 - text_size.y / 2
            ),
            m04_font->baseSize, 1, RAYWHITE
        );
    }
    else if (frame_counter < 80)
    {
        DrawTextEx(
            *m04_font, "Press enter to continue...",
            Vector2(
                this->get_width() / 2 - text_size.x / 2,
                this->get_height() / 2 - text_size.y / 2
            ),
            m04_font->baseSize, 1,
            Color(0xff, 0xff, 0xff, (80 - frame_counter) / 60.0f * 0xff)
        );
    }
    if (frame_counter > 95)
        frame_counter = 0;

    frame_counter++;
}

SurvivalFrame::~SurvivalFrame()
{
    delete choosing_difficulty_easy_button;
    delete choosing_difficulty_medium_button;
    delete choosing_difficulty_hard_button;
}

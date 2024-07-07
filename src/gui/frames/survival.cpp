#include "api/opentdb.hpp"
#include "gui/frames/survival.hpp"
#include "gui/widgets.hpp"
#include "raylib.h"
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>
#include <thread>

SurvivalFrame::SurvivalFrame(Window *parent) : Frame(parent)
{
    Image question_background_image = LoadImage("resources/question-background.png");
    ImageResize(&question_background_image, get_width()-30, get_height()-30);
    question_background = LoadTextureFromImage(question_background_image);
    UnloadImage(question_background_image);

    m04_font = parent->load_font("m04", "resources/m04.TTF");
    blink_sound = parent->load_sound("blink-sound", "resources/blink-sound.wav");
    text_font = parent->load_font("text-font", "resources/text-font.png");
    choosing_difficulty_easy_button =
        new Button{parent, Vector2(0, 0), "Easy", text_font};
    choosing_difficulty_medium_button =
        new Button{parent, Vector2(0, 0), "Medium", text_font};
    choosing_difficulty_hard_button =
        new Button{parent, Vector2(0, 0), "Hard", text_font};

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

    option_buttons = new Button[4]{
        {parent, Vector2(0, 0), "Sample Text", text_font, text_font->baseSize * 3 / 5,
         false, Vector2(-1, -1), ButtonState::STATE_LESS, Color(239, 246, 255, 255),
         Color(120, 200, 130, 255), Color(120, 200, 130, 255)},
        {parent, Vector2(0, 0), "Sample Text", text_font, text_font->baseSize * 3 / 5,
         false, Vector2(-1, -1), ButtonState::STATE_LESS, Color(239, 246, 255, 255),
         Color(120, 200, 130, 255), Color(120, 200, 130, 255)},
        {parent, Vector2(0, 0), "Sample Text", text_font, text_font->baseSize * 3 / 5,
         false, Vector2(-1, -1), ButtonState::STATE_LESS, Color(239, 246, 255, 255),
         Color(120, 200, 130, 255), Color(120, 200, 130, 255)},
        {parent, Vector2(0, 0), "Sample Text", text_font, text_font->baseSize * 3 / 5,
         false, Vector2(-1, -1), ButtonState::STATE_LESS, Color(239, 246, 255, 255),
         Color(120, 200, 130, 255), Color(120, 200, 130, 255)}
    };
    Vector2 question_text_size =
        MeasureTextEx(*text_font, "Sample Text", text_font->baseSize / 2, 1);
    option_buttons[0].set_text_position({100, 150 + question_text_size.y});
    for (int i = 1; i < 4; i++)
        option_buttons[i].set_text_position(Vector2(
            100, option_buttons[i - 1].get_bounds().y +
                     option_buttons[i - 1].get_bounds().height + 50
        ));
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

void SurvivalFrame::update_answering_question()
{
    if (questions.size() - question_index < 4)
        more_questions();
    if (question_index < questions.size())
        for (int i = 0; i < 4; i++)
            option_buttons[i].update();
}

void SurvivalFrame::draw_answering_question()
{
    if (question_index < questions.size())
    {
        DrawTexture(question_background, 15, 15, Color(0xff, 0xff, 0xff, 0x77));
        DrawTextEx(
            *text_font, questions[question_index].question.c_str(), {70, 70},
            text_font->baseSize / 2, 1, RAYWHITE
        );
        int correct_option = 0; // rand() % 4
        for (int i = 0; i < 4; i++)
        {
            if (i == correct_option)
                option_buttons[i].set_text(
                    std::to_string(i + 1) + ". " +
                    questions[question_index].correct_answer
                );
            else if (i < correct_option)
                option_buttons[i].set_text(
                    std::to_string(i + 1) + ". " +
                    questions[question_index].incorrect_answers[i]
                );
            else
                option_buttons[i].set_text(
                    std::to_string(i + 1) + ". " +
                    questions[question_index].incorrect_answers[i - 1]
                );
            option_buttons[i].draw();
        }
    }
    else if (more_questions_state == MoreQuestionsState::GETTING_MORE_QUESTIONS)
    {
        static Vector2 getting_more_questions_text_size = MeasureTextEx(
            *m04_font, "Getting more questions... Please be patient.",
            m04_font->baseSize / 2, 1
        );
        DrawTextEx(
            *m04_font, "Getting more questions... Please be patient.",
            Vector2(
                this->get_width() / 2 - getting_more_questions_text_size.x / 2,
                this->get_height() / 2 - getting_more_questions_text_size.y / 2
            ),
            m04_font->baseSize / 2, 1, RAYWHITE
        );
    }
    else if (more_questions_state == MoreQuestionsState::FAILED_TO_GET_QUESTIONS)
    {
        std::string text = "Oh no... We failed to get more questions!\nError: " +
                           getting_questions_error;
        Vector2 text_size =
            MeasureTextEx(*m04_font, text.c_str(), m04_font->baseSize / 2, 1);
        DrawTextEx(
            *m04_font, "Getting more questions... Please be patient.",
            Vector2(
                this->get_width() / 2 - text_size.x / 2,
                this->get_height() / 2 - text_size.y / 2
            ),
            m04_font->baseSize / 2, 1, RAYWHITE
        );
    }
}

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

void SurvivalFrame::more_questions()
{
    static std::thread *questions_thread = nullptr;
    if (questions_thread == nullptr)
        questions_thread = new std::thread([this]() {
            try
            {
                more_questions_state = MoreQuestionsState::GETTING_MORE_QUESTIONS;
                auto response = opentdb::get_questions(
                    20, opentdb::Category::ANY_CATEGORY, difficulty, "multiple"
                );
                if (response.response_code != 0) // CURLE_OK
                {
                    getting_questions_error = "Response with status code `" +
                                              std::to_string(response.response_code) +
                                              "`";
                    more_questions_state = MoreQuestionsState::FAILED_TO_GET_QUESTIONS;
                    return;
                }
                for (auto question : response.results)
                    questions.push_back(question);
                more_questions_state = MoreQuestionsState::GOT_MORE_QUESTIONS;
            }
            catch (std::runtime_error &exception)
            {
                getting_questions_error = exception.what();
                more_questions_state = MoreQuestionsState::FAILED_TO_GET_QUESTIONS;
                return;
            }
        });
    else if (more_questions_state == MoreQuestionsState::GOT_MORE_QUESTIONS)
    {
        delete questions_thread;
        questions_thread = nullptr;
    }
    else if (more_questions_state == MoreQuestionsState::FAILED_TO_GET_QUESTIONS)
        delete questions_thread;
}

SurvivalFrame::~SurvivalFrame()
{
    delete choosing_difficulty_easy_button;
    delete choosing_difficulty_medium_button;
    delete choosing_difficulty_hard_button;
    UnloadTexture(question_background);
}

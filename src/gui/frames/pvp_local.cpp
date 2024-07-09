#include "api/opentdb.hpp"
#include "gui/frames/pvp_local.hpp"
#include "gui/main.hpp"
#include "gui/widgets.hpp"
#include "raylib.h"
#include "utils/json.hpp"
#include <cstdlib>
#include <ctime>
#include <exception>
#include <filesystem>
#include <string>
#include <thread>

PvPLocalFrame::PvPLocalFrame(Window *parent) : Frame(parent)
{
    Image question_background_image = LoadImage("resources/question-background.png");
    ImageResize(&question_background_image, get_width() - 30, get_height() - 30);
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
    correct_option = rand() % 4;
}

void PvPLocalFrame::update() {}

void PvPLocalFrame::draw() {}

void PvPLocalFrame::update_choosing_difficulty()
{
    choosing_difficulty_easy_button->update();
    choosing_difficulty_medium_button->update();
    choosing_difficulty_hard_button->update();
    if (choosing_difficulty_easy_button->pressed ||
        choosing_difficulty_medium_button->pressed ||
        choosing_difficulty_hard_button->pressed)
        current_screen = PvPLocalScreen::PLAYER1_CHOOSING_DIFFICULTY;
    if (choosing_difficulty_easy_button->pressed)
        difficulty = "easy";
    else if (choosing_difficulty_medium_button->pressed)
        difficulty = "medium";
    else if (choosing_difficulty_hard_button->pressed)
        difficulty = "hard";
}

void PvPLocalFrame::draw_choosing_difficulty()
{
    choosing_difficulty_easy_button->draw();
    choosing_difficulty_medium_button->draw();
    choosing_difficulty_hard_button->draw();
}

void PvPLocalFrame::update_choosing_topic() {}

void PvPLocalFrame::draw_choosing_topic() {}

void PvPLocalFrame::update_answering_question()
{
    if (questions.size() - question_index < 4)
        more_questions();
    if (more_questions_state == PvPMoreQuestionsState::PVP_GETTING_MORE_QUESTIONS ||
        start_time == 0)
        start_time = time(0);
    if (time(0) - start_time > 15)
        game_over();
    if (question_index < questions.size())
        for (int i = 0; i < 4; i++)
        {
            option_buttons[i].update();
            if (option_buttons[i].pressed)
            {
                if (i == correct_option)
                // Correct choice
                {
                    correct_option = rand() % 4;
                    question_index++;
                    score++;
                    start_time = time(0);
                }
                // Incorrect choice
                else
                {
                    game_over();
                }
            }
        }
}

void PvPLocalFrame::draw_answering_question()
{
    if (question_index < questions.size())
    {
        DrawTexture(question_background, 15, 15, Color(0xff, 0xff, 0xff, 0x77));
        // Draw the timer (A number in the top right corner of screen)
        Vector2 timer_size = MeasureTextEx(
            *text_font, std::to_string(15 - time(0) + start_time).c_str(),
            text_font->baseSize / 2, 1
        );
        DrawTextEx(
            *text_font, std::to_string(15 - time(0) + start_time).c_str(),
            {get_width() - 40 - timer_size.x, 40}, text_font->baseSize / 2, 1, WHITE
        );
        // Draw the question and the options
        DrawTextEx(
            *text_font, questions[question_index].question.c_str(), {70, 70},
            text_font->baseSize / 2, 1, RAYWHITE
        );
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
    else if (more_questions_state == PvPMoreQuestionsState::PVP_GETTING_MORE_QUESTIONS)
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
    else if (more_questions_state == PvPMoreQuestionsState::PVP_FAILED_TO_GET_QUESTIONS)
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

void PvPLocalFrame::update_pvp_results()
{
    // Go to the ANSWERING_QUESTION screen when Enter button is pressed.
    if (IsKeyPressed(KEY_ENTER) ||
        (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && IsGestureDetected(GESTURE_TAP)))
    {
        question_index = 0;
        score = 0;
        start_time = 0;
        difficulty = "easy";
        getting_questions_error = "";
        more_questions_state = PvPMoreQuestionsState::PVP_GOT_MORE_QUESTIONS;
        questions.clear();
        current_screen = PvPLocalScreen::PLAYER1_CHOOSING_DIFFICULTY;
        parent->change_frame(CurrentFrame::menu);
    }
}

void PvPLocalFrame::draw_pvp_results()
{
    // Draw your score and high-score
    DrawTextEx(
        *text_font, ("Score: " + std::to_string(score)).c_str(), score_pos,
        text_font->baseSize, 1, WHITE
    );
    DrawTextEx(
        *text_font, ("High Score: " + std::to_string(highscore)).c_str(),
        Vector2(score_pos.x, score_pos.y + score_text_size.y), text_font->baseSize, 1,
        GOLD
    );
}

void PvPLocalFrame::more_questions()
{
    static std::thread *questions_thread = nullptr;
    if (questions_thread == nullptr)
        questions_thread = new std::thread([this]() {
            try
            {
                more_questions_state = PvPMoreQuestionsState::PVP_GETTING_MORE_QUESTIONS;
                auto response = opentdb::get_questions(
                    20, opentdb::Category::ANY_CATEGORY, difficulty, "multiple"
                );
                if (response.response_code != 0) // CURLE_OK
                {
                    getting_questions_error = "Response with status code `" +
                                              std::to_string(response.response_code) +
                                              "`";
                    more_questions_state =
                        PvPMoreQuestionsState::PVP_FAILED_TO_GET_QUESTIONS;
                }
                else
                {
                    for (auto question : response.results)
                        questions.push_back(question);
                    more_questions_state = PvPMoreQuestionsState::PVP_GOT_MORE_QUESTIONS;
                }
            }
            catch (std::exception &exception)
            {
                getting_questions_error = exception.what();
                more_questions_state = PvPMoreQuestionsState::PVP_FAILED_TO_GET_QUESTIONS;
            }
        });
    else if (more_questions_state == PvPMoreQuestionsState::PVP_GOT_MORE_QUESTIONS)
    {
        questions_thread->detach();
        delete questions_thread;
        questions_thread = nullptr;
    }
    else if (more_questions_state == PvPMoreQuestionsState::PVP_FAILED_TO_GET_QUESTIONS)
        delete questions_thread;
}

void PvPLocalFrame::game_over()
{
    // Load the current score log file containing the high-score
    json::JsonObject *survival_log;
    // Check if the survival.json exists and is not empty
    if (std::filesystem::exists("survival.json") &&
        std::filesystem::file_size("survival.json") > 0)
        survival_log = json::JsonObject::from_file("survival.json");
    else
        survival_log =
            json::JsonObject::parse_json("{\"highscore\": 0, \"games_played\": []}");
    const json::JsonObject *highscore_item = survival_log->get_item("highscore");
    if (highscore_item != nullptr)
        highscore = highscore_item->get_value<long long>();
    highscore = (score > highscore) ? score : highscore;
    survival_log->set_item("highscore", new json::JsonObject((long long)highscore));
    json::JsonObject *games_played = survival_log->get_item("games_played");
    if (games_played == nullptr)
    {
        games_played = json::JsonObject::parse_json("[]");
        survival_log->set_item("games_played", games_played);
    }
    games_played->push_back_item(json::JsonObject::parse_json(
        "{\"time\": " + std::to_string(time(0)) +
        ", \"score\": " + std::to_string(score) + "}"
    ));
    survival_log->to_file("survival.json");
    // Set the position for the score texts
    score_text_size = MeasureTextEx(
        *text_font, ("Score: " + std::to_string(score)).c_str(), text_font->baseSize, 1
    );
    highscore_text_size = MeasureTextEx(
        *text_font, ("High Score: " + std::to_string(highscore)).c_str(),
        text_font->baseSize, 1
    );
    score_pos = Vector2(
        this->get_width() / 2 - score_text_size.x / 2,
        this->get_height() / 2 - (score_text_size.y + highscore_text_size.y) / 2
    );
    // Change the screen
    current_screen = PvPLocalScreen::PVP_RESULTS;
}

PvPLocalFrame::~PvPLocalFrame()
{
    delete choosing_difficulty_easy_button;
    delete choosing_difficulty_medium_button;
    delete choosing_difficulty_hard_button;
    UnloadTexture(question_background);
}

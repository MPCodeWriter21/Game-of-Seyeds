#pragma once

#include "api/opentdb.hpp"
#include "gui/frame.hpp"
#include "gui/widgets.hpp"
#include "gui/window.hpp"
#include <vector>

enum PvPLocalScreen
{
    PLAYER1_CHOOSING_DIFFICULTY = 0,
    PLAYER1_CHOOSING_TOPIC = 1,
    PLAYER1_ANSWERING_QUESTIONS = 2,
    PLAYER2_CHOOSING_DIFFICULTY = 3,
    PLAYER2_CHOOSING_TOPIC = 4,
    PLAYER2_ANSWERING_QUESTIONS = 5,
    PVP_RESULTS = 6
};

enum PvPMoreQuestionsState
{
    PVP_GETTING_MORE_QUESTIONS = 0,
    PVP_FAILED_TO_GET_QUESTIONS = 1,
    PVP_GOT_MORE_QUESTIONS = 2
};

class PvPLocalFrame : public Frame
{
  public:
    PvPLocalFrame(Window *parent);
    void update();
    void draw();

    void update_choosing_difficulty();
    void draw_choosing_difficulty();

    void update_choosing_topic();
    void draw_choosing_topic();

    void update_answering_question();
    void draw_answering_question();

    void update_pvp_results();
    void draw_pvp_results();

    void more_questions();
    void game_over();

    ~PvPLocalFrame();

  private:
    Font *m04_font, *text_font;
    Sound *blink_sound;
    Texture2D question_background;
    Vector2 score_text_size, highscore_text_size, score_pos;

    Button *choosing_difficulty_easy_button, *choosing_difficulty_medium_button,
        *choosing_difficulty_hard_button;
    Button *option_buttons;

    PvPMoreQuestionsState more_questions_state = PvPMoreQuestionsState::PVP_GOT_MORE_QUESTIONS;
    std::string getting_questions_error = "";
    PvPLocalScreen current_screen = PvPLocalScreen::PLAYER1_CHOOSING_DIFFICULTY;
    std::string difficulty = "easy";
    std::vector<opentdb::Question> questions;
    size_t question_index = 0, score = 0, highscore = 0;
    int correct_option = 0;
    time_t start_time = 0;
};

#pragma once

#include "api/opentdb.hpp"
#include "gui/frame.hpp"
#include "gui/widgets.hpp"
#include "gui/window.hpp"
#include <vector>

enum SurvivalScreen
{
    CHOOSING_DIFFICULTY = 0,
    ANSWERING_QUESTION = 1,
    IN_BETWEEN_QUESTIONS = 2,
    SURVIVAL_RESULTS = 3
};

enum MoreQuestionsState
{
    GETTING_MORE_QUESTIONS = 0,
    FAILED_TO_GET_QUESTIONS = 1,
    GOT_MORE_QUESTIONS = 2
};

class SurvivalFrame : private Frame
{
  public:
    SurvivalFrame(Window *parent);
    void update();
    void draw();

    void update_choosing_difficulty();
    void draw_choosing_difficulty();

    void update_answering_question();
    void draw_answering_question();

    void update_in_between_questions();
    void draw_in_between_questions();

    void update_survival_results();
    void draw_survival_results();

    void more_questions();
    void game_over();

    ~SurvivalFrame();

  private:
    Font *m04_font, *text_font;
    Sound *blink_sound;
    Texture2D question_background;
    Vector2 score_text_size, highscore_text_size, score_pos;

    Button *choosing_difficulty_easy_button, *choosing_difficulty_medium_button,
        *choosing_difficulty_hard_button;
    Button *option_buttons;

    MoreQuestionsState more_questions_state = MoreQuestionsState::GOT_MORE_QUESTIONS;
    std::string getting_questions_error = "";
    SurvivalScreen current_screen = SurvivalScreen::CHOOSING_DIFFICULTY;
    std::string difficulty = "easy";
    std::vector<opentdb::Question> questions;
    size_t question_index = 0, score = 0, highscore = 0;
    int correct_option = 0;
    time_t start_time = 0;
};

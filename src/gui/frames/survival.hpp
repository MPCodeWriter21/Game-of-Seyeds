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
    IN_BETWEEN_QUESTIONS = 2
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

    void more_questions();

    ~SurvivalFrame();

  private:
    Font *m04_font, *text_font;
    Sound *blink_sound;
    Texture2D question_background;

    Button *choosing_difficulty_easy_button, *choosing_difficulty_medium_button,
        *choosing_difficulty_hard_button;
    Button *option_buttons;

    MoreQuestionsState more_questions_state = MoreQuestionsState::GOT_MORE_QUESTIONS;
    std::string getting_questions_error = "";
    SurvivalScreen current_screen = SurvivalScreen::CHOOSING_DIFFICULTY;
    std::string difficulty = "easy";
    std::vector<opentdb::Question> questions;
    size_t question_index = 0;
};

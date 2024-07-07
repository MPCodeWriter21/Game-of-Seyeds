#include "gui/frames/survival.hpp"
#include "raylib.h"

SurvivalFrame::SurvivalFrame(Window *parent) : Frame(parent) {}

void SurvivalFrame::update()
{
    switch (current_screen)
    {
        case CurrentScreen::CHOOSING_DIFFICULTY: break;
        case CurrentScreen::ANSWERING_QUESTION: break;
        case CurrentScreen::IN_BETWEEN_QUESTIONS: break;
    }
}

void SurvivalFrame::draw() {}

SurvivalFrame::~SurvivalFrame() {}

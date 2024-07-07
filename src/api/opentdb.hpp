#pragma once

#include "utils/json.hpp"
#include <string>
#include <vector>

namespace opentdb
{

enum Category
{
    ANY_CATEGORY = 0,
    GENERAL_KNOWLEDGE = 9,
    ENTERTAINMENT_BOOKS = 10,
    ENTERTAINMENT_FILMS = 11,
    ENTERTAINMENT_MUSIC = 12,
    ENTERTAINMENT_MUSICALS_AND_THEATRES = 13,
    ENTERTAINMENT_TELEVISION = 14,
    ENTERTAINMENT_VIDEO_GAMES = 15,
    ENTERTAINMENT_BOARD_GAMES = 16,
    SCIENCE_AND_NATURE = 17,
    SCIENCE_COMPUTERS = 18,
    SCIENCE_MATHEMATICS = 19,
    MYTHOLOGY = 20,
    SPORTS = 21,
    GEOGRAPHY = 22,
    HISTORY = 23,
    POLITICS = 24,
    ART = 25,
    CELEBRITIES = 26,
    ANIMALS = 27,
    VEHICLES = 28,
    ENTERTAINMENT_COMICS = 29,
    SCIENCE_GADGETS = 30,
    ENTERTAINMENT_JAPANESE_ANIME_AND_MANGA = 31,
    ENTERTAINMENT_CARTOON_AND_ANIMATION = 32
};

struct Question
{
    std::string type;
    std::string difficulty;
    std::string category;
    std::string question;
    std::string correct_answer;
    std::vector<std::string> incorrect_answers;
};

class ApiResponse
{
  public:
    int response_code;
    std::vector<Question> results;

    ApiResponse(int response_code, const std::vector<Question> &results)
        : response_code(response_code), results(results)
    {}
    ApiResponse(const json::JsonObject &parsed_json);
};

Question create_result(
    const std::string &type,
    const std::string &difficulty,
    const std::string &category,
    const std::string &question,
    const std::string &correct_answer,
    const std::vector<std::string> &incorrect_answers
);

Question create_result(const json::JsonObject &parsed_json);

ApiResponse get_questions(
    int amount,
    const Category &category,
    const std::string &difficulty = "", // Empty, "easy", "medium","hard"
    const std::string &type = ""        // Empty: Any type; "multiple": Multiple choice;
                                        // "boolean": True/False questions
);

} // namespace opentdb

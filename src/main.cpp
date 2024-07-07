#include "api/opentdb.hpp"
#include "gui/main.hpp"
#include "utils/utils.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

int main(void)
{
    srand(time(0));
    init_curl();

    MainApp app(1280, 590, true, "Game of Seyeds");
    app.main_loop();

    // try
    // {
    //     opentdb::ApiResponse res = opentdb::get_questions(
    //         2, opentdb::Category::SCIENCE_COMPUTERS, "hard", "multiple"
    //     );
    //     std::cout << "Response code: " << res.response_code << std::endl;
    //     for (auto &question : res.results)
    //     {
    //         std::cout << "Q: " << question.question << std::endl;
    //         std::vector<std::string> answers = question.incorrect_answers;
    //         answers.push_back(question.correct_answer);
    //         unsigned int x = rand();
    //         for (unsigned int i = x, j = 1; i < x + 4; i++, j++)
    //             std::cout << j << ") " << answers[i % 4] << std::endl;
    //         std::cout << std::endl
    //                   << "Correct answer: " << question.correct_answer << std::endl;
    //     }
    // }
    // catch (std::runtime_error &error)
    // {
    //     std::cerr << error.what() << std::endl;
    //     return 1;
    // }
    // catch (...)
    // {
    //     std::cerr << "Huh!\n";
    // }

    clean_curl();
    return 0;
}

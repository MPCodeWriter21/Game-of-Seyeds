#include "api/opentdb.hpp"
#include <cstdlib>
#include <ctime>
#include <curl/curl.h>
#include <iostream>

const char *OpenTDB_API_URL = "https://opentdb.com/api.php?amount=10&encode=url3986";

int main(int argc, char *argv[])
{
    srand(time(0));
    curl_global_init(CURL_GLOBAL_DEFAULT);

    try
    {
        opentdb::ApiResponse res = opentdb::get_questions(
            2, opentdb::Category::SCIENCE_COMPUTERS, "hard", "multiple"
        );
        std::cout << "Response code: " << res.response_code << std::endl;
        for (auto &question : res.results)
        {
            std::cout << "Q: " << question.question << std::endl;
            std::vector<std::string> answers = question.incorrect_answers;
            answers.push_back(question.correct_answer);
            unsigned int x = rand();
            for (unsigned int i = x, j = 1; i < x + 4; i++, j++)
                std::cout << j << ") " << answers[i % 4] << std::endl;
            std::cout << std::endl
                      << "Correct answer: " << question.correct_answer << std::endl;
        }
    }
    catch (std::runtime_error &error)
    {
        std::cerr << error.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Huh!\n";
    }

    curl_global_cleanup();
    return 0;
}

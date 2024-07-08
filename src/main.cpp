#include "gui/main.hpp"
#include "utils/utils.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

int main(void)
{
    srand(time(0));
    init_curl();

    try
    {
        MainApp app(1280, 590, true, "Game of Seyeds");
        app.main_loop();
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
    //                   << "Correct answer: " << question.correct_answer <<
    // std::endl;
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

// #include <curl/curl.h>
// #include <iostream>
// #include <thread>
//
// // Function to perform a cURL request
// void performCurlRequest()
// {
//     try
//     {
//         CURL *curl;
//         CURLcode res;
//
//         curl = curl_easy_init();
//         if (curl)
//         {
//             curl_easy_setopt(curl, CURLOPT_URL, "http://icanhazip.com");
//             curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
//             curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
//
//             res = curl_easy_perform(curl);
//
//             if (res != CURLE_OK)
//             {
//                 std::cerr << "cURL error in thread: " << curl_easy_strerror(res)
//                           << std::endl;
//             }
//             else
//             {
//                 std::cout << "Request successful in thread" << std::endl;
//             }
//
//             curl_easy_cleanup(curl);
//         }
//         else
//         {
//             std::cerr << "Failed to initialize cURL in thread" << std::endl;
//         }
//     }
//     catch (...)
//     {
//         std::cout << "Gotcha!\n";
//     }
// }
//
// int main()
// {
//     // Global cURL initialization
//     if (curl_global_init(CURL_GLOBAL_ALL) != 0)
//     {
//         std::cerr << "cURL global initialization failed" << std::endl;
//         return 1;
//     }
//
//     // Start a thread that performs a cURL request
//     std::thread curlThread(performCurlRequest);
//     std::thread curlThread2(performCurlRequest);
//
//     // Wait for the thread to finish
//     // curlThread.join();
//     // curlThread2.join();
//
//     // Global cURL cleanup
//     curl_global_cleanup();
//
//     return 0;
// }

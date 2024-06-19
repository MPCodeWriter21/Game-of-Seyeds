#include "api/opentdb.hpp"
#include "utils/exceptions.hpp"
#include "utils/json.hpp"
#include "utils/utils.hpp"
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <vector>

opentdb::ApiResponse::ApiResponse(const json::JsonObject &parsed_json)
{
    if (parsed_json.type != json::JsonObjectType::DICTIONARY)
        throw InvalidJsonObject(
            "ApiResponse: The input JSON object must be a dictionary."
        );
    auto response_code = parsed_json.get_item("response_code");
    if (response_code == nullptr)
        throw InvalidJsonObject(
            "ApiResponse: The input JSON object must have a `response_code` key."
        );
    if (response_code->type != json::JsonObjectType::INTEGER)
        throw InvalidJsonObject(
            "ApiResponse: The `response_code` key must be an integer."
        );
    auto results = parsed_json.get_item("results");
    if (results == nullptr)
        throw InvalidJsonObject(
            "ApiResponse: The input JSON object must have a `results` key."
        );
    if (results->type != json::JsonObjectType::LIST)
        throw InvalidJsonObject("ApiResponse: The `results` key must be a JSON array.");

    this->response_code = response_code->get_value<long long>();
    for (json::JsonObject *result : results->get_items())
        this->results.push_back(create_result(*result));
}

opentdb::Question opentdb::create_result(
    const std::string &type,
    const std::string &difficulty,
    const std::string &category,
    const std::string &question,
    const std::string &correct_answer,
    const std::vector<std::string> &incorrect_answers
)
{
    std::vector<std::string> decoded_incorrect_answers;
    for (const std::string &answer : incorrect_answers)
        decoded_incorrect_answers.push_back(url_decode(answer));
    return {
        type,
        difficulty,
        url_decode(category),
        url_decode(question),
        url_decode(correct_answer),
        decoded_incorrect_answers
    };
}

opentdb::Question opentdb::create_result(const json::JsonObject &parsed_json)
{
    if (parsed_json.type != json::JsonObjectType::DICTIONARY)
        throw InvalidJsonObject("create_result: The input JSON result must be a dictionary.");
    auto type = parsed_json.get_item("type");
    auto difficulty = parsed_json.get_item("difficulty");
    auto category = parsed_json.get_item("category");
    auto question = parsed_json.get_item("question");
    auto correct_answer = parsed_json.get_item("correct_answer");
    auto incorrect_answers = parsed_json.get_item("incorrect_answers");
    if (type == nullptr)
        throw InvalidJsonObject("create_result: The input JSON result must have a `type` key.");
    if (difficulty == nullptr)
        throw InvalidJsonObject("create_result: The input JSON result must have a `difficulty` key.");
    if (category == nullptr)
        throw InvalidJsonObject("create_result: The input JSON result must have a `category` key.");
    if (question == nullptr)
        throw InvalidJsonObject("create_result: The input JSON result must have a `question` key.");
    if (correct_answer == nullptr)
        throw InvalidJsonObject(
            "create_result: The input JSON result must have a `correct_answer` key."
        );
    if (incorrect_answers == nullptr)
        throw InvalidJsonObject(
            "create_result: The input JSON result must have a `incorrect_answers` key."
        );
    if (type->type != json::JsonObjectType::STRING)
        throw InvalidJsonObject("create_result: The `type` key must be a string.");
    if (difficulty->type != json::JsonObjectType::STRING)
        throw InvalidJsonObject("create_result: The `difficulty` key must be a string.");
    if (category->type != json::JsonObjectType::STRING)
        throw InvalidJsonObject("create_result: The `category` key must be a string.");
    if (question->type != json::JsonObjectType::STRING)
        throw InvalidJsonObject("create_result: The `question` key must be a string.");
    if (correct_answer->type != json::JsonObjectType::STRING)
        throw InvalidJsonObject("create_result: The `correct_answer` key must be a string.");
    if (incorrect_answers->type != json::JsonObjectType::LIST)
        throw InvalidJsonObject("create_result: The `incorrect_answers` key must be a JSON array.");

    std::vector<std::string> incorrect_answer_strings;
    for (const json::JsonObject *incorrect_answer : incorrect_answers->get_items())
    {
        if (incorrect_answer->type != json::JsonObjectType::STRING)
            throw InvalidJsonObject(
                "create_result: The `incorrect_answers` array must only contain strings."
            );
        incorrect_answer_strings.push_back(incorrect_answer->get_value<std::string>());
    }
    return create_result(
        type->get_value<std::string>(), difficulty->get_value<std::string>(),
        category->get_value<std::string>(), question->get_value<std::string>(),
        correct_answer->get_value<std::string>(), incorrect_answer_strings
    );
}

opentdb::ApiResponse opentdb::get_questions(
    int amount,
    const opentdb::Category &category,
    const std::string &difficulty, // Empty, "easy", "medium","hard"
    const std::string &type        // Empty: Any type; "multiple": Multiple
                                   // choice; "boolean": True/False questions
)
{
    std::string url =
        "https://opentdb.com/api.php?encode=url3986&amount=" + std::to_string(amount);
    if (category != opentdb::Category::ANY_CATEGORY)
        url += "&category=" + std::to_string((int)category);
    if (difficulty != "")
        url += "&difficulty=" + difficulty;
    if (type != "")
        url += "&type=" + type;

    CURL *curl = curl_easy_init();
    CURLcode response_code;
    std::string response_text;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_to_string_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_text);
        response_code = curl_easy_perform(curl);

        /* Check for errors */
        if (response_code != CURLE_OK)
            return opentdb::ApiResponse(response_code, {});

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    else
        throw CurlFailedToInitiateError();

    json::JsonObject *json_obj = json::JsonObject::parse_json(response_text);
    return opentdb::ApiResponse(*json_obj);
}

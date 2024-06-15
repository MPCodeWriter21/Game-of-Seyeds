#include "utils/json.hpp"
#include <curl/curl.h>
#include <fstream>
#include <iostream>

const char *OpenTDB_API_URL = "https://opentdb.com/api.php?amount=10&encode=url3986";

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

int main(int argc, char *argv[])
{
    std::string data = "";
    if (argc == 1)
    {
        std::cout << "No file was provided!" << std::endl
                  << "Downloading sample data from OpenTDB API..." << std::endl;
        CURL *curl;
        CURLcode response_code;

        curl_global_init(CURL_GLOBAL_DEFAULT);

        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, OpenTDB_API_URL);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
            response_code = curl_easy_perform(curl);
            /* Check for errors */
            if (response_code != CURLE_OK)
                std::cerr << "curl_easy_perform() failed: "
                          << curl_easy_strerror(response_code) << std::endl;

            /* always cleanup */
            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
    }
    // One or more arguments arguments provided
    else
    {
        std::ifstream file(argv[1]);
        char buffer[1024];
        while (!file.eof())
        {
            file.read(buffer, 1024);
            data.append(buffer, file.gcount());
        }
    }
    try
    {
        json::JsonObject *obj = json::JsonObject::parse_json(data);
        std::cout << obj->to_string() << std::endl;
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
    return 0;
}

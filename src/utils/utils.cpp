#include <curl/curl.h>
#include <string>

std::string url_encode(const std::string &data)
{
    const auto encoded_value =
        curl_easy_escape(nullptr, data.c_str(), static_cast<int>(data.length()));
    std::string result(encoded_value);
    curl_free(encoded_value);
    return result;
}

std::string url_decode(const std::string &url_encoded_data)
{
    int output_length;
    const auto decoded_value = curl_easy_unescape(
        nullptr, url_encoded_data.c_str(), static_cast<int>(url_encoded_data.length()),
        &output_length
    );
    std::string result(decoded_value, output_length);
    curl_free(decoded_value);
    return result;
}

size_t curl_write_to_string_callback(
    void *contents, size_t size, size_t nmemb, void *userp
)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

void init_curl() { curl_global_init(CURL_GLOBAL_DEFAULT); }

void clean_curl() { curl_global_cleanup(); }

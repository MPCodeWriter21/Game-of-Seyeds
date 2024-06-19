#pragma once

#include <string>

std::string url_encode(const std::string &data);
std::string url_decode(const std::string &url_encoded_data);
size_t curl_write_to_string_callback(
    void *contents, size_t size, size_t nmemb, void *userp
);

/*
 * JSON specifications: https://www.json.org/json-en.html
 * RFC8259: https://datatracker.ietf.org/doc/html/rfc8259
 */

#pragma once

#include <initializer_list>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace json
{

enum JsonObjectType
{
    null = 0,
    INTEGER = 1,
    FLOAT = 2,
    STRING = 3,
    BOOLEAN = 4,
    LIST = 5,
    DICTIONARY = 6
};

class JsonObject
{
    JsonObject(const JsonObject &) = delete;

  public:
    JsonObject();
    JsonObject(const long long &value);
    JsonObject(const double &value);
    JsonObject(const std::string &value);
    JsonObject(const bool &value);
    JsonObject(const std::vector<JsonObject *> &items);
    JsonObject(const std::initializer_list<JsonObject *> &items);
    JsonObject(const std::map<std::string, JsonObject *> &item_map);
    JsonObject(
        const std::initializer_list<std::string> &names,
        const std::initializer_list<JsonObject *> &items
    );
    JsonObject(const int &value) = delete;
    std::string to_string() const;
    const JsonObjectType &get_type() const;
    template <typename T> inline const T &get_value() const;
    const std::vector<JsonObject *> &get_items() const;
    const std::map<std::string, JsonObject *> &get_item_map() const;
    const JsonObject *get_item(const size_t index) const;
    const JsonObject *get_item(const std::string &key) const;
    JsonObject *set_item(const size_t index, JsonObject *value);
    JsonObject *set_item(const std::string &key, JsonObject *value);
    static JsonObject *parse_json(const std::string &json_text);
    static JsonObject *from_file(const std::string &file_path);
    ~JsonObject();

    const JsonObjectType type;

  private:
    static JsonObject *parse_dictionary(const std::string json_text);
    static JsonObject *parse_list(const std::string json_text);
    static JsonObject *parse_string(const std::string json_text);
    static JsonObject *parse_number(const std::string number_string);

    void *value = nullptr;
    std::vector<JsonObject *> *items = nullptr;
    std::map<std::string, JsonObject *> *item_map = nullptr;
};

std::string _carve_out(const std::string &whole_text, const size_t i);

template <typename T> inline const T &JsonObject::get_value() const
{
    // Check if the type is supported
    static_assert(
        std::is_same<T, long long>::value || std::is_same<T, double>::value ||
            std::is_same<T, std::string>::value || std::is_same<T, bool>::value,
        "Invalid type T"
    );
    if (type == JsonObjectType::null || type > JsonObjectType::BOOLEAN)
        throw std::invalid_argument(
            "This method only works for JsonObjects of type INTEGER(1), FLOAT(2), "
            "STRING(3) and BOOLEAN(4). Current object type: " +
            std::to_string(type)
        );
    return *(T *)value;
}

} // namespace json

#include "exceptions.hpp"
#include "json.hpp"
#include <cctype>
#include <cstddef>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

namespace json
{

std::string JsonObject::to_string() const
{
    if (type == JsonObjectType::null)
        return "null";
    if (type == JsonObjectType::BOOLEAN)
    {
        if (*(bool *)value == true)
            return "true";
        else if (*(bool *)value == false)
            return "false";
        else
            throw std::runtime_error(
                "Oh no...\nError in to_string: the type is BOOLEAN but the value is "
                "not either true or false! How? I have no idea!"
            );
    }
    if (type == JsonObjectType::INTEGER)
        return std::to_string(*(long long *)value);
    if (type == JsonObjectType::FLOAT)
        return std::to_string(*(double *)value);
    if (type == JsonObjectType::STRING)
    {
        std::string string_value = "\"";
        for (const char &c : *(std::string *)value)
        {
            switch (c)
            {
                case '\n': string_value += "\\n"; break;
                case '\r': string_value += "\\r"; break;
                case '\"': string_value += "\\\""; break;
                case '/': string_value += "\\/"; break;
                case '\f': string_value += "\\f"; break;
                case '\b': string_value += "\\b"; break;
                case '\t': string_value += "\\t"; break;
                default: string_value += c;
            }
        }
        return string_value + "\"";
    }
    if (type == JsonObjectType::LIST)
    {
        std::string string_value = "[";
        for (auto const &value : *items)
            string_value += value->to_string() + ',';
        if (string_value.ends_with(','))
            string_value = string_value.substr(0, string_value.length() - 1);
        return string_value + "]";
    }
    if (type == JsonObjectType::DICTIONARY)
    {
        std::string string_value = "{";
        for (auto const &[key, value] : *item_map)
            string_value += '"' + key + "\":" + value->to_string() + ',';
        if (string_value.ends_with(','))
            string_value = string_value.substr(0, string_value.length() - 1);
        return string_value + "}";
    }
    throw std::runtime_error(
        "What the hell? How was the type not handled in the if statements above?"
    );
}

const JsonObjectType &JsonObject::get_type() const { return type; }

const std::vector<JsonObject *> &JsonObject::get_items() const
{
    if (type != JsonObjectType::LIST)
        throw std::runtime_error(
            "This method only works for LISTs(5). Current object type: " +
            std::to_string(type)
        );
    return *items;
}

const std::map<std::string, JsonObject *> &JsonObject::get_item_map() const
{
    if (type != JsonObjectType::DICTIONARY)
        throw std::runtime_error(
            "This method only works for DICTIONARIES(6). Current object type: " +
            std::to_string(type)
        );
    return *item_map;
}

const JsonObject *JsonObject::get_item(const size_t index) const
{
    if (type != JsonObjectType::LIST)
        throw std::runtime_error(
            "This method only works for LISTs(5). Current object type: " +
            std::to_string(type)
        );
    return (*items)[index];
}

const JsonObject *JsonObject::get_item(const std::string &key) const
{
    if (type != JsonObjectType::DICTIONARY)
        throw std::runtime_error(
            "This method only works for DICTIONARIES(6). Current object type: " +
            std::to_string(type)
        );
    return (*item_map)[key];
}

JsonObject *JsonObject::parse_json(const std::string &json_text)
{
    // TODO: Handle some errors here and throw them back with more/more accurate
    // messages
    size_t i = 0;
    char first_char = json_text[0];
    while (std::isspace(first_char))
        first_char = json_text[++i];
    if (first_char == '{')
    {
        return JsonObject::parse_dictionary(_carve_out(json_text, i));
    }
    else if (first_char == '[')
    {
        return JsonObject::parse_list(_carve_out(json_text, i));
    }
    else if (first_char == '"')
    {
        return JsonObject::parse_string(_carve_out(json_text, i));
    }
    else if ((first_char >= '0' && first_char <= '9') || first_char == '-')
    {
        return JsonObject::parse_number(_carve_out(json_text, i));
    }
    else if (first_char == 't')
    {
        if (json_text.length() < i + 4)
            throw UnexpectedTokenError('t', -1, i);
        if (json_text[i + 1] != 'r' || json_text[i + 2] != 'u' ||
            json_text[i + 3] != 'e')
            throw UnexpectedTokenError('t', -1, i);
        return new JsonObject(true);
    }
    else if (first_char == 'f')
    {
        if (json_text.length() < i + 5)
            throw UnexpectedTokenError('f', -1, i);
        if (json_text[i + 1] != 'a' || json_text[i + 2] != 'l' ||
            json_text[i + 3] != 's' || json_text[i + 4] != 'e')
            throw UnexpectedTokenError('f', -1, i);
        return new JsonObject(false);
    }
    else if (first_char == 'n')
    {
        if (json_text.length() < i + 4)
            throw UnexpectedTokenError('n', -1, i);
        if (json_text[i + 1] != 'u' || json_text[i + 2] != 'l' ||
            json_text[i + 3] != 'l')
            throw UnexpectedTokenError('n', -1, i);
        return new JsonObject();
    }
    else
    {
        throw UnexpectedTokenError(first_char, -1, i);
    }
}

JsonObject *JsonObject::parse_dictionary(const std::string json_text)
{
    if (!(json_text.starts_with('{') && json_text.ends_with('}')))
        throw ParseListError("The list must start with an opening curly bracket({) "
                             "and end with a closing curly bracket(})!");
    std::map<std::string, JsonObject *> item_map;
    std::string *key = nullptr;
    JsonObject *key_obj = nullptr;
    for (size_t i = 1; i < json_text.size() - 1; i++)
    {
        if (std::isspace(json_text[i]))
            continue;
        if (key == nullptr)
        {
            if (json_text[i] != '"')
                throw UnexpectedTokenError(std::format(
                    "Found `{}`. Was expecting a double-quotation for a key of "
                    "the dictionary.",
                    json_text[i]
                ));
            key_obj = parse_string(_carve_out(json_text, i));
            key = new std::string(key_obj->get_value<std::string>());
            i += key->length() + 2;
            while (i < json_text.length())
            {
                if (std::isspace(json_text[i]))
                {
                    i++;
                    continue;
                }
                if (json_text[i] == ':')
                {
                    i++;
                    break;
                }
                throw UnexpectedTokenError(json_text[i], -1, i);
            }
        }
        else
        {
            std::string item = _carve_out(json_text, i);
            i += item.length();
            item_map[*key] = parse_json(item);
            delete key;
            delete key_obj;
            key = nullptr;
            while (i < json_text.length())
            {
                if (std::isspace(json_text[i]))
                {
                    i++;
                    continue;
                }
                if (json_text[i] == ',' || json_text[i] == '}')
                {
                    i++;
                    break;
                }
                throw UnexpectedTokenError(json_text[i], -1, i);
            }
        }
    }
    return new JsonObject(item_map);
}

JsonObject *JsonObject::parse_list(const std::string json_text)
{
    if (!(json_text.starts_with('[') && json_text.ends_with(']')))
        throw ParseListError("The list must start with an opening square bracket([) "
                             "and end with a closing square bracket(])!");
    std::vector<JsonObject *> items;
    for (size_t i = 1; i < json_text.size() - 1; i++)
    {
        if (std::isspace(json_text[i]))
            continue;
        std::string item = _carve_out(json_text, i);
        items.push_back(parse_json(item));
        i += item.length();
        while (i < json_text.length())
        {
            if (std::isspace(json_text[i]))
            {
                i++;
                continue;
            }
            if (json_text[i] == ',' || json_text[i] == ']')
            {
                i++;
                break;
            }
            throw UnexpectedTokenError(json_text[i], -1, i);
        }
    }
    return new JsonObject(items);
}

JsonObject *JsonObject::parse_string(const std::string json_text)
{
    if (!(json_text.starts_with('"') && json_text.ends_with('"')))
        throw ParseStringError(
            "The string must start and end with a double quotation(\")!"
        );
    std::string string_value = "";
    for (size_t i = 1; i < json_text.size() - 1; i++)
    {
        if (json_text[i] == '\\')
        {
            if (json_text[i + 1] == 'u' || json_text[i + 1] == 'U')
                // TODO: Make unicode work...
                throw ParseStringError("Ooh... Sorry! But this parser doesn't "
                                       "currently support unicode values.");
            if (json_text[i + 1] == 'x')
            {
                if (json_text.size() < i + 4)
                    throw ParseStringError("SyntaxError: \"\\x\" in a string must be "
                                           "followed by two hex digits!");
                try
                {
                    string_value +=
                        (char)std::stoi(json_text.substr(i + 2, 2), nullptr, 16);
                }
                catch (std::invalid_argument &error)
                {
                    throw ParseStringError(
                        "Failed to convert value `" + json_text.substr(i + 2, 2) +
                        "` to a character: " + error.what()
                    );
                }
                i += 2;
            }
            else if (json_text[i + 1] == '"')
                string_value += '"';
            else if (json_text[i + 1] == '\\')
                string_value += '\\';
            else if (json_text[i + 1] == '/')
                string_value += '/';
            else if (json_text[i + 1] == 'b')
                string_value += '\b';
            else if (json_text[i + 1] == 'f')
                string_value += '\f';
            else if (json_text[i + 1] == 'n')
                string_value += '\n';
            else if (json_text[i + 1] == 'r')
                string_value += '\r';
            else if (json_text[i + 1] == 't')
                string_value += '\t';
            else
                throw UnexpectedCharacterError(json_text[i + 1]);
            i++;
        }
        else
            string_value += json_text[i];
    }
    return new JsonObject(string_value);
}

JsonObject *JsonObject::parse_number(const std::string number_string)
{
    size_t dots = std::count(number_string.begin(), number_string.end(), '.');
    size_t es = std::count(number_string.begin(), number_string.end(), 'e');

    if (dots > 1)
        throw ParseNumberError("Reason: too many dots in the number!");
    if (es > 1)
        throw ParseNumberError("Reason: too many `e`s in the number!");

    if (dots == 0 && es == 0)
        try
        {
            return new JsonObject(std::stoll(number_string));
        }
        catch (std::invalid_argument &error)
        {
            throw ParseNumberError("Reason: " + (std::string)error.what());
        }
    try
    {
        return new JsonObject(std::stod(number_string));
    }
    catch (std::invalid_argument &error)
    {
        throw ParseNumberError("Reason: " + (std::string)error.what());
    }
}

JsonObject::JsonObject() : type(JsonObjectType::null) {}

JsonObject::JsonObject(const long long &value)
    : type(JsonObjectType::INTEGER), value((void *)new long long(value))
{}

JsonObject::JsonObject(const double &value)
    : type(JsonObjectType::FLOAT), value((void *)new double(value))
{}

JsonObject::JsonObject(const std::string &value)
    : type(JsonObjectType::STRING), value((void *)new std::string(value))
{}

JsonObject::JsonObject(const bool &value)
    : type(JsonObjectType::BOOLEAN), value((void *)new bool(value))
{}

JsonObject::JsonObject(const std::vector<JsonObject *> &items)
    : type(JsonObjectType::LIST)
{
    this->items = new std::vector<JsonObject *>(items);
}

JsonObject::JsonObject(const std::initializer_list<JsonObject *> &items)
    : type(JsonObjectType::LIST)
{
    this->items = new std::vector<JsonObject *>(items);
}

JsonObject::JsonObject(const std::map<std::string, JsonObject *> &item_map)
    : type(JsonObjectType::DICTIONARY)
{
    this->item_map = new std::map<std::string, JsonObject *>(item_map);
}

JsonObject::JsonObject(
    const std::initializer_list<std::string> &names,
    const std::initializer_list<JsonObject *> &items
)
    : type(JsonObjectType::DICTIONARY)
{
    if (names.size() != items.size())
        throw std::runtime_error("names and items must have the same number of elements"
        );
    this->item_map = new std::map<std::string, JsonObject *>();
    const std::string *name = names.begin();
    JsonObject json_object = items.begin();
    for (; name != names.end(); ++name)
        (*this->item_map)[*name] = &json_object;
}

JsonObject::~JsonObject()
{
    if (type == JsonObjectType::null)
        return;
    if (value != nullptr)
    {
        if (type == JsonObjectType::INTEGER)
            delete (long long *)value;
        else if (type == JsonObjectType::FLOAT)
            delete (double *)value;
        else if (type == JsonObjectType::STRING)
            delete (std::string *)value;
        else if (type == JsonObjectType::BOOLEAN)
            delete (bool *)value;
    }
    if (item_map != nullptr)
    {
        for (auto const &[key, value] : *item_map)
            delete value;
        delete item_map;
    }
    if (items != nullptr)
    {
        for (auto const value : *items)
            delete value;
        delete items;
    }
}

std::string _carve_out(const std::string &whole_text, const size_t start)
{
    size_t line = 1, temp = 0, index = 1;
    size_t i = start + 1;
    while (whole_text.find('\n', temp) < i)
    {
        line += 1;
        temp = whole_text.find('\n', temp) + 1;
        index = temp - start + 1;
    }
    if (whole_text[start] == '{' || whole_text[start] == '[' ||
        whole_text[start] == '"')
    {
        std::vector<std::tuple<char, size_t, size_t>> stack = {};
        stack.push_back(std::make_tuple(whole_text[start], line, index));
        bool in_string = whole_text[start] == '"';
        while (i < whole_text.length())
        {
            index += 1;
            if (in_string)
            {
                if (whole_text[i] == '\n' || whole_text[i] == '\t' ||
                    whole_text[i] == '\b' || whole_text[i] == '\f' ||
                    whole_text[i] == '\r' || whole_text[i] == '\v')
                    throw UnexpectedCharacterError(whole_text[i], line, index);
                if (whole_text[i] == '\\')
                    i++;
                else if (whole_text[i] == '"')
                {
                    in_string = false;
                    stack.pop_back();
                }
            }
            else if (whole_text[i] == '{' || whole_text[i] == '[')
            {
                stack.push_back(std::make_tuple(whole_text[i], line, index));
            }
            else if (whole_text[i] == '"')
            {
                stack.push_back(std::make_tuple('"', line, index));
                in_string = true;
            }
            else if (whole_text[i] == '}' || whole_text[i] == ']')
            {
                char opening;
                if (whole_text[i] == '}')
                    opening = '{';
                if (whole_text[i] == ']')
                    opening = '[';
                const char last_stack_char = std::get<0>(stack.back());
                if (last_stack_char == opening)
                    stack.pop_back();
                else
                    throw CarveOutError(std::format(
                        "`{}` at {}:{} was opened but never closed! Found non-matching "
                        "closing bracket(`{}`) at {}:{}.",
                        std::get<0>(stack.back()), std::get<1>(stack.back()),
                        std::get<2>(stack.back()), whole_text[i], line, index
                    ));
            }

            if (stack.size() == 0)
                return whole_text.substr(start, i - start + 1);

            if (whole_text[i] == '\n')
            {
                line += 1;
                index = 0;
            }
            i++;
        }
        throw CarveOutError(std::format(
            "Could not find the matching closing character for `{}` at {}:{}!",
            std::get<0>(stack.back()), std::get<1>(stack.back()),
            std::get<2>(stack.back())
        ));
    }
    else if ((whole_text[start] >= '0' && whole_text[start] <= '9') || whole_text[start] == '-')
    {
        std::string num = "";
        num += whole_text[start];
        while ((whole_text[i] >= '0' && whole_text[i] <= '9') || whole_text[i] == '.' ||
               whole_text[i] == 'e')
        {
            num += whole_text[i];
            i++;
        }
        if (std::count(num.begin(), num.end(), '.') > 1)
            throw CarveOutError(
                "The input data of type number must not contain more than one "
                "`.`!\nData: `" +
                num + "`"
            );
        else if (std::count(num.begin(), num.end(), 'e') > 1)
            throw CarveOutError(
                "The input data of type number must not contain more than one "
                "`e`!\nData: `" +
                num + "`"
            );
        else
            return num;
    }
    else if (whole_text[start] == 't')
    {
        if (whole_text.length() < i + 3)
            throw UnexpectedTokenError('t', -1, i);
        if (whole_text[i] != 'r' || whole_text[i + 1] != 'u' ||
            whole_text[i + 2] != 'e')
            throw UnexpectedTokenError('t', -1, i);
        return "true";
    }
    else if (whole_text[start] == 'f')
    {
        if (whole_text.length() < i + 4)
            throw UnexpectedTokenError('f', -1, i);
        if (whole_text[i] != 'a' || whole_text[i + 1] != 'l' ||
            whole_text[i + 2] != 's' || whole_text[i + 3] != 'e')
            throw UnexpectedTokenError('f', -1, i);
        return "false";
    }
    else if (whole_text[start] == 'n')
    {
        if (whole_text.length() < i + 3)
            throw UnexpectedTokenError('n', -1, i);
        if (whole_text[i] != 'u' || whole_text[i + 1] != 'l' ||
            whole_text[i + 2] != 'l')
            throw UnexpectedTokenError('n', -1, i);
        return "null";
    }
    else
        throw std::runtime_error(
            "The input data must begin with `{` or `[` or `\"` or be a number!"
        );
}

} // namespace json

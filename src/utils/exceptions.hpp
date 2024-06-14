#include <format>
#include <stdexcept>

#define LONG long long

class JsonParserError : public std::runtime_error
{
  public:
    JsonParserError() : std::runtime_error("JsonParserError") {}
    JsonParserError(const std::string &message) : std::runtime_error(message) {}
    const char *what() const { return std::runtime_error::what(); }
};

class CarveOutError : public JsonParserError
{
  public:
    CarveOutError() : JsonParserError("CarveOutError") {}
    CarveOutError(const std::string &message) : JsonParserError(message) {}
};

class UnexpectedCharacterError : public CarveOutError
{
    const std::string _escape_char(char character)
    {
        switch (character)
        {
            case '\n': return "\\n";
            case '\r': return "\\r";
            case '\t': return "\\t";
            case '\b': return "\\b";
            case '\f': return "\\f";
            case '\a': return "\\a";
            case '\v': return "\\v";
            default: return std::string(1, character);
        }
    }

  public:
    UnexpectedCharacterError() : CarveOutError("UnexpectedCharacterError") {}
    UnexpectedCharacterError(const std::string &message) : CarveOutError(message) {}
    UnexpectedCharacterError(const char &character)
        : CarveOutError(
              std::format("Unexpected `{}` character.", _escape_char(character))
          )
    {}
    UnexpectedCharacterError(
        const char &character, const LONG &line, const LONG &index
    )
        : CarveOutError(std::format(
              "Unexpected `{}` character at {}:{}", _escape_char(character), line, index
          ))
    {}
};

class UnexpectedTokenError : public JsonParserError
{
  public:
    UnexpectedTokenError() : JsonParserError("UnexpectedTokenError") {}
    UnexpectedTokenError(const std::string &message) : JsonParserError(message) {}
    UnexpectedTokenError(const char &character)
        : JsonParserError(std::format("Unexpected token `{}`.", character))
    {}
    UnexpectedTokenError(const char &character, const LONG &line, const LONG &index)
        : JsonParserError(
              std::format("Unexpected token `{}` at {}:{}", character, line, index)
          )
    {}
};

class ParseNumberError : public JsonParserError
{
  public:
    ParseNumberError() : JsonParserError("ParseNumberError") {}
    ParseNumberError(const std::string &message) : JsonParserError(message) {}
};

class ParseStringError : public JsonParserError
{
  public:
    ParseStringError() : JsonParserError("ParseStringError") {}
    ParseStringError(const std::string &message) : JsonParserError(message) {}
};

class ParseListError : public JsonParserError
{
  public:
    ParseListError() : JsonParserError("ParseListError") {}
    ParseListError(const std::string &message) : JsonParserError(message) {}
};

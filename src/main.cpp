#include "utils/json.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        std::cerr << "Need a file to process!\n";
        return 1;
    }
    std::ifstream file(argv[1]);
    std::string data = "";
    char buffer[1024];
    while (!file.eof())
    {
        file.read(buffer, 1024);
        data.append(buffer, file.gcount());
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
